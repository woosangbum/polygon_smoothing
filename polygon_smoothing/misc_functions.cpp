/*
================================================================================
Filename: misc_functions.cpp
Description: Definition of misc_functions
================================================================================
*/

#pragma once
#include "smooth_polygon.h"
#include "misc_functions.h"
#define M_PI 3.14159

Pos find_between_point(Pos begin_point, Pos end_point, float alpha)
{
    float x_begin = static_cast<float>(begin_point.x);
    float y_begin = static_cast<float>(begin_point.y);
    float x_end = static_cast<float>(end_point.x);
    float y_end = static_cast<float>(end_point.y);
    float x_result, y_result;
    Pos result;

    x_result = x_begin + alpha * (x_end - x_begin);
    y_result = y_begin + alpha * (y_end - y_begin);

    result.x = x_result;
    result.y = y_result;

    return result;
}

Pos calc_bezier_point(Pos begin_point, Pos middle_point, Pos end_point, float t)
{
    float x_begin = static_cast<float>(begin_point.x);
    float y_begin = static_cast<float>(begin_point.y);
    float x_middle = static_cast<float>(middle_point.x);
    float y_middle = static_cast<float>(middle_point.y);
    float x_end = static_cast<float>(end_point.x);
    float y_end = static_cast<float>(end_point.y);
    float x_result, y_result;
    Pos result;

    // calculate x coordinates
    x_result = std::pow((1 - t), 2) * x_begin + 2 * (1 - t) * t * x_middle + \
        std::pow(t, 2) * x_end;

    // calculate y coordinates
    y_result = std::pow((1 - t), 2) * y_begin + 2 * (1 - t) * t * y_middle + \
        std::pow(t, 2) * y_end;

    result.x = x_result;
    result.y = y_result;

    return result;
}

double dist(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

float getCurvature(Pos p1, Pos p2, Pos p3) {
    double R;
    double cross = sqrt(pow((p3.x - p2.x), 2) + pow((p3.y - p2.y), 2));
    double cross1 = sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
    double cross2 = sqrt(pow((p3.x - p1.x), 2) + pow((p3.y - p1.y), 2));
    double cross3 = abs(((p3.x - p2.x) * (p1.y - p2.y)) - ((p3.y - p2.y) * (p1.x - p2.x)));
    R = (cross * cross1 * cross2) / (2 * cross3);
    return R;
}

Pos pointToLine(Pos mp1, Pos mp2, Pos ugv) {
    Pos H;
    float x1 = mp1.x;
    float y1 = mp1.y;
    float x2 = mp2.x;
    float y2 = mp2.y;
    double gradient1 = (y2-y1) / (x2-x1);
    double gradient2 = 0;
    double var1[3];
    double var2[3];

    // The coefficient of the first equation for the simultaneous equation
    var1[0] = gradient1;
    var1[1] = -1;
    var1[2] = -gradient1*x1 + y1;

    // Coefficients of the second equation for the simultaneous equation (including exception processing)
    if (y2 == y1) {
        return Pos(ugv.x, y1);
    }
    else if (x1 == y1) {
        return Pos(x1, ugv.y);
    }
    else {
        gradient2 = -(1 / gradient1);
        var2[0] = gradient2;
        var2[1] = -1;
        var2[2] = ugv.y - gradient2 * ugv.x;

        equation(var1, var2, H);
        return H;
    }
}

void equation(const double* var1, const double* var2, Pos& H)
{
    double t_var1[3];
    double t_var2[3];
    double x, y;

    if (var1[0] == 0 && var1[1] == 0) {
        H.x = 0;
    }
    if (var2[0] == 0 && var2[1] == 0) {
        H.x = 0;

    }

    if (var1[0] == 0 && var2[0] == 0) {
        y = var1[2] / var1[1];
        if (y != (var2[2] / var2[1])) {
            H.x = 0;

        }
        H.x = 0;

    }

    if (var1[1] == 0 && var2[1] == 0) {
        x = var1[2] / var1[0];
        if (x != (var2[2] / var2[0])) {
            H.x = 0;

        }
        H.x = 1;

    }

    for (int i = 0; i < 3; i++) {
        t_var1[i] = var1[i] * var2[0];
        t_var2[i] = var2[i] * var1[0];
    }
    if (var1[0] != 0) {
        y = (t_var2[2] - t_var1[2]) / (t_var1[1] - t_var2[1]);
        x = (-var1[2] - var1[1] * y) / var1[0];
    }
    else {
        y = var1[2] / var1[1];
        x = (-var2[2] - var2[1] * y) / var2[0];
    }
    H.x = x;
    H.y = y;
}

vector<Pos> getLinearInterpolation(vector<Pos> tmp, bool circular) {
    int cnt = 0;
    double node_interval = 5.0;  // the interval between each point

    vector<Pos> result;
    int num;

    while (cnt < tmp.size()) {
        int next = cnt + 1;

        // Case classification based on whether the path you want to interpolate is a circular path or not
        if (cnt == tmp.size() - 1) {
            if (circular) next = 0;
            else return result;
        }

        double id1_x = tmp[cnt].x;
        double id2_x = tmp[next].x;
        double id1_y = tmp[cnt].y;
        double id2_y = tmp[next].y;

        double temp_dist = dist(id1_x, id1_y, id2_x, id2_y);

        int n = ceil((1 / node_interval) * temp_dist + 1);  // Number of points to enter between points

        result.push_back(Pos(id1_x, id1_y));

        for (int i = 1; i < n; i++) {
            double temp_point_x = (id1_x * (n - 1 - i) + id2_x * i) / (n - 1);
            double temp_point_y = (id1_y * (n - 1 - i) + id2_y * i) / (n - 1);
            result.push_back(Pos(temp_point_x, temp_point_y));
        }
        cnt += 1;

        result.push_back(Pos(id2_x, id2_y));

    }
    return result;

}