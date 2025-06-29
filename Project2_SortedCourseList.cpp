//============================================================================
// Name        : Project2.cpp
// Author      : Jared Borck
// Date        : 6/22/20205
// Version     : 1.0
// Description : Project 2
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Holds info about each course
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Node structure for the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        this->course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Our BST for storing courses
class CourseBST {
private:
    Node* root;

    // Helper to insert new nodes into tree
    void insert(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else {
            insert(node->right, course);
        }
    }

    // Go through the tree in order (A-Z)
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseNumber << ": " << node->course.title << endl;
        inOrder(node->right);
    }

    // Look for and print course info
    bool printCourse(Node* node, const string& courseNumUpper) {
        if (node == nullptr) return false;

        if (node->course.courseNumber == courseNumUpper) {
            cout << node->course.courseNumber << ": " << node->course.title << endl;
            if (!node->course.prerequisites.empty()) {
                cout << "Prerequisites: ";
                for (size_t i = 0; i < node->course.prerequisites.size(); ++i) {
                    cout << node->course.prerequisites[i];
                    if (i < node->course.prerequisites.size() - 1) cout << ", ";
                }
                cout << endl;
            }
            else {
                cout << "Prerequisites: None" << endl;
            }
            return true;
        }
        else if (courseNumUpper < node->course.courseNumber) {
            return printCourse(node->left, courseNumUpper);
        }
        else {
            return printCourse(node->right, courseNumUpper);
        }
    }

    // Recursively clear memory
    void destroy(Node* node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:
    CourseBST() { root = nullptr; }
    ~CourseBST() { destroy(root); }

    void Insert(Course course) {
        insert(root, course);
    }

    void PrintCourseList() {
        inOrder(root);
    }

    void PrintCourse(string courseNum) {
        string upper = courseNum;
        transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        bool found = printCourse(root, upper);
        if (!found) {
            cout << "Course not found: " << courseNum << endl;
        }
    }
};

// Strip spaces from both sides of text
string Trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return ""; // string is all spaces
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Read data from CSV into the BST
void LoadCourses(string filename, CourseBST& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file \"" << filename << "\"." << endl;
        cout << "Make sure the file is in the same folder and the name is typed exactly (including .csv)." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string word;
        vector<string> tokens;

        while (getline(ss, word, ',')) {
            tokens.push_back(Trim(word));
        }

        Course course;
        if (tokens.size() >= 2) {
            course.courseNumber = tokens[0];
            course.title = tokens[1];
            for (size_t i = 2; i < tokens.size(); ++i) {
                course.prerequisites.push_back(tokens[i]);
            }
            bst.Insert(course);
        }
    }

    file.close();
    cout << "Courses successfully loaded!" << endl;
}

// Display the user menu
void DisplayMenu() {
    cout << "\nMenu:" << endl;
    cout << "  1. Load course data" << endl;
    cout << "  2. Print course list" << endl;
    cout << "  3. Print course" << endl;
    cout << "  9. Exit" << endl;
    cout << "Enter choice: ";
}

// Main program loop
int main() {
    CourseBST bst;
    int choice;
    string filename;
    string courseNum;

    do {
        DisplayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter file name: ";
            cin.ignore(); // Clear leftover newline
            getline(cin, filename); // Allow full filename input
            LoadCourses(filename, bst);
            break;
        case 2:
            cout << "\nCourse List:\n";
            bst.PrintCourseList();
            break;
        case 3:
            cout << "Enter course number (e.g., CSCI200): ";
            cin >> courseNum;
            bst.PrintCourse(courseNum);
            break;
        case 9:
            cout << "Exiting program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }

    } while (choice != 9);

    return 0;
}
