//***********************************************************//
//** Name: Dhiyaa Al Jorf                                  **//
//** Net ID: da2863                                        **//
//** Date Created: November 27, 2021                       **//
//** Assignment 4: Electrical Engineering Case Study:      **//
//** Robot Kinematics                                      **//
//** Program: Assignment3.cpp                              **//
//***********************************************************//

#include <cctype> // Used to allow user to enter both y or Y to say yes
// Including all the custom classes
#include "Robot.h"
#define SMALLSCREEN 50 // A good resolution for printing on the default size of the output window
#define FULLSCREEN 90 // A good resolution for printing on the maximized output window
#define PI 3.14159265359 // Value of pi to 11 decimal places. Used to convert from degrees to radians and vice verca

using namespace std;

void options(bool& loop, Robot**& robots, int& size, bool& mode);
void createNewRobot(Robot**& robots, int& size);
void editExistingRobot(Robot**& robots, int& size);
void moveAndDisplayRobot(Robot**& robots, int& size, bool mode);
void changeAngleUnits(bool& mode);

int main() {
	bool mode = true; // Default angle unit mode is 1 = radians
	int size = 0; // Represents the number of robots created
	Robot** robots = new Robot * [size]; // Dynamic array of robot pointers 
	bool loop = true; // loop condition. true until user decides to exit program
	cout << "Welcome to the robot factory . . ." << endl;
	do {
		options(loop, robots, size, mode);
	} while (loop);
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Thanks for using our services . . . Goodbye . . ." << endl;
	// Deallocating all the memory allocated for the robots
	for (int i = 0; i < size; i++) {
		delete robots[i];
	}
	delete[] robots;
}

// MAIM MENU: This function displays the main menu and hadnles to which menu the user should go according to their choice
void options(bool& loop, Robot**& robots, int& size, bool& mode) {
	char choice;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "MAIN MENU:" << endl;
	cout << "Enter number to select option:" << endl;
	cout << "1 - Create new robot" << endl; 
	cout << "2 - Edit existing robot (Add, remove, or change length of segments)" << endl;
	cout << "3 - Manipulate positions and angles and/or display robot information" << endl;
	// Angle units mode toggler
	cout << "4 - Change angle units. Current mode: ";
	if (mode) {
		cout << "Radians" << endl;
	}
	else {
		cout << "Degrees" << endl;
	}
	cout << "5 - Exit program" << endl; // Set the condition for the do while in the main function to false such that execution ends
	cin >> choice;
	// Switch case block to handle user input
	switch (choice) {
	case '1':
		createNewRobot(robots, size);
		break;
	// Option 2 and 3 are only available after at least one robot has been created
	case '2':
		if (size > 0)
			editExistingRobot(robots, size);
		else {
			cout << "Sorry, you haven't created any robots yet.\nPlease choose option 1 before editing your robot." << endl;
		}
		break;
	case '3':
		if (size > 0) {
			moveAndDisplayRobot(robots, size, mode);
		}
		else {
			cout << "Sorry, you haven't created any robots yet.\nPlease choose option 1 before manipulating/displaying your robot." << endl;
		}
		break;
	case '4':
		changeAngleUnits(mode);
		break;
	case '5':
		loop = false;
		break;
	default:
		cout << "Invalid selection . . ." << endl;
		break;
	}
}

