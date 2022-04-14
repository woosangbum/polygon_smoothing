/*
================================================================================
Filename: smooth_polygon.cpp
Description: Definition of the SmoothPolygon class methods.
================================================================================
*/

#define _USE_MATH_DEFINES
#include <math.h>

// Program includes:
#include "smooth_polygon.h"
#include "misc_functions.h"



SmoothPolygon::SmoothPolygon(vector<Point> mp, float rq, int sp)
{
    // create the polygon
    this->num_major_points = mp.size();
    this->num_smooth_points = sp;
    this->num_minor_points = this->num_major_points * this->num_smooth_points;

    // round qualities
    this->round_qualities = rq;

    // Calculate coordinates of the polygon major and minor points (de Casteljan algorithm)
    this->setMajorPoints(mp);
    this->calcMinorPoints();
}


void SmoothPolygon::setMajorPoints(vector<Point> mp)
{
    for (int i = 0; i < this->num_major_points; ++i)
    { 
        this->major_points.push_back(mp[i]);
    }
    
}

void SmoothPolygon::calcMinorPoints()
{
    /* Method for calculation of the minor polygon points. Calculations are performed
       utilizing de Casteljan algorithm. */

    Point prev_major_point;  // Previous major point of the polygon
    Point cur_major_point;  // Curent major point of the polygon
    Point next_major_point;  // Next major point of the polygon

    Point begin_edge_point;  // Begin edge point of the Bezier curve
    Point end_edge_point;  // End edge point of the Bezier curve
    Point result_minor_point;  // Point sliding the edge between prev and next major points

    int i, j;  // Temporary counters
    float round_quality;  // Current major point round quality value
    float delta_t;  // Delta t parameter for Bezier points calculation
    float t;  // t parameter for Bezier points calculation (0 - 1)

    // Clear the vector of minor polygon points
    this->minor_points.clear();

    for (i = 0; i < this->num_major_points; ++i)
    {
        // Current major point definition:
        cur_major_point = this->major_points[i];

        // Previous major point definition:
        if (i == 0)
            prev_major_point = this->major_points[this->num_major_points - 1];
        else
            prev_major_point = this->major_points[i - 1];

        // Next major point definition:
        if (i == (this->num_major_points - 1))
            next_major_point = this->major_points[0];
        else
            next_major_point = this->major_points[i + 1];

        // Calculate the begin and end point for Bezier points calculation:
        round_quality = this->round_qualities;
        delta_t = 1.0f / (this->num_smooth_points - 1);
        t = 0.0f;

        begin_edge_point = find_between_point(prev_major_point, cur_major_point,
            (1.0f - round_quality));
        end_edge_point = find_between_point(cur_major_point, next_major_point, round_quality);

        // Cycle to calculate the minor point positions
        for (j = 0; j < this->num_smooth_points; ++j)
        {
            result_minor_point = calc_bezier_point(begin_edge_point, cur_major_point,
                end_edge_point, t);
            this->minor_points.push_back(result_minor_point);


            t += delta_t;
        }
    }
}


