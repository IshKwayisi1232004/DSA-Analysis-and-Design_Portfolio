//============================================================================
// Name        : Project_2.cpp
// Author      : Ishmael Kwayisi
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Project_2
//============================================================================

#include <filesystem>
#include <iostream>
#include <time.h>
#include <functional>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <cctype>    // for toupper

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold bid information
struct Course {
    string courseNumber; // unique identifier
    string title;
    vector<string> prerequisites;
};

vector<Course> courses;

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

    // initialize with a bid
    Node(Course aCourse) :
        Node() {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Course search(Node* node, string courseNumber);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {

    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node

    std::function<void(Node*)> deleteRecursively = [&](Node* node) {
        if (node != nullptr) {
            deleteRecursively(node->left);
            deleteRecursively(node->right);
            delete node;
        }
        };

    deleteRecursively(root);

    root = nullptr;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}


/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course);
        root->left = nullptr;
        root->right = nullptr;
    }
    // else
    else {
        // add Node root and bid
        addNode(root, course);

    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    return search(root, courseNumber);
}

Course BinarySearchTree::search(Node* node, string courseNumber) {
    if (node == nullptr) {
        return Course();
    }

    if (node->course.courseNumber == courseNumber) {
        return node->course;
    }

    if (courseNumber < node->course.courseNumber) {
        return search(node->left, courseNumber);
    }
    else {
        return search(node->right, courseNumber);
    }
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param Course course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseNumber > course.courseNumber) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else recurse down the left node
        else {
            addNode(node->right, course);
        }
    }
}
void BinarySearchTree::inOrder(Node* node) {

    //if node is not equal to null ptr
    if (node == nullptr) {
        return;
    }
    //InOrder not left
    inOrder(node->left);
    //output bidID, title, amount, fund
    cout << node->course.courseNumber << ", "
        << node->course.title << endl;
    //InOrder right
    inOrder(node->right);
}

/**
 * Load the courses to the console (std::out)
 *
 * @param string struct containing the course info
 * @param BinarySearchTree class containing the bst structure
 */
void loadCourses(string fileName, BinarySearchTree* bst) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error opening file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Course course;

        getline(ss, course.courseNumber, ',');
        getline(ss, course.title, ',');

        while (getline(ss, token, ',')) {
            course.prerequisites.push_back(token);
        }

        bst->Insert(course);
    }

    file.close();
}


/**
 * Display the course information to the console (std::out)
 *
 * @param Course struct containing the course info
 */
void displayCourseInfo(Course course) {

    cout << course.courseNumber << ", " << course.title << endl;
    cout << "Prerequisites: ";
    if (course.prerequisites.empty()) {
        cout << "None";
    }
    else {
        for (size_t i = 0; i < course.prerequisites.size(); i++) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    cout << endl;
}

/**
 * The one and only main() method
 */
int main() {

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    string filename, courseNum, tempCourseNum;
    cout << "Welcome to the course planner!" << endl << endl;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course Info" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            cout << "Enter file name: ";
            cin.ignore(); // clears leftover newline from menu choice input
            getline(cin, filename);

            // Complete the method call to load the course
            loadCourses(filename, bst);
            cout << "Courses loaded." << endl;
            
            break;

        case 2:
            cout << "Here is a sample schedule: " << endl << endl;
            bst->InOrder();
            cout << endl;
            break;

        case 3:
            cout << "Enter course number: ";
            cin >> courseNum;

            tempCourseNum.clear();

            for (int i = 0; i < courseNum.size(); i++) {
                tempCourseNum.push_back(toupper(courseNum.at(i)));
            }

            course = bst->Search(tempCourseNum);
            
            if (course.courseNumber.empty()) {
                cout << "Course not found!" << endl;
            }
            else {
                displayCourseInfo(course);
            }

            break;

        default:
            cout << choice << " is an invalid choice!" << endl << endl;
        }
    }

    cout << "Good bye." << endl;

    delete bst;

    return 0;
}