// ROBOT CREATION MENU: This function allows the user to create custom robots using the constructors of the Robot class
void createNewRobot(Robot**& robots, int& size) {
	bool loop = true;
	char choice;
	double length, x, y;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "ROBOT CREATION MENU:" << endl;
	while (loop) {
		cout << "Enter selection: " << endl;
		cout << "1 - Empty robot starting at the origin" << endl; // Default robot constructor
		cout << "2 - Create robot starting at the origin with 1 segment" << endl; // Non-default robot constructor that asks for length only
		cout << "3 - Create robot at custom point with 1 segment" << endl; // Non-default robot constructor that asks for length and root position
		cout << "4 - Back to main menu" << endl; // Back to main menu
		cin >> choice;
		// Switch case block to handle user input
		switch (choice) {
		case '1': {
			// Expanding the robots array to have space for the robot we want to create
			size++;
			Robot** temp = new Robot * [size];
			for (int i = 0; i < size - 1; i++) {
				temp[i] = robots[i];
			}
			delete[] robots;
			robots = temp;
			robots[size - 1] = new Robot(); // Create the robot with the default constructor
			cout << "Robot " << size << " created." << endl;
			break;
		}
		case '2': {
			// Expanding the robots array to have space for the robot we want to create
			size++;
			Robot** temp = new Robot * [size];
			for (int i = 0; i < size - 1; i++) {
				temp[i] = robots[i];
			}
			delete[] robots;
			robots = temp;
			cout << "Enter length of segment: ";
			cin >> length;
			robots[size - 1] = new Robot(length); // Use the non-default constructor that takes in the length only to create the new robot
			cout << "Robot " << size << " with a segment of length " << length << " created" << endl;
			break;
		}
		case '3': {
			// Expanding the robots array to have space for the robot we want to create
			size++;
			Robot** temp = new Robot * [size];
			for (int i = 0; i < size - 1; i++) {
				temp[i] = robots[i];
			}
			delete[] robots;
			robots = temp;
			cout << "Enter length of segment: ";
			cin >> length;
			cout << "Enter x position of root: ";
			cin >> x;
			cout << "Enter y position of root: ";
			cin >> y;
			robots[size - 1] = new Robot(length, Point(x, y)); // Use the non-default constructor that takes in the length and starting position to create the new robot
			cout << "Robot " << size << " with a segment of length " << length << " created at ";
			Point(x, y).printPointInfo();
			cout << endl;
			break;
		}
		case '4':
			loop = false; // To exit this submenu
			break;
		default:
			cout << "Invalid selection . . ." << endl;
			break;
		}
	}
}

// This function allows the user between angle units to give the user the flexibility in data input
void changeAngleUnits(bool& mode) {
	char choice;
	cout << "Current mode: ";
	if (mode) {
		cout << "Radians" << endl;
		cout << "Would you like to switch to degrees? (Y/N)" << endl;
	}
	else {
		cout << "Degrees" << endl;
		cout << "Would you like to switch to radians? (Y/N)" << endl;
	}
	cin >> choice;
	if (tolower(choice) == 'y')
		mode = !mode;
}

// ROBOT EDITING MENU: This function allows the user to add and remove segments from their robots
void editExistingRobot(Robot**& robots, int& size) {
	bool loop = true;
	int robNumber, segNumber;
	char choice;
	double length;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "ROBOT EDITING MENU:" << endl;
	// Displays the available robots
	cout << "Which robot would you like to edit? (Max robot ID " << size << "): " << endl;
	cin >> robNumber;
	if (robNumber > size) {
		cout << "Invalid selection . . ." << endl;
	}
	while (loop) {
		cout << "What would you like to do?" << endl;
		cout << "1 - Add segment to the end" << endl; // addSegment() function
		if (robots[robNumber - 1]->getNumSegments() == 0) {
			cout << "(This robot is empty. Adding a segment to it automatically adds it at the origin)" << endl; // Notifies the user that an empty robot always sets the start point at the origin
		}
		cout << "2 - Remove segment from the end" << endl; // removeLastSegment() function
		cout << "3 - Edit segment length" << endl; // Makes use of the setSegment() function to edit the length of a particular segment
		cout << "4 - Back to main menu" << endl;
		cin >> choice;
		// Switch-case block to handle user input
		switch (choice) {
		case '1':
			cout << "Enter length of segment to create: ";
			cin >> length;
			robots[robNumber - 1]->addSegment(length);
			break;
		case '2':
			robots[robNumber - 1]->removeLastSegment();
			break;
		case '3':
			if (robots[robNumber - 1]->getNumSegments() != 0) {
				while (loop) {
					cout << "Which segment would you like to change the length of? (Max Segment ID " << robots[robNumber - 1]->getNumSegments() << ")" << endl;
					cin >> segNumber;
					if (segNumber > robots[robNumber - 1]->getNumSegments()) {
						cout << "Invalid selection" << endl;
					}
					else {
						loop = false;
					}
				}
				cout << "Enter the new length of the segment: ";
				cin >> length;
				robots[robNumber - 1]->setSegment(length, robots[robNumber - 1]->getSegment(segNumber - 1)->getAngle(), (segNumber - 1));
				loop = true;
			}
			else {
				cout << "This robot is empty. There are no segments to modify" << endl;
			}
			break;
		case '4':
			loop = false; // Leave the submenu
			break;
		default:
			cout << "Invalid selection . . ." << endl;
			break;
		}
	}
}

