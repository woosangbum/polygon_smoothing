/*
================================================================================
Filename: smooth_polygon.cpp
Description: Definition of SmoothPolygon
================================================================================
*/

#include "smooth_polygon.h"
#include "misc_functions.h"

SmoothPolygon::SmoothPolygon() {}

SmoothPolygon::SmoothPolygon(float rq, int sp)
{
    round_qualities = rq;
    num_smooth_points = sp;
}

SmoothPolygon::~SmoothPolygon() {}

vector<Pos> SmoothPolygon::getSmoothPolygon(vector<Pos> mp, float mc)
{
    //(Data preprocessing) If the beginning and end of the input data are the same, delete one element at the end
    if (mp[0].x == mp[mp.size() - 1].x && mp[0].y == mp[mp.size() - 1].y)
        mp.pop_back();

    num_major_points = mp.size();
    num_minor_points = num_major_points * num_smooth_points;

    setMajorPoints(mp); // Setting Major Points
    calcMinorPoints();  // Calculate Minor Points(Smoothed Points)
    checkCurvature(mc); // Checking Curvature

    return minor_points;
}

void SmoothPolygon::setMajorPoints(vector<Pos> mp)
{
    // Set Major Points on SmoothPolygon Object
    for (int i = 0; i < this->num_major_points; ++i)
    {
        this->major_points.push_back(mp[i]);
    }
}

void SmoothPolygon::calcMinorPoints()
{
    // Function to calculate Minor Points (smoothed points)

    // Three points to apply Bezier_curve
    Pos prev_major_point;
    Pos cur_major_point;
    Pos next_major_point;

    Pos begin_edge_point;  // Begin edge point to apply Bezier_curve
    Pos end_edge_point;  // End edge point to apply Bezier_curve
    Pos result_minor_point;  // Sliding point between prev_major_point and next_major_point

    int i, j;
    float t;  // Parameter t applied to the Bezier_curv algorithm
    float delta_t;  //the amount of change in t (0 - 1)

    minor_points.clear();

    for (i = 0; i < num_major_points; ++i)
    {
        // Definition of Current major point
        cur_major_point = major_points[i];

        // Definition of Previous major point
        if (i == 0)
            prev_major_point = major_points[num_major_points - 1];
        else
            prev_major_point = major_points[i - 1];

        // Definition of Next major point
        if (i == (num_major_points - 1))
            next_major_point = major_points[0];
        else
            next_major_point = major_points[i + 1];

        // Calculate the points at the beginning and end of the Bezier_curv
        delta_t = 1.0f / (num_smooth_points - 1);
        t = 0.0f;

        begin_edge_point = find_between_point(prev_major_point, cur_major_point,
                                              (1.0f - round_qualities));
        end_edge_point = find_between_point(cur_major_point, next_major_point,
                                            round_qualities);

        // Loop for calculating Minor points
        for (j = 0; j < num_smooth_points; ++j)
        {
            result_minor_point = calc_bezier_point(begin_edge_point, cur_major_point,
                                                   end_edge_point, t);
            minor_points.push_back(result_minor_point);

            t += delta_t;
        }
    }
}

vector<Pos> SmoothPolygon::calcMinorPoints_Each(vector<Pos> mp, int mpn, int mi, float rt, int sp)
{
    /*
        Equivalent to the calcMinorPoints function,
        but smoothing for a particular vertex consisting of three points
    */

    Pos prev_major_point;
    Pos cur_major_point;
    Pos next_major_point;

    Pos begin_edge_point;
    Pos end_edge_point;
    Pos result_minor_point;

    int i, j;
    float round_quality;
    float delta_t;
    float t;

    vector<Pos> minorPoints;

    cur_major_point = mp[mi];

    if (mi == 0)
        prev_major_point = mp[mpn - 1];
    else
        prev_major_point = mp[mi - 1];

    if (mi == (mpn - 1))
        next_major_point = mp[0];
    else
        next_major_point = mp[mi + 1];

    round_quality = rt;
    delta_t = 1.0f / (sp - 1);
    t = 0.0f;

    begin_edge_point = find_between_point(prev_major_point, cur_major_point,
                                          (1.0f - round_quality));
    end_edge_point = find_between_point(cur_major_point, next_major_point, round_quality);

    for (j = 0; j < num_smooth_points; ++j)
    {
        result_minor_point = calc_bezier_point(begin_edge_point, cur_major_point,
                                               end_edge_point, t);
        minorPoints.push_back(result_minor_point);

        t += delta_t;
    }
    return minorPoints;
}

