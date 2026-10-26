#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

struct Course {
    float grade;
    float credits;
};

int main() {
    int n;
    cout << "Enter number of courses: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of courses!\n";
        return 1;
    }

    vector<Course> courses(n);
    float totalCredits = 0.0f;
    float totalGradePoints = 0.0f;

    for (int i = 0; i < n; ++i) {
        cout << "\nCourse " << i + 1 << '\n';
        cout << "Enter grade point: ";
        cin >> courses[i].grade;
        cout << "Enter credit hours: ";
        cin >> courses[i].credits;

        if (courses[i].grade < 0 || courses[i].credits <= 0) {
            cout << "Invalid grade or credit hours!\n";
            return 1;
        }

        totalCredits += courses[i].credits;
        totalGradePoints += courses[i].grade * courses[i].credits;
    }

    const float gpa = totalGradePoints / totalCredits;

    cout << fixed << setprecision(2);
    cout << "\n----- Course Details -----\n";
    for (int i = 0; i < n; ++i) {
        cout << "Course " << i + 1
             << ": Grade = " << courses[i].grade
             << ", Credits = " << courses[i].credits
             << ", Grade Points = "
             << courses[i].grade * courses[i].credits << '\n';
    }

    cout << "\n----- CGPA Calculator -----\n";
    cout << "Total Credits: " << totalCredits << '\n';
    cout << "Total Grade Points: " << totalGradePoints << '\n';
    cout << "Semester GPA: " << gpa << '\n';
    cout << "Overall CGPA (one semester): " << gpa << '\n';

    cout << "\nNote: For CGPA across multiple semesters, include previous "
            "semesters' total grade points and credits.\n";
    return 0;
}
