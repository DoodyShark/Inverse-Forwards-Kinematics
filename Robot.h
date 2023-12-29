#pragma once
// Including custom classes
#include "DhiyaaGraphics.h"
#define ALLOWED 100

using namespace std;

// The Robot class links segments together and handles their interactions
class Robot {
private:
	Segment* rootSegment; // Pointer to the root segment of the robot
	int numSegments; // Used to keep track of the number of segments in a given robot
public:
	// Default constructor: Creates an empty robot at the origin
	Robot() {
		rootSegment = nullptr;
		numSegments = 0;
	}

	// Non-Default constructor: Creates a robot with a segment of specified length at the origin
	Robot(double length) {
		rootSegment = new Segment(Point(), 0, length);
		numSegments = 1;
	}

	// Non-Default constructor: Creates a robot with a segment of specified length at the specified point
	Robot(double length, Point root) {
		rootSegment = new Segment(root, 0, length);
		numSegments = 1;
	}

	// Getter Functions:
	Segment* getRootSegment() {
		return rootSegment;
	}

	int getNumSegments() {
		return numSegments;
	}

	// Functions to go through the linked list of segments
	Segment* getSegment(int index) {
		if (index >= numSegments) {
			cout << "ERROR: There is no segment of this index" << endl;
			return nullptr;
		}
		Segment* currentSegment = rootSegment;
		for (int i = 0; i < index; i++) {
			currentSegment = currentSegment->getChild();
		}
		return currentSegment;
	}

	// Function that handles the addition of segments at the end of the robot
	void addSegment(int length) {
		numSegments++;
		// If the robot is empty, create the root segment and set its adress to rootSegment then exit the function
		if (rootSegment == nullptr) {
			rootSegment = new Segment(Point(), 0, length);
			cout << "Successfully added a segment" << endl;
			return;
		}
		// Go to the end of the linked list (last segment), then create a child for the last segment using the user specified length
		Segment* currentSegment = rootSegment;
		while (currentSegment->getChild() != nullptr) {
			currentSegment = currentSegment->getChild();
		}
		currentSegment->setChild(new Segment(currentSegment->getEnd(), currentSegment->getAngle(), length));
		cout << "Successfully added a segment" << endl; // Confirmation message
	}

	// Useful for changing the length of a segment anywhere in the chain
	void setSegment(double length, double angle, int index) {
		// You can't set a segment that doesn't exist 
		if (index > numSegments) {
			cout << "There is no segment stored at that index" << endl;
			return;
		}
		// If the index is numSegments, create a new segment at the end;
		else if (index == numSegments) {
			addSegment(length);
			return;
		}
		Segment* previousSegment = nullptr; // Used to edit the child of the previous segment
		Segment* currentSegment = rootSegment; // Used to edit the segment itself
		for (int i = 0; i < index; i++) {
			previousSegment = currentSegment; // Previous segment always lags 1 segment behind current segment
			currentSegment = currentSegment->getChild();
		}
		// Temporarilt holding all the old data of a segment that is relevant when it is deleted
		Segment* nextSegment = currentSegment->getChild();
		Point* oldStart = new Point(currentSegment->getStart().getX(), currentSegment->getStart().getY());
		// deleting the segment
		delete currentSegment;
		// Creating the robot
		currentSegment = new Segment(*oldStart, angle, length);
		// Linking the robot to the rest of the linked list
		if (previousSegment != nullptr) // This condition would be false if index == 0
			previousSegment->setChild(currentSegment);
		if (nextSegment != nullptr)
			currentSegment->setChild(nextSegment);
		// Correct for all the next segments
		for (int i = index + 1; i < numSegments; i++) {
			getSegment(i)->moveStart(getSegment(i - 1)->getEnd());
		}
	}

	// The moveRoot function moves the starting position of the robot to the specified new position while making sure the segments stay connected
	void moveRoot(Point newPos) {
		rootSegment->moveStart(newPos);
		// Propagating the change throughout the chain to maintain connectedness
		for (int i = 1; i < numSegments; i++) {
			getSegment(i)->moveStart(getSegment(i-1)->getEnd());
		}
	}
	
