/*
================================================================================
Filename: smooth_polygon.cpp
Description: Definition of the SmoothPolygon class methods.
================================================================================
*/

// Program includes:
#include "smooth_polygon.h"
#include "misc_functions.h"



SmoothPolygon::SmoothPolygon(float rq, int sp)
{

    this->num_smooth_points = sp;
    this->round_qualities = rq;

}

vector<Point> SmoothPolygon::getSmoothPolygon(vector<Point> mp, float mc) {
    /*
    double lowest_dist = 0;
    while (lowest_dist < 200) { // ¹¶ÃÄÀÖ´Â Á¡ Ã³¸®
        int li = lowest_idx(mp);
        lowest_dist = dist(mp[li + 1].x(), mp[li + 1].y(), mp[li].x(), mp[li].y());

        mp[li] = Point((mp[li + 1].x() + mp[li].x()) / 2, (mp[li + 1].y() + mp[li].y()) / 2);
        mp.erase(mp.begin() + li + 1);
    }

    double cur_gradient = gradient(mp[1].x(), mp[1].y(), mp[0].x(), mp[0].y());

    for (vector<int>::size_type i = 1; i < mp.size() - 1; i++) {
        double g = gradient(mp[i + 1].x(), mp[i + 1].y(), mp[i].x(), mp[i].y());

        if (fabs(cur_gradient - g) < 1.0) {
            mp.erase(mp.begin() + i);
        }
        cur_gradient = g;
    }
    */

    // create the polygon
    this->num_major_points = mp.size();
    this->num_minor_points = this->num_major_points * this->num_smooth_points;

    // round qualities

    // Calculate coordinates of the polygon major and minor points (de Casteljan algorithm)
    this->setMajorPoints(mp);
    this->calcMinorPoints();
    this->checkCurvature(mc);

    return this->minor_points;
}

vector<Point> SmoothPolygon::getEntryPath(Point startPoint, vector<Point> mp) {
    // create the entry path
    this->entryPoint = closestPoint(startPoint, mp);
    this->secondPoint = closestPoint(entryPoint, mp);
    this->entryPath.push_back(startPoint);
    this->entryPath.push_back(entryPoint);
    this->entryPath.push_back(secondPoint);

    this->num_major_points_entry = entryPath.size();
    this->num_smooth_points = this->num_smooth_points;
    this->num_minor_points_entry = this->num_smooth_points + 2;

    // round qualities
    this->round_qualities = this->round_qualities;

    // Calculate coordinates of the polygon major and minor points (de Casteljan algorithm)
    this->setMajorPoints_entry(entryPath);
    this->minor_points_entry.push_back(startPoint);


    this->calcMinorPoints_entry();

    this->minor_points_entry.push_back(this->secondPoint);

    return this->minor_points_entry;
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

vector<Point> SmoothPolygon::calcMinorPoints_Each(int mi, float rt)
{
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
    vector<Point> minorPoints;

    // Current major point definition:
    cur_major_point = this->major_points[mi];

    // Previous major point definition:
    if (mi == 0)
        prev_major_point = this->major_points[this->num_major_points - 1];
    else
        prev_major_point = this->major_points[mi - 1];

    // Next major point definition:
    if (mi == (this->num_major_points - 1))
        next_major_point = this->major_points[0];
    else
        next_major_point = this->major_points[mi + 1];

    // Calculate the begin and end point for Bezier points calculation:
    round_quality = rt;
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
        minorPoints.push_back(result_minor_point);

        t += delta_t;
    }

    return minorPoints;
}

void SmoothPolygon::setMajorPoints_entry(vector<Point> mp)
{
    for (int i = 0; i < this->num_major_points_entry; ++i)
    {
        this->major_points_entry.push_back(mp[i]);
    }

}

void SmoothPolygon::calcMinorPoints_entry()
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
    int centerI = this->major_points_entry.size() / 2;
    // Clear the vector of minor polygon points

    cur_major_point = this->major_points_entry[centerI];

    prev_major_point = this->major_points_entry[centerI - 1];


    next_major_point = this->major_points_entry[centerI + 1];

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
        this->minor_points_entry.push_back(result_minor_point);


        t += delta_t;
    }

}

void SmoothPolygon::checkCurvature(float mc)
{
    int lastPoint = num_smooth_points - 1;
    int midPoint = num_smooth_points/2;
    double R;
    double R_temp;
    float curvature_t = 0;

    vector<Point> tempMinorPoints;

    for (int i = 0; i < num_major_points * num_smooth_points; i += num_smooth_points) {
        float q_temp = this->round_qualities;

        double cross = sqrt(pow((this->minor_points[i + lastPoint].a - this->minor_points[i + midPoint].a), 2) + pow((this->minor_points[i + lastPoint].b - this->minor_points[i + midPoint].b), 2));
        double cross1 = sqrt(pow((this->minor_points[i + midPoint].a - this->minor_points[i].a), 2) + pow((this->minor_points[i + midPoint].b - this->minor_points[i].b), 2));
        double cross2 = sqrt(pow((this->minor_points[i + lastPoint].a - this->minor_points[i].a), 2) + pow((this->minor_points[i + lastPoint].b - this->minor_points[i].b), 2));
        double cross3 = abs(((this->minor_points[i + midPoint].a - this->minor_points[i + midPoint].a)*(this->minor_points[i].b - this->minor_points[i + midPoint].b)) - ((this->minor_points[i + lastPoint].b - this->minor_points[i + midPoint].b)*(this->minor_points[i].a - this->minor_points[i + midPoint].a)));
        R = (cross*cross1*cross2) / (2 * cross3);
        float curvature = 1 / R;
        cout << curvature << endl;
        curvature_t = curvature;
        while (curvature_t > mc && q_temp <= 4.5) {
            q_temp += 0.01;

            tempMinorPoints = calcMinorPoints_Each(i, q_temp);
            for (int j = 0; j < this->num_smooth_points; j++) {
                this->minor_points[i + j] = tempMinorPoints[j];
            }
            double cross_t = sqrt(pow((this->minor_points[i + lastPoint].a - this->minor_points[i + midPoint].a), 2) + pow((this->minor_points[i + lastPoint].b - this->minor_points[i + midPoint].b), 2));
            double cross1_t = sqrt(pow((this->minor_points[i + midPoint].a - this->minor_points[i].a), 2) + pow((this->minor_points[i + midPoint].b - this->minor_points[i].b), 2));
            double cross2_t = sqrt(pow((this->minor_points[i + lastPoint].a - this->minor_points[i].a), 2) + pow((this->minor_points[i + lastPoint].b - this->minor_points[i].b), 2));
            double cross3_t = abs(((this->minor_points[i + midPoint].a - this->minor_points[i + midPoint].a) * (this->minor_points[i].b - this->minor_points[i + midPoint].b)) - ((this->minor_points[i + lastPoint].b - this->minor_points[i + midPoint].b) * (this->minor_points[i].a - this->minor_points[i + midPoint].a)));
            R_temp = (cross_t * cross1_t * cross2_t) / (2 * cross3_t);
            curvature_t = 1 / R_temp;
        }
        cout << "°î·ü ¹Ý°æ È®ÀÎ ÈÄ: " << curvature_t << endl;


    }
    
}