void SmoothPolygon::checkCurvature(float mc)
{
    /*
        A function that increases smoothing_quilty
        if the curvature of the vertex is small compared to the reciprocal of
        the minimum turning radius of the vehicle
    */

    int lastPoint = num_smooth_points - 1;
    int midPoint = num_smooth_points / 2;
    double R;
    double R_temp;
    float curvature_t = 0;

    vector<Pos> tempMinorPoints;

    for (int i = 0; i < num_major_points * num_smooth_points; i += num_smooth_points)
    {
        float q_temp = this->round_qualities;

        R = getCurvature(this->minor_points[i], this->minor_points[i + midPoint], this->minor_points[i + lastPoint]);
        curvature_t = 1 / R;

        // Increase the smoothing qulity by 0.1
        while (curvature_t > mc && q_temp <= 4.5)
        {
            q_temp += 0.1;

            tempMinorPoints = calcMinorPoints_Each(this->major_points, this->num_major_points, i / num_smooth_points, q_temp, this->num_smooth_points);

            for (int j = 0; j < this->num_smooth_points; j++)
            {
                this->minor_points[i + j] = tempMinorPoints[j];
            }
            R_temp = getCurvature(this->minor_points[i], this->minor_points[i + midPoint], this->minor_points[i + lastPoint]);

            curvature_t = 1 / R_temp;
        }
    }
}

vector<Pos> SmoothPolygon::getEntryPath(Pos startPoint)
{
    // Creating entry path and Smoothing
    vector<Pos> entryPath;
    vector<Pos> HsCCW;
    vector<Pos> HsCW;
    int minIdx = -1;
    int minDist = 100000000;
    int minIdx2 = -1;
    int minDist2 = 100000000;
    Pos MinDistMP;
    Pos MinDistMP2;

    Pos MinDistH;
    int next = 0;

    // Store foot of perpendicular for all straight lines created counterclockwise
    for (int i = 0; i < num_major_points; i++)
    {
        if (i == major_points.size() - 1)
            next = 0;
        else
            next = i + 1;
        Pos H = pointToLine(major_points[i], major_points[next], startPoint);
        HsCCW.push_back(H);
    }

    // Store foot of perpendicular for all straight lines created clockwise
    for (int i = num_major_points - 1; i >= 0; i--)
    {
        if (i == 0)
            next = num_major_points - 1;
        else
            next = i - 1;
        Pos H = pointToLine(major_points[i], major_points[next], startPoint);
        HsCW.push_back(H);
    }

    // Obtain the nearest major point from the UGV position
    for (int i = 0; i < num_major_points; i++)
    {
        double distMp = dist(major_points[i].x, major_points[i].y, startPoint.x, startPoint.y);
        if (minDist > distMp)
        {
            minIdx = i;
            minDist = distMp;
        }
    }

    double CCWdist = dist(major_points[minIdx].x, major_points[minIdx].y, HsCCW[minIdx].x, HsCCW[minIdx].y) +
                     dist(HsCCW[minIdx].x, HsCCW[minIdx].y, startPoint.x, startPoint.y);
    double CWdist = dist(major_points[minIdx].x, major_points[minIdx].y, HsCW[minIdx].x, HsCW[minIdx].y) +
                    dist(HsCW[minIdx].x, HsCW[minIdx].y, startPoint.x, startPoint.y);

    MinDistH = (CCWdist > CWdist) ? HsCW[minIdx] : HsCCW[minIdx];

    // Explore the nearest Major Point and the nearest smoothing point found above
    for (int i = 0; i < num_minor_points; i++)
    {
        double distMp = dist(major_points[minIdx].x, major_points[minIdx].y, minor_points[i].x, minor_points[i].y);
        if (minDist2 > distMp)
        {
            minIdx2 = i;
            minDist2 = distMp;
        }
    }

    // Apply Bezier_curve with UGV position, foot of perpendicular, and nearest smoothing point
    vector<Pos> ThreePoints;
    ThreePoints.push_back(startPoint);
    ThreePoints.push_back(MinDistH);
    ThreePoints.push_back(minor_points[minIdx2]);
    entryPoint = minor_points[minIdx2];
    entryPath = calcMinorPoints_Each(ThreePoints, 3, 1, 1, 10);
    entryPointIdx = minIdx2;
    return entryPath;
}

vector<Pos> SmoothPolygon::getTotalPath(Pos startPoint)
{
    // A function that connects the entry path and the smoothing path in order
    vector<Pos> sp = this->minor_points;
    vector<Pos> ep = getEntryPath(startPoint);
    ep.reserve(sp.size() + ep.size());
    ep.insert(ep.end(), sp.begin() + entryPointIdx + 1, sp.end());
    ep.insert(ep.end(), sp.begin(), sp.begin() + entryPointIdx - 1);
    ep.push_back(sp[entryPointIdx + 1]);
    return ep;
}