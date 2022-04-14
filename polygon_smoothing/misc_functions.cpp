#include "smooth_polygon.h"
#include <iostream>

Point find_between_point(Point begin_point, Point end_point, float alpha)
{
    // Function to calculate the point position between other two points
    float x_begin = static_cast<float>(begin_point.x());
    float y_begin = static_cast<float>(begin_point.y());
    float x_end = static_cast<float>(end_point.x());
    float y_end = static_cast<float>(end_point.y());
    float x_result, y_result;
    Point result;

    x_result = x_begin + alpha * (x_end - x_begin);
    y_result = y_begin + alpha * (y_end - y_begin);

    result.setX(x_result);
    result.setY(y_result);

    return result;
}

Point calc_bezier_point(Point begin_point, Point middle_point, Point end_point, float t)
{
    // Function to calculate the arbitrary Bezier point
    float x_begin = static_cast<float>(begin_point.x());
    float y_begin = static_cast<float>(begin_point.y());
    float x_middle = static_cast<float>(middle_point.x());
    float y_middle = static_cast<float>(middle_point.y());
    float x_end = static_cast<float>(end_point.x());
    float y_end = static_cast<float>(end_point.y());
    float x_result, y_result;
    Point result;

    // Calculate x point coordinate
    x_result = std::pow((1 - t), 2) * x_begin + 2 * (1 - t) * t * x_middle + \
        std::pow(t, 2) * x_end;

    // Calcualte y point coordinate
    y_result = std::pow((1 - t), 2) * y_begin + 2 * (1 - t) * t * y_middle + \
        std::pow(t, 2) * y_end;

    result.setX(x_result);
    result.setY(y_result);

    return result;
}