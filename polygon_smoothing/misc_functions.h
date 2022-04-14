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

#endif // MISC_FUNCTIONS_H_
