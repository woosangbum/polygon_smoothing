/*
================================================================================
Filename: misc_functions.h
Description: Definition of misc functions
================================================================================
*/

#ifndef MISC_FUNCTIONS_H_
#define MISC_FUNCTIONS_H_
#include <cmath>
#include "Pos.h"

// A function that calculates the point between two points
Pos find_between_point(Pos begin_point, Pos end_point, float alpha);

// Function to calculate points via bezier_curve
Pos calc_bezier_point(Pos begin_point, Pos middle_point, Pos end_point,
    float t);

// Calculate the distance between two points
double dist(double x1, double y1, double x2, double y2);

float getCurvature(Pos p1, Pos p2, Pos p3);

// Return the foot of perpendicular from point UGV to the straight line made using point mp1 and point mp2
Pos pointToLine(Pos mp1, Pos mp2, Pos ugv);

// Function to find the answer to the quadratic equation
void equation(const double* var1, const double* var2, Pos& H);

// A function that provides linear interpolation
vector<Pos> getLinearInterpolation(vector<Pos> tmp, bool circular);

#endif // MISC_FUNCTIONS_H_
