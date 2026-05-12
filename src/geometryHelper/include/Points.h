#pragma once

struct POINT
{
    long x;
    long y;
};

class CFPoint
{
public:
    double x;
    double y;

    inline CFPoint() : x(0), y(0) {};
    inline CFPoint(double X, double Y) : x(X), y(Y) {};
    inline CFPoint(int X, int Y) : x(X) , y(Y) {};
    inline CFPoint(long X, long Y) : x(static_cast<double>(X)) , y(static_cast<double>(Y)) {};
    inline CFPoint(POINT p) : x(p.x), y(p.y) {};

    inline operator POINT() const
    {
        POINT p;
        p.x = int(x);
        p.y = int(y);
        return p;
    }

    inline CFPoint &Offset(double xOffset, double yOffset)
    {
        x += xOffset;
        y += yOffset;
        return *this;
    }

    inline CFPoint &Offset(POINT point)
    {
        x += point.x;
        y += point.y;
        return *this;
    }

    inline bool operator==(POINT point) const
    {
        return (x == point.x && y == point.y);
    }

    inline bool operator!=(POINT point) const
    {
        return (x != point.x || y != point.y);
    }

    inline CFPoint &operator+=(CFPoint point)
    {
        x += point.x;
        y += point.y;
        return *this;
    }

    inline CFPoint &operator-=(CFPoint point)
    {
        x -= point.x;
        y -= point.y;
        return *this;
    }

    inline void SetPoint(double X, double Y)
    {
        x = X;
        y = Y;
    }

    inline CFPoint operator-() const
    {
        return CFPoint(-x, -y);
    }

    inline CFPoint operator+(CFPoint point) const
    {
        return CFPoint(x + point.x, y + point.y);
    }

    inline CFPoint operator-(CFPoint point) const
    {
        return CFPoint(x - point.x, y - point.y);
    }

    inline CFPoint operator*(double scale) const
    {
        return CFPoint(x * scale, y * scale);
    }

    inline CFPoint &operator*=(double scale)
    {
        x *= scale;
        y *= scale;
        return *this;
    }
};

class PointFloatGdiplus
{
public:
    PointFloatGdiplus()
    {
        X = Y = 0.0f;
    }

    PointFloatGdiplus(const PointFloatGdiplus &point)
    {
        X = point.X;
        Y = point.Y;
    }

    PointFloatGdiplus(float x,
                      float y)
    {
        X = x;
        Y = y;
    }

    PointFloatGdiplus operator+(const PointFloatGdiplus &point) const
    {
        return PointFloatGdiplus(X + point.X,
                                 Y + point.Y);
    }

    PointFloatGdiplus operator-(const PointFloatGdiplus &point) const
    {
        return PointFloatGdiplus(X - point.X,
                                 Y - point.Y);
    }

    bool Equals(const PointFloatGdiplus &point)
    {
        return (X == point.X) && (Y == point.Y);
    }

public:
    float X;
    float Y;
};

using PointFloat = PointFloatGdiplus;
using PointDouble = CFPoint;
using PointInteger = POINT;