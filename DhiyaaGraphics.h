#pragma once
#include <fstream>
#include "Segment.h"
#define DEFAULT 50
#define RESOLUTION 100
#define PI 3.14159265359

using namespace std;

class DhiyaaGraphics {
private:
	// To navigate through a robot
	Segment* rootSegment;
	int numSegments;
	// Width of the canvas
	int s;
	// A boolean matrix representing pixels to draw into
	bool** canvas;
	// Points used to keep track of scaling of the axes
	Point topCorner;
	Point bottomCorner;
public:
	// Default Constructor: Creates an empty canvas of size 2 * DEFAULT
	DhiyaaGraphics() {
		s = 2 * DEFAULT;
		canvas = new bool* [s];
		for (int r = 0; r < s; r++) {
			canvas[r] = new bool[s];
		}
		topCorner = bottomCorner = Point();
		rootSegment = nullptr;
		numSegments = 0;
		clearCanvas();
	}
	// Non-Default Constructor: Creates an empty canvas of size 2 * width
	DhiyaaGraphics(int width) {
		s = 2 * width;
		canvas = new bool* [s];
		for (int r = 0; r < s; r++) {
			canvas[r] = new bool[s];
		}
		topCorner = bottomCorner = Point();
		rootSegment = nullptr;
		numSegments = 0;
		clearCanvas();
	}
	// This getSEgment class is the exact same as the one in the Robot class. The reason it was put here was to avoid circular inclusion
	Segment* getSegment(int index) {
		if (index >= numSegments) {
			cout << "ERROR: there is no segment of this index" << endl;
			return nullptr;
		}
		Segment* currentSegment = rootSegment;
		for (int i = 0; i < index; i++) {
			currentSegment = currentSegment->getChild();
		}
		return currentSegment;
	}
	// SetUpEdges finds the point farthest away from the origin and sets up the canvas's scale such that it can fit the robot
	void setUpEdges() {
		double maxAbs = getSegment(0)->getEnd().getX();
		// Fincing the macimum distance from the origin
		for (int i = 0; i < numSegments; i++) {
			if (abs(getSegment(i)->getEnd().getX()) > maxAbs) {
				maxAbs = abs(getSegment(i)->getEnd().getX());
			}
			if (abs(getSegment(i)->getStart().getX()) > maxAbs) {
				maxAbs = abs(getSegment(i)->getStart().getX());
			}
			if (abs(getSegment(i)->getEnd().getY()) > maxAbs) {
				maxAbs = abs(getSegment(i)->getEnd().getY());
			}
			if (abs(getSegment(i)->getStart().getY()) > maxAbs) {
				maxAbs = abs(getSegment(i)->getStart().getY());
			}
		}
		// Adding padding for beauty
		double padding = maxAbs / 5;
		maxAbs += padding;
		topCorner = Point(maxAbs, maxAbs);
		bottomCorner = Point(-maxAbs, -maxAbs);
	}

	// This dunction receives the root segment of a robot and the number of segments in the robot to add it to the 
	void addRobotToCanvas(Segment* root, int size) {
		rootSegment = root;
		numSegments = size;
		setUpEdges(); // Setting up the scale
		for (int i = 0; i < numSegments; i++) {
			double yPrev = getSegment(i)->getStart().getY();
			double stepX = (getSegment(i)->getEnd().getX() - getSegment(i)->getStart().getX()) / (RESOLUTION); // The step size in x as we move through the segment
			// For positive stepX
			if (stepX == 0) // Any 90 degree angle will be slightly skewed for tan(angle) to be defined
				stepX = 0.000000000000000001;
			if (stepX > 0) {
				for (double j = getSegment(i)->getStart().getX(); j < getSegment(i)->getEnd().getX(); j += stepX) {
					double yCurr;
					yCurr = stepX * tan(getSegment(i)->getAngle()) + yPrev; // Basic trigonometry
					yPrev = yCurr;
					canvas[(int)((double)s / 2 - (yCurr / topCorner.getY()) * (s / 2))][(int)((double)s / 2 + (j / topCorner.getX()) * (s / 2))] = true; // Filling uo the correspoding point in the canvas
				}
			}
			// For negative stepX
			else {
				for (double j = 0; j < getSegment(i)->getStart().getX() - getSegment(i)->getEnd().getX(); j -= stepX) {
					double yCurr;
					yCurr = stepX * tan(getSegment(i)->getAngle()) + yPrev; // Basic trigonemety
					yPrev = yCurr;
					canvas[(int)((double)s / 2 - (yCurr / topCorner.getY()) * (s / 2))][(int)((double)s / 2 + ((getSegment(i)->getStart().getX() - j) / topCorner.getX()) * (s / 2))] = true; // Filling uo the correspoding point in the canvas
				}
			}
		}
	}

	// Sets all the booleans in the matrix as false (0)
	void clearCanvas() {
		for (int r = 0; r < s; r++) {
			for (int c = 0; c < s; c++) {
				canvas[r][c] = false;
			}
		}
	}

	// Draws axes and translates the boolean array into * _ and |
	void printToFile() {
		ofstream writer("LastGraph.txt", ios::out);
		if (writer.fail()) {
			cerr << "ERROR: Failed to open file . . ." << endl;
			exit(-1);
		}
		double step = topCorner.getY() / (s / 2);
		for (double x = -topCorner.getX(); x <= topCorner.getX(); x += step * (s / 10)) {
			writer << left << fixed << setprecision(1) << setw(s / 10) << x;
			cout << left << fixed << setprecision(1) << setw(s / 10) << x;
		}
		writer << endl;
		cout << endl;
		for (double x = -topCorner.getX(); x <= topCorner.getX(); x += step * (s / 10)) {
			writer << setw(s / 10) << '|';
			cout << setw(s / 10) << '|';
		}
		cout << endl;
		writer << endl;
		double y = topCorner.getY();
		for (int r = 0; r < s - 1; r += 2, y -= 2 * step) {
			for (int c = 0; c < s; c++) {
				if (canvas[r][c] && canvas[r + 1][c]) {
					writer << '|';
					cout << '|';
				}
				else if (canvas[r][c]) {
					writer << '*';
					cout << '*';
				}
				else if (canvas[r + 1][c]) {
					writer << '_';
					cout << '_';
				}
				else {
					writer << ' ';
					cout << ' ';
				}
			}
			if ((r / 2) % (s / 20) == 0) {
				writer << "  -- " << left << fixed << setprecision(1) << y;
				cout << "  -- " << left << fixed << setprecision(1) << y;
			}
			writer << endl;
			cout << endl;
		}
		writer.close();
	}

	// Destructor: Deallocates the dynamically allocated memory for the canvas
	~DhiyaaGraphics() {
		for (int i = 0; i < s; i++) {
			delete[] canvas[i];
		}
		delete[] canvas;
	}
};