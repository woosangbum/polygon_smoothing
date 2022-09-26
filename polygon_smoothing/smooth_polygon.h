/*
================================================================================
Filename: smooth_polygon.h
Description: Definition of SmoothPolygon
================================================================================
*/

#ifndef SMOOTH_POLYGON_H_
#define SMOOTH_POLYGON_H_

#include <vector>
#include <iostream>
#include "Pos.h"

using namespace std;

class SmoothPolygon
{
private:
    // polygon
    int num_major_points = 0;
    int num_minor_points = 0;
    vector<Pos> major_points;
    vector<Pos> minor_points;

    // entry path
    int num_major_points_entry = 0;
    vector<Pos> major_points_entry;
    int num_minor_points_entry = 0;
    vector<Pos> minor_points_entry;
    Pos entryPoint;

public:
    float round_qualities = 0;  // The degree of the smoothing vertex, the larger the vertex, the more rounded it becomes
    int num_smooth_points = 0;  // The number of points that make up a single smoothed vertex
    int entryPointIdx=0;

    SmoothPolygon();
    SmoothPolygon(float rq, int sp);
    ~SmoothPolygon();
    vector<Pos> getSmoothPolygon(vector<Pos> mp, float mc);
    vector<Pos> getEntryPath(Pos startPoint);
    vector<Pos> getTotalPath(Pos startPoint);
    void setMajorPoints(vector<Pos> mp);
    void calcMinorPoints();
    vector<Pos> calcMinorPoints_Each(vector<Pos> mp, int mpn, int mi, float rt, int sp);
    void calcMinorPoints_entry();
    void checkCurvature(float mc);
};

#endif // SMOOTH_POLYGON_H_
