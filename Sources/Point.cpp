#include "Point.h"
#include <cmath> // for sqrt, if needed

using namespace std;

// Default constructor, initializes (0,0)
Point::Point() : xCoordinate(0), yCoordinate(0) {}

// Single value constructor, sets both X and Y to the same value
Point::Point(int xy) : xCoordinate(xy), yCoordinate(xy) {}

// Constructor with separate X and Y
Point::Point(int x, int y) : xCoordinate(x), yCoordinate(y) {}

// Copy constructor
Point::Point(const Point& otherPoint) : xCoordinate(otherPoint.xCoordinate), yCoordinate(otherPoint.yCoordinate) {}

// Get X coordinate
int Point::X() const { return xCoordinate; }

// Get Y coordinate
int Point::Y() const { return yCoordinate; }

// Multiply both coordinates by a value
Point Point::operator*(const int& value) {
    return Point(xCoordinate * value, yCoordinate * value);
}

// Multiply both coordinates by a value and assign
Point& Point::operator*=(const int& value) {
    xCoordinate *= value;
    yCoordinate *= value;
    return *this;
}

// Multiply coordinates with another point (component-wise)
Point Point::operator*(const Point& otherPoint) {
    return Point(xCoordinate * otherPoint.xCoordinate, yCoordinate * otherPoint.yCoordinate);
}

// Add two points
Point Point::operator+(const Point& otherPoint) {
    return Point(xCoordinate + otherPoint.xCoordinate, yCoordinate + otherPoint.yCoordinate);
}

// Subtract two points
Point Point::operator-(const Point& otherPoint) {
    return Point(xCoordinate - otherPoint.xCoordinate, yCoordinate - otherPoint.yCoordinate);
}

// Add another point to this one
Point& Point::operator+=(const Point& otherPoint) {
    xCoordinate += otherPoint.xCoordinate;
    yCoordinate += otherPoint.yCoordinate;
    return *this;
}

// Subtract another point from this one
Point& Point::operator-=(const Point& otherPoint) {
    xCoordinate -= otherPoint.xCoordinate;
    yCoordinate -= otherPoint.yCoordinate;
    return *this;
}

// Check equality
bool Point::operator==(const Point& otherPoint) {
    return xCoordinate == otherPoint.xCoordinate && yCoordinate == otherPoint.yCoordinate;
}

// Check inequality
bool Point::operator!=(const Point& otherPoint) {
    return !(*this == otherPoint);
}

// Return squared distance between two points (avoids sqrt for efficiency)
long Point::sqrDistance(const Point& otherPoint) {
    long dx = xCoordinate - otherPoint.xCoordinate;
    long dy = yCoordinate - otherPoint.yCoordinate;
    return dx * dx + dy * dy;
}
