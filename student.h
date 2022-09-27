#ifndef STUDENT_H
#define STUDENT_H

#include "libs.h"

using namespace std;

class Student{
public:
    Student(string _dir, string _class_name, string _student_name);

    void run();

    vector<vector<string>> read_csv();

    void send_grade_to_course_process(string course_name, string grade);
      
private:
    string dir;
    string student_name;
    string class_name;
};

#endif