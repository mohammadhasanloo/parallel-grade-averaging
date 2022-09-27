#ifndef COURSE_H
#define COURSE_H

#include "libs.h"


using namespace std;

class Course{
public:
    Course(string _course_name);

    void run();

private:
    string course_name;
};

#endif