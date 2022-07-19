#include "smooth_polygon.h"
#include "misc_functions.h"
#define M_PI 3.14159

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

double dist(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

double gradient(double x1, double y1, double x2, double y2) {
    if (x1 == x2) {
        if(y2 > y1) return 100000;
        else return -100000;
    }
    
    return (y1 - y2)/ (x1 - x2);
}

int lowest_idx(vector<Point> p) {
    double min = 1000000;
    int li = 0;
    for (vector<int>::size_type i = 0; i < p.size() - 1; i++) {
        double d = dist(p[i + 1].x(), p[i + 1].y(), p[i].x(), p[i].y());
        if (min > d) {
            li = i;
            min = d;
        }
    }
    return li;
}

Point closestPoint(Point s, vector<Point> v) {
    Point cp = Point(100000, 100000);
    double minDist = dist(cp.a, cp.b, s.a, s.b);

    // get vertex closest to the current position
    for (int i = 0; i < v.size(); i++) {
        double d = dist(v[i].a, v[i].b, s.a, s.b);
        if (minDist > d && d > 10) {
            cp = v[i];
            minDist = d;
        }
    }
    return cp;
}

Point closestPoint100(Point s, vector<Point> v) {
    Point cp = Point(100000, 100000);
    double minDist = dist(cp.a, cp.b, s.a, s.b);

    // get vertex closest to the current position
    for (int i = 0; i < v.size(); i++) {
        double d = dist(v[i].a, v[i].b, s.a, s.b);
        if (minDist > d && d > 100) {
            cp = v[i];
            minDist = d;
        }
    }
    return cp;

}

float getCurvature(Point p1, Point p2, Point p3) {
    double R;
    double cross = sqrt(pow((p3.a - p2.a), 2) + pow((p3.b - p2.b), 2));
    double cross1 = sqrt(pow((p2.a - p1.a), 2) + pow((p2.b - p1.b), 2));
    double cross2 = sqrt(pow((p3.a - p1.a), 2) + pow((p3.b - p1.b), 2));
    double cross3 = abs(((p3.a - p2.a) * (p1.b - p2.b)) - ((p3.b - p2.b) * (p1.a - p2.a)));
    R = (cross * cross1 * cross2) / (2 * cross3);
    return R;
}

double angle(Point a, Point b, Point c) {
    double aa, bb, cc;
    double ang, temp;

    aa = sqrt(pow(a.x() - c.x(), 2) + pow(a.y() - c.y(), 2));
    bb = sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(), 2));
    cc = sqrt(pow(b.x() - c.x(), 2) + pow(b.y() - c.y(), 2));

    temp = (pow(bb, 2) + pow(cc, 2) - pow(aa, 2)) / (2 * bb * cc);
    ang = acos(temp);
    ang = ang * (180 / M_PI);
    return ang;
}