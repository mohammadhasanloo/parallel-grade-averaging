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

        fd1 = open(myfifo, O_RDONLY);
        if (fd1 < 0)
            continue;

        // read() fills the buffer but does not terminate it, and it may return
        // fewer bytes than asked for. Both have to be handled, or the stream
        // prints whatever happens to follow in the buffer.
        ssize_t received = read(fd1, str1, sizeof(str1) - 1);
        close(fd1);

        if (received <= 0)
            continue;
        str1[received] = '\0';

        cout << course_name << ": " << str1 << endl;
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