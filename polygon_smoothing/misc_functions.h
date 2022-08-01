/*
================================================================================
Filename: misc_functions.h
Description: Declaration of the misc functions
================================================================================
*/

#ifndef MISC_FUNCTIONS_H_
#define MISC_FUNCTIONS_H_

Point find_between_point(Point begin_point, Point end_point, float alpha);
// Function to calculate the point position between other two points

Point calc_bezier_point(Point begin_point, Point middle_point,
    Point end_point, float t);
// Function to calculate the point position by bezier_curve

double dist(double x1, double y1, double x2, double y2);
double gradient(double x1, double y1, double x2, double y2);
int lowest_idx(vector<Point> p);
Point closestPoint(Point s, vector<Point> v);
Point closestPoint100(Point s, vector<Point> v);
float getCurvature(Point p1, Point p2, Point p3);
double angle(Point a, Point b, Point c);
Point pointToLine(Point mp1, Point mp2, Point ugv);
void equation(const double* var1, const double* var2, Point& H);
#endif // MISC_FUNCTIONS_H_
