/*
================================================================================
Filename: smooth_polygon.cpp
Description: Definition of the SmoothPolygon class methods.
================================================================================
*/

#include "smooth_polygon.h"
#include "misc_functions.h"

//initiallize

SmoothPolygon::SmoothPolygon() {}
SmoothPolygon::SmoothPolygon(float rq, int sp)
{
    this->num_smooth_points = sp;
    this->round_qualities = rq;
}

vector<Pos> SmoothPolygon::getSmoothPolygon(vector<Pos> mp, float mc) {
    
    if (mp[0].x == mp[mp.size() - 1].x && mp[0].y == mp[mp.size() - 1].y) mp.pop_back();

    this->num_major_points = mp.size();
    this->num_minor_points = this->num_major_points * this->num_smooth_points;

    // Calculate coordinates of the polygon major and minor points (de Casteljan algorithm)
    this->setMajorPoints(mp);
    this->calcMinorPoints();
    this->checkCurvature(mc);

    return this->minor_points;
}

// Smoothing Polygon
void SmoothPolygon::setMajorPoints(vector<Pos> mp)
{
    for (int i = 0; i < this->num_major_points; ++i)
    { 
        this->major_points.push_back(mp[i]);
    }
}

void SmoothPolygon::calcMinorPoints()
{
    Pos prev_major_point;  // Previous major point of the polygon
    Pos cur_major_point;  // Curent major point of the polygon
    Pos next_major_point;  // Next major point of the polygon

    Pos begin_edge_point;  // Begin edge point of the Bezier curve
    Pos end_edge_point;  // End edge point of the Bezier curve
    Pos result_minor_point;  // Pos sliding the edge between prev and next major points

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

vector<Pos> SmoothPolygon::calcMinorPoints_Each(vector<Pos> mp, int mpn, int mi, float rt, int sp)
{
    Pos prev_major_point;  // Previous major point of the polygon
    Pos cur_major_point;  // Curent major point of the polygon
    Pos next_major_point;  // Next major point of the polygon

    Pos begin_edge_point;  // Begin edge point of the Bezier curve
    Pos end_edge_point;  // End edge point of the Bezier curve
    Pos result_minor_point;  // Pos sliding the edge between prev and next major points

    int i, j;  // Temporary counters
    float round_quality;  // Current major point round quality value
    float delta_t;  // Delta t parameter for Bezier points calculation
    float t;  // t parameter for Bezier points calculation (0 - 1)

    // Clear the vector of minor polygon points
    vector<Pos> minorPoints;

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

    vector<Pos> tempMinorPoints;

    for (int i = 0; i < num_major_points * num_smooth_points; i += num_smooth_points) {
        float q_temp = this->round_qualities;

        R = getCurvature(this->minor_points[i], this->minor_points[i + midPoint], this->minor_points[i + lastPoint]);
        float curvature = 1 / R;
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
    }
}

// Smoothing Entry Path
vector<Pos> SmoothPolygon::getEntryPath(Pos startPoint) {
    vector<Pos> entryPath;
    vector<Pos> HsCCW;
    vector<Pos> HsCW;
    int minIdx = -1;
    int minDist = 100000000;
    Pos MinDistMP;
    int minIdx2 = -1;
    int minDist2 = 100000000;
    Pos MinDistMP2;

    Pos MinDistH;
    int  next = 0;

    // StartPoint에서 Major Pos 두 개를 통해 만들어진 직선에 내린 수선의 발을 저장하는 벡터 생성(Hs) - Counter Clock Wise
    for (int i = 0; i < this->num_major_points; i++) {
        if (i == this->major_points.size() - 1) next = 0;
        else next = i + 1;
        Pos H = pointToLine(this->major_points[i], this->major_points[next], startPoint);
        HsCCW.push_back(H);
    }

    // StartPoint에서 Major Pos 두 개를 통해 만들어진 직선에 내린 수선의 발을 저장하는 벡터 생성(Hs) - Clock Wise
    for (int i = this->num_major_points-1; i>=0; i--) {
        if (i == 0) next = this->num_major_points - 1;
        else next = i - 1;
        Pos H = pointToLine(this->major_points[i], this->major_points[next], startPoint);
        HsCW.push_back(H);

    }

    // Major Points 중, StartPoint와 가장 가까운 점 탐색(진입점 탐색)
    for (int i = 0; i < this->num_major_points; i++) {
        double distMp = dist(this->major_points[i].x, this->major_points[i].y, startPoint.x, startPoint.y);
        if (minDist > distMp) {
            minIdx = i;
            minDist = distMp;
        }
    }

    // CCW 와 CW 진입경로 중, 더 짧은 경로를 선택
    double CCWdist = dist(this->major_points[minIdx].x, this->major_points[minIdx].y, HsCCW[minIdx].x, HsCCW[minIdx].y) + \
        dist(HsCCW[minIdx].x, HsCCW[minIdx].y, startPoint.x, startPoint.y);
    double CWdist = dist(this->major_points[minIdx].x, this->major_points[minIdx].y, HsCW[minIdx].x, HsCW[minIdx].y) + \
        dist(HsCW[minIdx].x, HsCW[minIdx].y, startPoint.x, startPoint.y);

    MinDistH = (CCWdist > CWdist) ? HsCW[minIdx] : HsCCW[minIdx];

    // 진입 major point와 가장 가까운 minor point 탐색
    for (int i = 0; i < this->num_minor_points; i++) {
        double distMp = dist(this->major_points[minIdx].x, this->major_points[minIdx].y, this->minor_points[i].x, this->minor_points[i].y);
        if (minDist2 > distMp) {
            minIdx2 = i;
            minDist2 = distMp;
        }
    }

    // Enrty Path의 기본이 되는 세 개의 점을 저장하고, 베지에 커브 적용
    vector<Pos> ThreePoints;
    ThreePoints.push_back(startPoint);
    ThreePoints.push_back(MinDistH);
    ThreePoints.push_back(this->minor_points[minIdx2]);
    this->entryPoint = this->minor_points[minIdx2];
    entryPath = calcMinorPoints_Each(ThreePoints, 3, 1, 1, 10);
    this->entryPointIdx = minIdx2;
    return entryPath;
}

void SmoothPolygon::setMajorPoints_entry(vector<Pos> mp)
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

    Pos prev_major_point;  // Previous major point of the polygon
    Pos cur_major_point;  // Curent major point of the polygon
    Pos next_major_point;  // Next major point of the polygon

    Pos begin_edge_point;  // Begin edge point of the Bezier curve
    Pos end_edge_point;  // End edge point of the Bezier curve
    Pos result_minor_point;  // Pos sliding the edge between prev and next major points

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

vector<Pos> SmoothPolygon::getTotalPath(Pos startPoint) {
    vector<Pos> sp = this->minor_points;
    vector<Pos> ep = getEntryPath(startPoint);
    ep.reserve(sp.size() + ep.size());
    ep.insert(ep.end(), sp.begin() + entryPointIdx + 1, sp.end());
    ep.insert(ep.end(), sp.begin(), sp.begin() + entryPointIdx - 1);
    return ep;
}

