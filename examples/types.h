
#pragma once

class Point2d
{
public:
    Point2d() : x(0), y(0) {}
    Point2d(int x, int y) : x(x), y(y) {}
    int x,y;
};

inline bool operator==(const Point2d& lhs, const Point2d& rhs)
{
    return lhs.x==rhs.x && lhs.y==rhs.y;
}

class Point3d
{
public:
    Point3d() : x(0), y(0), z(0) {}
    Point3d(int x, int y, int z) : x(x), y(y), z(z) {}
    int x,y,z;
};

inline bool operator==(const Point3d& lhs, const Point3d& rhs)
{
    return lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z;
}

class MiscData
{
public:
    MiscData() {}
    MiscData(Point2d p, Point3d q, int r, float s) : p(p), q(q), r(r), s(s) {}
    Point2d p;
    Point3d q;
    int r=42;
    float s=1.2f;
};

inline bool operator==(const MiscData& lhs, const MiscData& rhs)
{
    return lhs.p==rhs.p && lhs.q==rhs.q && lhs.r==rhs.r && lhs.s==rhs.s;
}
