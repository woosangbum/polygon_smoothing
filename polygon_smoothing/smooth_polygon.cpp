/*
================================================================================
Filename: smooth_polygon.cpp
Description: Definition of the SmoothPolygon class methods.
================================================================================
*/

#include "smooth_polygon.h"
#include "misc_functions.h"

//initiallize
SmoothPolygon::SmoothPolygon(float rq, int sp)
{

    this->num_smooth_points = sp;
    this->round_qualities = rq;

}

vector<Point> SmoothPolygon::getSmoothPolygon(vector<Point> mp, float mc) {
    
    double lowest_dist = dist(mp[1].x(), mp[1].y(), mp[0].x(), mp[0].y());
    while (lowest_dist < 100) { // 뭉쳐있는 점 처리
        int li = lowest_idx(mp);
        lowest_dist = dist(mp[li + 1].x(), mp[li + 1].y(), mp[li].x(), mp[li].y());

        mp[li] = Point((mp[li + 1].x() + mp[li].x()) / 2, (mp[li + 1].y() + mp[li].y()) / 2);
        mp.erase(mp.begin() + li + 1);
    }

    //동일 기울기 점들 제거
    double cur_gradient = gradient(mp[1].x(), mp[1].y(), mp[0].x(), mp[0].y());

    for (vector<int>::size_type i = 1; i < mp.size() - 1; i++) {
        double g = gradient(mp[i + 1].x(), mp[i + 1].y(), mp[i].x(), mp[i].y());

        if (fabs(cur_gradient - g) < 1.0) {
            mp.erase(mp.begin() + i);
        }
        cur_gradient = g;
    }
    

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

// Smoothing Polygon
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

vector<Point> SmoothPolygon::calcMinorPoints_Each(vector<Point> mp, int mpn, int mi, float rt, int sp)
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
    cur_major_point = mp[mi];

    // Previous major point definition:
    if (mi == 0)
        prev_major_point = mp[mpn - 1];
    else
        prev_major_point = mp[mi - 1];

    // Next major point definition:
    if (mi == (mpn - 1))
        next_major_point = mp[0];
    else
        next_major_point = mp[mi + 1];

    // Calculate the begin and end point for Bezier points calculation:
    round_quality = rt;
    delta_t = 1.0f / (sp - 1);
    t = 0.0f;

    begin_edge_point = find_between_point(prev_major_point, cur_major_point,
        (1.0f - round_quality));
    end_edge_point = find_between_point(cur_major_point, next_major_point, round_quality);

    // Cycle to calculate the minor point positions
    for (j = 0; j < sp; ++j)
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
    int lastPoint = num_smooth_points - 1;
    int midPoint = num_smooth_points/2;
    double R;
    double R_temp;
    float curvature_t = 0;

    vector<Point> tempMinorPoints;

    for (int i = 0; i < num_major_points * num_smooth_points; i += num_smooth_points) {
        float q_temp = this->round_qualities;

        R = getCurvature(this->minor_points[i], this->minor_points[i + midPoint], this->minor_points[i + lastPoint]);
        float curvature = 1 / R;
        cout << "곡률 반경 확인 전: " << curvature << endl;
        curvature_t = curvature;
        while (curvature_t > mc && q_temp <= 4.5) {
            q_temp += 0.1;
            
            tempMinorPoints = calcMinorPoints_Each(this->major_points, this->num_major_points, i/num_smooth_points, q_temp, this->num_smooth_points);

            for (int j = 0; j < this->num_smooth_points; j++) {
                this->minor_points[i + j] = tempMinorPoints[j];
            }
            R_temp = getCurvature(this->minor_points[i], this->minor_points[i + midPoint], this->minor_points[i + lastPoint]);
            
            curvature_t = 1 / R_temp;
        }
        cout << "곡률 반경 확인 후: " << curvature_t << endl;


    }
    
}

// Smoothing Entry Path
vector<Point> SmoothPolygon::getEntryPath(Point startPoint) {
    // create the entry path
    //this->entryPoint = closestPoint(startPoint, mp);
    //this->secondPoint = closestPoint(entryPoint, mp);
    //vector<Point> tempMinorPoints;
    //int entry = 0;
    //Point entryPoint;
    //Point secondPoint;
    vector<Point> entryPath;
    //Point Point1= this->minor_points[0];
    //Point Point2= closestPoint100(Point1, this->minor_points);
    //vector<Point> ep;
    //ep.push_back(startPoint);
    //ep.push_back(Point1);
    //ep.push_back(Point2);
    //int lastPoint = num_smooth_points - 1;
    //int midPoint = num_smooth_points / 2;
    //float minCost = getCurvature(ep[0], ep[1], ep[2]);
    //Point a[2];
    //vector<Point> temp;
    //for (int i = 0; i < num_minor_points; i++) {
    //        
    //    temp = calcMinorPoints_Each(ep, 3, 1, this->round_qualities, this->num_smooth_points);
    //
    //    float R = getCurvature(temp[0], temp[midPoint], temp[lastPoint]);
    //    if (minCost >= R) {
    //        minCost = R;
    //        ep[1] = this->minor_points[i];
    //        ep[2] = closestPoint100(ep[1], this->minor_points);
    //        a[0] = temp[midPoint];
    //        a[1] = temp[lastPoint];
    //    }
    //}
    //
    //entryPath.push_back(startPoint);
    //entryPath.push_back(a[0]);
    //entryPath.push_back(a[1]);
    
    Point Point1 = this->major_points[0];
    Point Point2 = closestPoint100(Point1, this->major_points);
    entryPath.push_back(startPoint);
    entryPath.push_back(Point1);
    double cur_gradient = gradient(startPoint.x(), startPoint.y(), Point1.x(), Point1.y());
    double g = gradient(Point1.x(), Point1.y(), Point2.x(), Point2.y());
    double ming = fabs(cur_gradient - g);
    float next;
    for (vector<int>::size_type i = 1; i < this->major_points.size(); i++) {
        if (i == this->major_points.size() - 1) next = 0;
        else next = i + 1;

        g = gradient(this->major_points[next].x(), this->major_points[next].y(), this->major_points[i].x(), this->major_points[i].y());
        cur_gradient = gradient(startPoint.x(), startPoint.y(), this->major_points[i].x(), this->major_points[i].y());

        if (fabs(cur_gradient - g) <= ming) {
            entryPath[1] = this->major_points[i];
        }
    }
    //entryPath[1] = closestPoint(entryPath[1], this->minor_points);
    
    return entryPath;
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