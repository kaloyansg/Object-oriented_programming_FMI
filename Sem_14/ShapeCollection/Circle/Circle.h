#pragma once
#include "../Shape/Shape.h"

class Circle : public Shape
{
private:
	double radius;

public:
	Circle(int x, int y, double radius);

	virtual double getArea() const override;
	virtual double getPerimeter() const override;

	virtual bool isPointInShape(int x, int y) const override;

	virtual Shape* clone() const override;
};