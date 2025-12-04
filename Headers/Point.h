#pragma once

// Represents a point on the console grid
class Point
{
private:
    int xCoordinate; // X-axis
    int yCoordinate; // Y-axis

public:
    // Constructors
    Point() : xCoordinate(0), yCoordinate(0) {}
    Point(int xy) : xCoordinate(xy), yCoordinate(xy) {}
    Point(int x, int y) : xCoordinate(x), yCoordinate(y) {}
    Point(const Point& otherPoint) : xCoordinate(otherPoint.xCoordinate), yCoordinate(otherPoint.yCoordinate) {}

    // Accessors
    int X() const { return xCoordinate; }
    int Y() const { return yCoordinate; }

    // Arithmetic operators
    Point operator*(const int& value) { return Point(xCoordinate * value, yCoordinate * value); }
    Point& operator*=(const int& value) { xCoordinate *= value; yCoordinate *= value; return *this; }
    Point operator*(const Point& other) { return Point(xCoordinate * other.xCoordinate, yCoordinate * other.yCoordinate); }
    Point operator+(const Point& other) { return Point(xCoordinate + other.xCoordinate, yCoordinate + other.yCoordinate); }
    Point operator-(const Point& other) { return Point(xCoordinate - other.xCoordinate, yCoordinate - other.yCoordinate); }
    Point& operator+=(const Point& other) { xCoordinate += other.xCoordinate; yCoordinate += other.yCoordinate; return *this; }
    Point& operator-=(const Point& other) { xCoordinate -= other.xCoordinate; yCoordinate -= other.yCoordinate; return *this; }

    // Comparison
    bool operator==(const Point& other) { return xCoordinate == other.xCoordinate && yCoordinate == other.yCoordinate; }
    bool operator!=(const Point& other) { return !(*this == other); }

    // Calculate squared distance
    long sqrDistance(const Point& other) { 
        long dx = xCoordinate - other.xCoordinate;
        long dy = yCoordinate - other.yCoordinate;
        return dx * dx + dy * dy;
    }
};
