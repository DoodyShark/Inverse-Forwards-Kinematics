#pragma once
#include <iostream>
#include <iomanip>
#include <cmath>
#define DELTA 0.000001

using namespace std;

// The Point class handles coordinate data and is used to create the start and endpoints of each segment
class Point {
private:
	double x;
	double y;
public:
	// Default constructor: Any non-initialized point is placed at the origin
	Point() {
		x = 0;
		y = 0;
	}
	// Non-default constructor: Creates a point at the specified coordinates
	Point(double xPos, double yPos) {
		x = xPos;
		y = yPos;
	}
	// Setter functions:
	void setX(double xPos) {
		this->x = xPos;
	}
	void setY(double yPos) {
		this->y = yPos;
	}
	// Getter functions:
	double getX() {
		return this->x;
	}
	double getY() {
		return this->y;
	}
	// The printPointInfo funtion prints the x and y coordinates in the format (x,y)
	void printPointInfo() {
		cout << '(' << setprecision(3) << fixed << x << ',' << setprecision(3) << fixed << y << ')';
	}
	// Finds the euclidean distance between two pointsusing the pythagorean theorem
	double distanceFrom(Point* other) {
		return sqrt(pow(x - other->x, 2) + pow(y - other->y, 2));
	}
	// Checks if two points are the same within some margin of error due to the uncertainty of the point data
	bool isEqualTo(Point* other) {
		bool xIsEqual = false;
		bool yIsEqual = false;
		if (abs(x - other->x) <= DELTA) {
			xIsEqual = true;
		}
		if (abs(y - other->y) <= DELTA) {
			yIsEqual = true;
		}
		return yIsEqual && xIsEqual;
	}
};