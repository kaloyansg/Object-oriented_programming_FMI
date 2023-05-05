#include "FlightsScanner.h"
#include "Utils.h"
#include <iostream>
#include <cstring>
#include <sstream>

#pragma warning (disable : 4996)

FlightsScanner::FlightsScanner(const char* input, const char* output)
{
	strcpy(inputFileName, input);
	strcpy(outputFileName, output);
}

void FlightsScanner::handleAirportCode(char* airportCode) const
{
	const unsigned short airportCodeSize = 3;
	if (strlen(airportCode) != airportCodeSize)
	{
		throw std::length_error("The airport code size should be exactly 3");
	}
	toUpper(airportCode);
}

void FlightsScanner::parseLine(const char* line, FlightsScanner::FlightRecord& record) const
{
	std::stringstream current(line);

	current >> record.origin;
	try
	{
		handleAirportCode(record.origin);
	}
	catch (const std::length_error& e)
	{
		throw;
	}

	current >> record.dest;
	try
	{
		handleAirportCode(record.dest);
	}
	catch (const std::length_error& e)
	{
		throw;
	}

	char amountStr[64];
	current >> amountStr;
	try
	{
		record.amount = strToNumber(amountStr);
	}
	catch (const std::bad_cast& e)
	{
		throw;
	}
}

void FlightsScanner::saveRecord(std::ofstream& ofs, const FlightRecord& record) const
{
	const float euroToLev = 1.95;
	ofs << record.origin << " " << record.dest << " " << record.amount * euroToLev << std::endl;
}

bool FlightsScanner::readAndExportData(std::ifstream& ifs) const
{
	std::ofstream ofs(outputFileName, std::ios::trunc);
	if (!ofs.is_open())
	{
		std::cout << "File could not be open." << std::endl;
		return false;
	}

	unsigned int lineIndex = 0;

	bool errorOccurred = false;
	while (!ifs.eof())
	{
		char line[1024];
		ifs.getline(line, 1024);

		FlightRecord record;
		try
		{
			parseLine(line, record);
			saveRecord(ofs, record);
		}
		catch (const std::length_error& e)
		{
			std::cout << "The length error \"" << e.what() << "\" occurred on line " << lineIndex << ". The flight will NOT be saved!" << std::endl;
			errorOccurred = true;
		}
		catch (const std::bad_cast& e)
		{
			std::cout << "The bad cast error \"" << e.what() << "\" occurred on line " << lineIndex << ". The flight will be saved!" << std::endl;
			saveRecord(ofs, record);
			errorOccurred = true;
		}
		catch (const std::exception& e)
		{
			std::cout << "The unknown error \"" << e.what() << "\" ccured on line " << lineIndex << ". The flight will NOT be saved!" << std::endl;
			errorOccurred = true;
		}
		lineIndex++;
	}
	return errorOccurred;
}

void FlightsScanner::run()
{
	std::ifstream ifs(inputFileName);
	if (!ifs.is_open())
	{
		std::cout << "File not found!" << std::endl;
		return;
	}

	if (readAndExportData(ifs))
	{
		std::cout << "Successfully exported file!" << std::endl;
	}
	else
	{
		std::cout << "Error occurred while exporting the file!" << std::endl;
	}
	ifs.close();
}