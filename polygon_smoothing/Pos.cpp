#include "Pos.h"
Pos::Pos()
{
	x = 0.0;
	y = 0.0;
	group = -1;
}
Pos::Pos(double nx, double ny)
{
	x = nx;
	y = ny;
	group = -1;
}
Pos Pos::operator+(Pos const& obj) {
	Pos re;
	re.x = x + obj.x;
	re.y = y + obj.y;
	return re;
}
Pos Pos::operator-(Pos const& obj) {
	Pos re;
	re.x = x - obj.x;
	re.y = y - obj.y;
	return re;
}
Pos Pos::mul(double a) {
	Pos re;
	re.x = x *a;
	re.y = y *a;
	return re;
}

