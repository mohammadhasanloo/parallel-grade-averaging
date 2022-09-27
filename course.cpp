#include "course.h"

Course::Course(string _course_name)
{
    course_name = _course_name;
}

void Course::run()
{
    //cout << "In course class we have: "<< course_name << endl;


    int fd1;

    // FIFO file path
    string myfifo_str = "./tmp/" + course_name;
    char* myfifo = strdup(myfifo_str.c_str());

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    while (1)
    {
        char str1[80];

        // First open in read only and read
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, str1, 80);

        // Print the read string and close
        cout << course_name << ": " << str1 << endl;
        close(fd1);
    }
}


int main(int argc, char *argv[])
{
    string curr_course_name(argv[1]);
    //cout << curr_course_name << endl;
    Course course(curr_course_name);
    course.run();

    return 0;
}