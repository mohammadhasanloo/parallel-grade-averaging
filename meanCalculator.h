#ifndef MEANCALCULATOR_H
#define MEANCALCULATOR_H



#include "libs.h"
using namespace std;



class MeanCalculator{

public:
    MeanCalculator(char* _dir);

    string convert_to_string(char* a);
    
    vector<string> get_folders_name();

    void create_classes_process(vector<string>classes_name, int class_count);

    void create_courses_process();

    void run();


private:
    string dir;
    vector<string> courses;
};



#endif