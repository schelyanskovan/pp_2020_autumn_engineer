// Copyright 2020 Khismatulina Karina
#include <algorithm>
#include <vector>

#ifndef MODULES_TASK_3_KHISMATULINA_K_GRAHAM_GRAHAM_H_
#define MODULES_TASK_3_KHISMATULINA_K_GRAHAM_GRAHAM_H_

class Point {
 public:
    double x;
    double y;
    Point() { x = 0; y = 0; }
    Point(double _x, double _y) : x(_x), y(_y) {}
    Point& operator=(const Point& A) {
        if (this != &A) {
            x = A.x;
            y = A.y;
        }
        return *this;
    }
    bool operator==(const Point& p) {
        return ((x == p.x) && (fabs(y - p.y) < 1e-7)));
    }
    bool operator!=(const Point& p) {
        return ((x != p.x) && (fabs(y - p.y) < 1e-7));
    }
    Point min(const Point& A, const Point& B) {
        if (A.y < B.y) {
            return A;
        } else {
            if ((fabs(A.y - B.y) < 1e-7) && (A.x < B.x))
                return A;
        }
        return B;
    }
    ~Point() { x = 0; y = 0; }
};

class Vector {
 public:
    double X;
    double Y;
    Vector() { X = 0; Y = 0; }
    Vector(Point A, Point B) { X = B.x - A.x; Y = B.y - A.y; }
    Vector(double X_, double Y_) { X = X_; Y = Y_; }
    ~Vector() { X = Y = 0; }
};


double modul(const Vector& Vec);

double skalyar(const Vector& Vec1, const Vector& Vec2);

double cos(const Vector& A, const Vector& B);

bool cw(double x1, double y1, double x2, double y2, double x3, double y3);

std::vector<Point> GrahamPar(const std::vector<Point>& Points);

std::vector<Point> GrahamSeq(const std::vector<Point>& Points);

Point SearchMinPoint(const std::vector<Point>& Points);

std::vector<Point> Sort(const std::vector<Point>& Points);

std::vector<Point> GenPoints(int n);

#endif  // MODULES_TASK_3_KHISMATULINA_K_GRAHAM_GRAHAM_H_
