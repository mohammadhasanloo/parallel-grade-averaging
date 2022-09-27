#ifndef CLASS_H
#define CLASS_H

#include "libs.h"
using namespace std;



class ClassHandler{
public:
    ClassHandler(string _class_name,string _dir);

    void create_students_process(vector<string> students);

    string convert_to_string(char* a);

    vector<string> get_students_name();

    void run();

private:
    string class_name;
    string dir;
};

#endif