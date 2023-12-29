#pragma once
// Including custom classes
#include "Point.h"
#define PI 3.14159265359 // Value of pi to 11 decimal places. Used to convert from degrees to radians and vice verca

using namespace std;

// The Segment class handles segment information where each segment is desribed by a start point, a length, an angle above the x-axis, and an endpoint
// Segments from the Segment class will be used as building blocks to assememble robots
class Segment {
private:
	Point startPoint;
	Point endPoint;
	double angle;
	double length;
	// Reference to a child segment. If there is no child, it is set nullptr.
	Segment* child;
public:
	// Default Constructor: assigns the safe default values for the segment's instance variables
	Segment() {
		startPoint = Point();
		angle = 0;
		length = 1;
		endPoint = calculateEndPoint();
		child = nullptr;
	}
	// Non-Default Constructor: Creates a segment using the provided values for the start point, length, and angle
	Segment(Point start, double theta, double l) {
		startPoint = start;
		angle = theta;
		// Length can't be less than or equal to 0; therefore, the program sets the value to the safe default value of 1
		if (l <= 0) {
			cerr << "ERROR: Length of segment can't be non-positive. Segment length set to default of 1" << endl;
			l = 1;
		}
		length = l;
		endPoint = calculateEndPoint(); // The user isn't given the option to set the end point since it is calculated using the other variables
		child = nullptr;
	}

	// Modified Setter Functions:
	void setChild(Segment* childReference) {
		child = childReference;
	}

	void setEnd(Point end) {
		this->endPoint = end;
		this->startPoint = calculateStartPoint(); // Any time the start point is changed, the endpoint must be updated accordingly
	}

	void moveStart(Point start) {
		this->startPoint = start;
		this->endPoint = calculateEndPoint(); // Any time the start point is changed, the endpoint must be updated accordingly
	}

	void changeLength(double l) {
		this->length = l;
		this->endPoint = calculateEndPoint(); // Any time the length is changed, the endpoint must be updated accordingly
	}

	void setAngle(double theta) {
		this->angle = theta;
		this->endPoint = calculateEndPoint(); // Any time the angle is changed, the endpoint must be updated accordingly
	}

	void RotateBy(double theta) {
		this->angle += theta;
		this->endPoint = calculateEndPoint(); // Any time the angle is changed, the endpoint must be updated accordingly
	}

	// Getter Functions:
	Segment* getChild() {
		return child;
	}

	Point getStart() {
		return this->startPoint;
	}

	Point getEnd() {
		return this->endPoint;
	}

	double getAngle() {
		return this->angle;
	}

	double getLength() {
		return this->length;
	}

	// The calculateEndPoint function uses basic trigonometry to find the x and y coordinates of the endpoint
	Point calculateEndPoint() {
		return Point(startPoint.getX() + (length * cos(angle)), startPoint.getY() + (length * sin(angle)));
	}
	// The calculateStartPoint function uses basic trigonometry to find the x and y coordinates of the startpoint
	Point calculateStartPoint() {
		return Point(endPoint.getX() - (length * cos(angle)), endPoint.getY() - (length * sin(angle)));
	}

	// The printSegmentInfo function prints the data related to segment. It makes use of the printPointInfo function when displaying the start and end points of the segment. 
	// The current units mode is also taken into account when displaying angles
	void printSegmentInfo(bool mode) {
		cout << "Start: ";
		startPoint.printPointInfo();
		cout << "; End: ";
		endPoint.printPointInfo();
		cout << "; Length: " << this->length << "; Angle above x-axis: ";
		// The below code displays the angle in the current angle mode
		if (mode) {
			cout << this->angle << " rad";
		}
		else {
			cout << this->angle * (180 / PI) << " deg";
		}
	}
};