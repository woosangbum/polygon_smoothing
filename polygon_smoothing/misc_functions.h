
/*
================================================================================
Filename: misc_functions.h
Description: Declaration of the misc functions
================================================================================
*/


#ifndef MISC_FUNCTIONS_H_
#define MISC_FUNCTIONS_H_
#include <cmath>
#include "Pos.h"

Pos find_between_point(Pos begin_point, Pos end_point, float alpha);
// Function to calculate the point position between other two points

Pos calc_bezier_point(Pos begin_point, Pos middle_point,
    Pos end_point, float t);
// Function to calculate the point position by bezier_curve

double dist(double x1, double y1, double x2, double y2);
double gradient(double x1, double y1, double x2, double y2);
int lowest_idx(vector<Pos> p);
Pos closestPoint(Pos s, vector<Pos> v);
Pos closestPoint100(Pos s, vector<Pos> v);
float getCurvature(Pos p1, Pos p2, Pos p3);
double angle(Pos a, Pos b, Pos c);
Pos pointToLine(Pos mp1, Pos mp2, Pos ugv);
void equation(const double* var1, const double* var2, Pos& H);
vector<Pos> getLinearInterpolation(vector<Pos> tmp, bool circular);
double getPathLength(vector<Pos> mp);
#endif // MISC_FUNCTIONS_H_