// MANIPULATION AND DISPLAY MENU: This function allows the user to perform forward kinematics on the robot, move the robot's root position, perform inverse kinematics, display the robot's information, and/or graphically display the robot
void moveAndDisplayRobot(Robot**& robots, int& size, bool mode) {
	bool loop = true;
	int robNumber, segNumber;
	char choice;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "MANIPULATION AND DISPLAY MENU:" << endl;
	cout << "Which robot would you like to work with? (Max robot ID " << size << "): " << endl;
	cin >> robNumber;
	if (robNumber > size) {
		cout << "Invalid selection . . ." << endl;
		return;
	}
	if (robots[robNumber - 1]->getNumSegments() == 0) {
		cout << "This robot is empty, there's nothing you can manipulate or display in it . . ." << endl;
		return;
	}
	cout << "What would you like to do with robot " << robNumber << "?" << endl;
	cout << "1 - Manipulate current postition" << endl;
	cout << "2 - Display robot info" << endl;
	cin >> choice;
	if (choice != '1' && choice != '2') {
		cout << "Invalid selection . . ." << endl;
		return;
	}
	if (choice == '1') {
		double* angles = new double[robots[robNumber - 1]->getNumSegments()];
		for (int i = 0; i < robots[robNumber - 1]->getNumSegments(); i++) {
			angles[i] = 0;
		}
		cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;
		cout << "ROBOT MANIPULATION SUB-MENU:" << endl;
		cout << "What would you like to do?" << endl;  
		cout << "1 - Move root segment" << endl; // Uses the moveRoot() function
		cout << "2 - Rotate single segment BY an angle (angles measured from previous segment)" << endl; // Makes use of the forwardKinematics function
		cout << "3 - Rotate single segment TO an angle (angles measured from previous segment)" << endl; // Makes use of the forwardKinematics function
		cout << "4 - Rotate multiple segments BY set of angles (angles measured from previous segment)" << endl; // Makes use of the forwardKinematics function
		cout << "5 - Rotate multiple segments TO set of angles (angles measured from previous segment)" << endl; // Makes use of the forwardKinematics function
		cout << "6 - Move end effector to a point (If possible)" << endl; // Uses the inverseKinematics() function
		cout << "7 - Reset robot to default position" << endl; // Uses the resetPosition() function
		cin >> choice;
		// Switch-case block to handle user selection
		switch (choice) {
		case '1': {
			double x = 0, y = 0;
			cout << "Enter x coordinate of new root position: ";
			cin >> x;
			cout << "Enter y coordinate of new root position: ";
			cin >> y;
			robots[robNumber - 1]->moveRoot(Point(x, y));
			break;
		}
		case '2':
			while (loop) {
				cout << "Which segment would you like to edit? (Max Segment ID " << robots[robNumber - 1]->getNumSegments() << ")" << endl;
				cin >> segNumber;
				if (segNumber > robots[robNumber - 1]->getNumSegments()) {
					cout << "Invalid selection";
				}
				else {
					loop = false;
				}
			}
			cout << "Enter angle to rotate the segment BY - Current Mode (";
			if (mode) {
				cout << "Radians):" << endl;
			}
			else {
				cout << "Degrees):" << endl;
			}
			cin >> angles[segNumber - 1];
			if (!mode) {
				angles[segNumber - 1] *= (PI / 180); // Converts user input to radians if the program is in degree mode
			}
			robots[robNumber - 1]->forwardsKinematics(angles, segNumber); // No need to alter the angles array as the forward kinematics already rotates BY an angle
			break;
		case '3':
			while (loop) {
				cout << "Which segment would you like to edit? (Max Segment ID " << robots[robNumber - 1]->getNumSegments() << ")" << endl;
				cin >> segNumber;
				if (segNumber > robots[robNumber - 1]->getNumSegments()) {
					cout << "Invalid selection";
				}
				else {
					loop = false;
				}
			}
			cout << "Enter angle to rotate the segment TO: - Current Mode (";
			if (mode) {
				cout << "Radians):" << endl;
			}
			else {
				cout << "Degrees):" << endl;
			}
			cin >> angles[segNumber - 1];
			if (!mode) {
				angles[segNumber - 1] *= (PI / 180); // Converts user input to radians if the program is in degree mode
			}
			angles[segNumber - 1] -= robots[robNumber - 1]->getSegment(segNumber - 1)->getAngle(); // Calculates by how much each segment has to rotate to reach the specified angle
			if (segNumber - 1 > 0) {
				angles[segNumber - 1] += robots[robNumber - 1]->getSegment(segNumber - 2)->getAngle();
			}
			robots[robNumber - 1]->forwardsKinematics(angles, segNumber); // After altering the angles array, the robot will now move TO the specified angles
			break;
		case '4': {
			int i = 0;
			for (i = 0; i < robots[robNumber - 1]->getNumSegments(); i++) {
				cout << "Enter -999 to stop entering angles (remaining angles will be left unchanged)" << endl;
				cout << "Rotate segment " << (i + 1) << " BY - Current Mode (";
				if (mode) {
					cout << "Radians):" << endl;
				}
				else {
					cout << "Degrees):" << endl;
				}
				cin >> angles[i];
				if (angles[i] == -999) {
					angles[i] = 0;
					break;
				}
				if (!mode) {
					angles[i] *= (PI / 180); // Converts user input to radians if the program is in degree mode
				}
			}
			robots[robNumber - 1]->forwardsKinematics(angles, i); // No need to alter the angles array as the forward kinematics already rotates BY an angle
			break;
		}
		case '5': {
			int i = 0;
			for (i = 0; i < robots[robNumber - 1]->getNumSegments(); i++) {
				cout << "Enter -999 to stop entering angles (remaining angles will be left unchanged)" << endl;
				cout << "Rotate segment " << (i + 1) << " TO - Current Mode (";
				if (mode) {
					cout << "Radians):" << endl;
				}
				else {
					cout << "Degrees):" << endl;
				}
				cin >> angles[i];
				if (angles[i] == -999) {
					angles[i] = 0;
					break;
				}
				if (!mode) {
					angles[i] *= (PI / 180); // Converts user input to radians if the program is in degree mode
				}
			}
			robots[robNumber - 1]->resetRobot(); // Reseting the robot makes any rotations BY the same as rotations TO
			robots[robNumber - 1]->forwardsKinematics(angles, i);
			break;
		}
		// Performs inverse kinematics
		case '6':
		{
			double x, y;
			cout << "Enter x coordinate of desired point: " << endl;
			cin >> x;
			cout << "Enter y coordinate of desired point: " << endl;
			cin >> y;
			Point target(x, y);
			robots[robNumber - 1]->inverseKinematics(&target);
			bool success = robots[robNumber - 1]->getSegment(robots[robNumber - 1]->getNumSegments() - 1)->getEnd().isEqualTo(&target);
			// If the robot succeeds at reaching the specified point, print confirmation message. Otherwise, reset to default and notify the user that it's impossible
			if (success) {
				cout << "Succeeded at reaching the point" << endl;
			}
			else {
				robots[robNumber - 1]->resetRobot();
				cout << "Failed to reach the point . . . \nRobot returned to default position. . ." << endl;
			}
			break;
		}
		case '7':
			robots[robNumber - 1]->resetRobot();
			break;
		default:
			cout << "Invalid selection . . ." << endl;
			delete[] angles;
			return;
			break;
		}
		delete[] angles; // Deallocating memory used for the angles array
		cout << "Would you like to print robot info? (Y/N)" << endl;
		cin >> choice;
		if (tolower(choice) == 'y') {
			choice = '2';
		}
	}
	// Handles displaying the robot info both in text and graphically
	if (choice == '2') {
		cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;
		cout << "ROBOT DISPLAY SUB-MENU:" << endl;
		cout << "ROBOT " << robNumber << ":" << endl;
		robots[robNumber - 1]->printRobotInfo(mode);
		cout << "Would you like to display robot " << robNumber << "'s information graphically as well? (Y/N)" << endl;
		cin >> choice;
		if (tolower(choice) == 'y') {
			cout << "Would you like to display it in full screen mode? (Y/N)" << endl;
			cin >> choice;
			if (tolower(choice) == 'y')
				robots[robNumber - 1]->printRobot(FULLSCREEN);
			else
				robots[robNumber - 1]->printRobot(SMALLSCREEN);
		}
		return;
	}
}