//============================================================================
// Name        : Project2.cpp
// Author      : Eric Schooler-Davison
// Version     : 1.0
// Description : Project 2 University Advisor Course Tool
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

// define a structure to hold course information
struct Course {
	string courseId;
	string courseName;
	vector<string> prereq;
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

//============================================================================
// Course Tree class definition
//============================================================================

class CourseTree {

private:
	void Destruct(Node* node);

public:
	Node* root;
	CourseTree();
	virtual ~CourseTree();
	void DisplayCourse(Node* node);
	void Insert(CourseTree* course, Node* node);
	void Search(string courseId);
};

// Default constructor
CourseTree::CourseTree() {
	root = nullptr;
}

// Destructor
CourseTree::~CourseTree() {
	Destruct(root);
}

void CourseTree::Destruct(Node* node) {
	if (node != nullptr) {
		Destruct(node->left);
		node->left = nullptr;
		Destruct(node->right);
		node->right = nullptr;
		delete node;
	}
}

// Insert a course
void CourseTree::Insert(CourseTree* course, Node* node) {
	if (course->root == nullptr) {
		course->root = node;
	}
	else {
		Node* current = course->root;
		while (current != nullptr) {
			if (node->course.courseId < current->course.courseId) {
				if (current->left == nullptr) {
					current->left = node;
					current = nullptr;
				}
				else {
					current = current->left;
				}
			}
			else {
				if (current->right == nullptr) {
					current->right = node;
					current = nullptr;
				}
				else {
					current = current->right;
				}
			}
		}
	}
}

// Search for a bid
void CourseTree::Search(string courseId) {
	Node* current = root;

	// As long as the pointer is not empty
	while (current != nullptr) {

		// If current node is a match then return it
		if (current->course.courseId == courseId) {
			cout << current->course.courseName << ":" << endl
				<< "==============================="
				<< "Course ID: " << current->course.courseId << endl
				<< "Prerequisits: ";

			// Checks current pointer for prereq
			for (string prereq : current->course.prereq) {
				if (prereq == current->course.prereq.back()) {
					cout << prereq << endl;
				}
				else {
					cout << prereq << ", ";
				}
			}

			return;

		}

		// Checks for courseId
		else if (courseId < current->course.courseId) {
			if (current->left != nullptr) {
				current = current->left;
			}
		}

		else {
			current = current->right;
		}
	}

	cout << "Course " << courseId << "not found." << endl;
	return;

}

//============================================================================
// Static methods used for testing
//============================================================================

// Display courses to console
void CourseTree::DisplayCourse(Node* node) {
	if (node == nullptr) {
		return;
	}

	DisplayCourse(node->left);
	cout << node->course.courseId << ", ";
	cout << node->course.courseName << endl;
	DisplayCourse(node->right);
}

// Load the CSV file with information
void loadCourses(string csvPath, CourseTree* bst) {
	cout << "Loading CSV file " << csvPath << endl;
	ifstream file(csvPath);

	// Checks if the file is open, then gets info
	if (file.is_open()) {
		cout << "File Loaded." << endl;

		int num;
		string line;
		string word;

		// While data can still be found
		while (getline(file, line)) {
			num = 0;
			Node* node = new Node();
			stringstream str(line);

			// While there is less than 2 itterations
			while (num < 2) {
				getline(str, word, ',');
				if (num == 0) {
					node->course.courseId = word;
				}
				else {
					node->course.courseName = word;
				}
				num++;
			}

			while (getline(str, word, ',')) {
				node->course.prereq.push_back(word);
			}

			bst->Insert(bst, node);
		}
	}

	else {
		cout << "File Error" << endl;
		return;
	}
}

void main() {
	CourseTree* bst = new CourseTree();

	string fileChoice;
	string courseChoice;
	string defaultInput;

	int input = 0;

	// While input from user does not equal 9, display menu
	while (input != 9) {
		cout << "Menu:" << endl
			<< "====================" << endl
			<< "  1. Load Courses" << endl
			<< "  2. Display All Courses" << endl
			<< "  3. Search Course" << endl
			<< "  9. Quit" << endl
			<< endl << "Enter Input: ";
		cin >> input;
		cout << endl;

		switch (input) {

		// User enters option 1 to load file
		case 1:
			// Uses saved default unless the user enters n
			cout << "Would you like to load default file?" << endl
				<< "Y/N" << endl;
			cin >> defaultInput;

			// Uses default
			if (defaultInput == "Y") {
				fileChoice = "CS 300 ABCU_Advising_Program_Input.csv";
			}

			// Else uses an entered file name
			else {
				cout << "Please enter file name:" << endl;
				cin >> fileChoice;
			}

			loadCourses(fileChoice, bst);
			cout << endl;
			break;

		// User enters option 2 to display all
		case 2:
			cout << "Course ID      Course Name" << endl;
			cout << "==========================";

			bst->DisplayCourse(bst->root);
			cout << endl;
			break;

		// User enters option 3 to search for an option
		case 3:
			
			// Takes course intake
			cout << "Enter the course to search: ";
			cin >> courseChoice;
			cout << endl;

			// Displays search results
			std::transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);
			bst->Search(courseChoice);

			cout << endl;
			break;

		// If the input can not be read
		default:
			cout << input << " not found." << endl;
			break;
		}
	}
	// End of program
	cout << "Good bye!" << endl;
}
