//============================================================================
// Name        : ProjectTwo.cpp
// Author      : William Mitchem
// Version     : 1.0
// Description : CS 300 Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Internal structure for courses
struct Course {
    string courseId;
    string title;
    vector<string> prerequisites;

    Course() {
        this->courseId = "";
        this->title = "";
        this->prerequisites = {};
    }

    Course(string courseId, string title, vector<string> prerequisites) {
        this->courseId = courseId;
        this->title = title;
        this->prerequisites = prerequisites;
    }
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

    // initialize with a bid
    Node(Course course) : Node() {
        this->course = course;
    }
};

// Simple binary search tree implementation
class BinarySearchTree {
private:
    Node* root;
    int size;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    void Insert(Course course);
    void InOrder();
    int GetSize();
    Course Search(string courseId);
};

void displayCourse(Course course);

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
    size = 0;
}

void BinarySearchTree::Insert(Course course) {
    if (root == nullptr)
        root = new Node(course);
    else
        this->addNode(root, course);
    size++;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    if (node->course.courseId > course.courseId) {
        if (node->left == nullptr)
            node->left = new Node(course);
        else
            addNode(node->left, course);
    }
    else {
        if (node->right == nullptr)
            node->right = new Node(course);
        else {
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        displayCourse(node->course);
        inOrder(node->right);
    }
}

/**
 * Search for a course 
 */
Course BinarySearchTree::Search(string courseId) {
    Node* current = root;
    
    while (current != nullptr) {
        if (current->course.courseId == courseId)
            return current->course;
        else if (current->course.courseId < courseId)
            current = current->right;
        else
            current = current->left;
    }

    // Course with the given courseId wasn't located.
    return Course();
}

void displayCourse(Course course) {
    cout << course.courseId << ", " << course.title << endl;
    cout << "Prerequisites: ";
    if (course.prerequisites.empty())
        cout << "No courses required";
    else
        for (auto i = course.prerequisites.begin(); i < course.prerequisites.end(); i++)
            cout << *i << " ";
    cout << endl << endl;
}

void parseFile(string fileName, BinarySearchTree* bst) {
    ifstream inputFile;
    inputFile.open(fileName);

    if (!inputFile.is_open()) {
        cout << "ERROR: File not found or failed to open." << endl;
        return;
    }

    string line = "";
    while (getline(inputFile, line)) {
        string courseId = "";
        string title = "";
        string prerequisite = "";
        vector<string> prerequisites;

        stringstream lineStream(line);
        getline(lineStream, courseId, ',');
        getline(lineStream, title, ',');

        if (courseId.compare("") == 0 || title.compare("") == 0) {
            cout << "ERROR: Line is missing a course ID or title." << endl;
            continue;
        }

        while (getline(lineStream, prerequisite, ',')) {
            stringstream prereqString(prerequisite);
            while (getline(prereqString, prerequisite, ','))
                prerequisites.push_back(prerequisite);
        }

        bst->Insert(Course(courseId, title, prerequisites));
        line = "";
    }

    inputFile.close();
}

int main()
{
    string fileName = "CS 300 ABCU_Advising_Program_Input.csv";
    BinarySearchTree* bst = new BinarySearchTree();

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            parseFile(fileName, bst);
            cout << "Courses successfully loaded." << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            string courseName = "";
            cout << "Enter a course to search for: ";
            cin >> courseName;
            displayCourse(bst->Search(courseName));
            break;
        }
    }
    
    cout << "Good bye." << endl;
    return 0;
}

