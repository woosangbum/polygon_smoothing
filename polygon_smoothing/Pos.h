#pragma once
class Pos
{
public:
	double x, y, group;
	Pos(double, double);
	Pos();
	Pos operator+(Pos const& obj);
	Pos operator-(Pos const& obj);
	Pos mul(double x);
};