	// The following function removes a segment at the end effector if the robot is not empty
	void removeLastSegment() {
		if (numSegments != 0) {
			numSegments--;
			Segment* previousSegment= nullptr; // Keeping track of the previous segment
			Segment* currentSegment = rootSegment;
			while (currentSegment->getChild() != nullptr) {
				previousSegment = currentSegment; // previousSegment always lags behind currentSegment by 1
				currentSegment = currentSegment->getChild();
			}
			if (previousSegment != nullptr)
				previousSegment->setChild(nullptr);
			delete currentSegment;
			if (numSegments == 0)
				rootSegment = nullptr;
			cout << "Successfully removed the last segment" << endl;
		}
		else {
			cerr << "ERROR: You are trying to remove a segment from an empty robot . . ." << endl;
		}
	}
	// The reset robot rotates everything back to 0 angles relative to the x-axis
	void resetRobot() {
		if (numSegments == 0) {
			cout << "Empty Robot: No actions performed . . ." << endl;
		}
		else {
			// Rotates by the opposite of its current angle to reset to 0
			getSegment(0)->RotateBy(-getSegment(0)->getAngle());
			for (int i = 1; i < numSegments; i++) {
				getSegment(i)->moveStart(getSegment(i - 1)->getEnd()); // Make sure the start of one segment is the endpoint of the first
				getSegment(i)->RotateBy(-getSegment(i)->getAngle());
			}
		}
	}
	// The forwardsKinematics function rotates the segments of a robot by a set of angles relative to the previous segments and returns the position of the end effector
	Point forwardsKinematics(double angles[], int size) {
		// To handle the cases where there are more angles than segments
		if (numSegments < size) {
			cout << "Last " << (size - numSegments) << " angles ignored." << endl;
			size = numSegments;
		}
		// Rotates each segment and the ones after it by a given angle. That is because the angles given to this function are relative the segment before it
		for (int i = 0; i < size; i++) {
			getSegment(i)->RotateBy(angles[i]);
			for (int j = i + 1; j < numSegments; j++) {
				getSegment(j)->moveStart(getSegment(j - 1)->getEnd()); // Make sure the start of one segment is the endpoint of the first
				getSegment(j)->RotateBy(angles[i]);
			}
		}
		// Returns the end point of the last segment
		return getSegment(numSegments - 1)->getEnd();
	}

	// Used in the inverseKinematics function. Moves a segment to a certain target
	void makeSegmentFollow(Segment* segment, Point* target) {
		double theta = atan2(target->getY() - segment->getStart().getY(), target->getX() - segment->getStart().getX());
		segment->setAngle(theta);
		segment->setEnd(*target);
	}

	// Inverse kinematics function to move the end effector to a target point if possible
	void inverseKinematics(Point* endEffector) {
		double maxLength = 0;
		int allowedIterations = 0;
		for (int i = 0; i < numSegments; i++) {
			maxLength += getSegment(i)->getLength();
		}
		if (Point().distanceFrom(endEffector) > maxLength) {
			return;
		}
		Point rootPosition(rootSegment->getStart().getX(), rootSegment->getStart().getY());
		do {
			Point* target = new Point(endEffector->getX(), endEffector->getY());
			allowedIterations++;
			//double maxLength = max;
			moveRoot(rootPosition);
			for (int i = numSegments - 1; i >= 0; i--) {
				makeSegmentFollow(getSegment(i), target);
				*target = getSegment(i)->getStart();
			}
			delete target;
		} while (!(rootSegment->getStart().isEqualTo(&rootPosition)) && allowedIterations < ALLOWED);
		moveRoot(rootPosition);

	}

	// The printRobotInfo function prints the information of all the segments contained in the robot. It makes use of the printSegmentInfo function
	void printRobotInfo(bool mode) {
		for (int i = 0; i < numSegments; i++) {
			cout << "Segment " << (i + 1) << ": ";
			getSegment(i)->printSegmentInfo(mode);
			cout << endl;
		}
	}

	// The printRobot function makes use of the DhiyaaGraphics class to draw the robot onto the output screen in a given resolution. It also prints the last robot into a file called "LastGraph.txt"
	void printRobot(int resolution) {
		// Only draws the robot if it isn't empty
		if (numSegments > 0) {
			DhiyaaGraphics artist(resolution); // Creates a canvas to draw onto with specified resolution
			artist.clearCanvas(); // Clear the canvas before anything is drawn onto it
			artist.addRobotToCanvas(rootSegment, numSegments); // Adds the robot to the canvas
			artist.printToFile(); // Prints the canvas on the output screen and a file called "LastGraph.txt"
			artist.clearCanvas(); // Clears the canvas again (not a very necessary step as the DhiyaaGraphics object is going to be destructed)
		}
		// If the robot is empty tell the user there's nothing to draw
		else {
			cout << "Nothing to draw . . .\nThe robot is empty . . ." << endl;
		}
	}

	// Deallocating the dynamic memory used for all the segments in the chain
	~Robot() {
		for (int i = numSegments-1; i >= 0; i--) { // Deleting in reverse because the only way to access a segment is through the segment before it
			delete getSegment(i);
		}
	}
};