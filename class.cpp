#include "class.h"

ClassHandler::ClassHandler(string _class_name,string _dir){
    class_name = _class_name;
    dir = _dir;
}

string ClassHandler::convert_to_string(char* a)
{
    string s(a);
    return s;
}

vector<string> ClassHandler::get_students_name()
{
    DIR *curr_dir;
    struct dirent *diread;
    vector<string> files;

    string path = "./" + dir + "/" + class_name;
    if ((curr_dir = opendir(strdup(path.c_str()))) != nullptr)
    {
        while ((diread = readdir(curr_dir)) != nullptr) {
            // Because some unknown folder opens like . and .. (So names' length should be more than 2)
            if(convert_to_string(diread->d_name).size() > 2)
            {
                files.push_back(convert_to_string(diread->d_name));
            }
        }
        closedir(curr_dir);
    } 
    else
    {
        perror("opendir");
    }

    //for (auto file : files) cout << file << "| ";
    //cout << endl;

    return files;
}

void ClassHandler::create_students_process(vector<string> students)
{
    for(int i=0;i<students.size();i++)
    {
        pid_t student_pid = fork();

        if(student_pid == 0)
        {
            // Child process
            char* student_name = strdup(students[i].c_str());

            int ret_val;
            int pfd[2];
            char buff[128] = "";

            ret_val = pipe(pfd);                 /* Create pipe */
            if (ret_val != 0) {             /* Test for success */
            printf("Unable to create a pipe; errno=%d\n",errno);

            exit(1);                         /* Print error message and exit */
            }
            
            pid_t curr_student_pid = fork();
            if (curr_student_pid == 0) {
                /* child program */
                close(pfd[1]); /* close the write end of pipe */
                ret_val = read(pfd[0],buff,strlen(student_name)); /* Read from pipe */
                if (ret_val != strlen(student_name)) {
                    printf("Read did not return expected value\n");
                    exit(3);                       /* Print error message and exit */
                }
                //cout << "buff:" << buff << "'" << endl;

                execl("student.out", "student.out", strdup(dir.c_str()), strdup(class_name.c_str()), buff, NULL);
                exit(0);
            }
            else
            {
                /* parent program */
                close(pfd[0]); /* close the read end */
                ret_val = write(pfd[1],student_name,strlen(student_name)); /*Write to pipe*/
                if (ret_val != strlen(student_name)) {
                    printf("Write did not return expected value\n");
                    exit(2);                       /* Print error message and exit */
                }
                //printf("parent writes %s to the child program\n",student_name);
                wait(NULL);
            }
            exit(0);
        }
        else
        {
            // Parent process
            continue;
        }
    }
    for(int i=0;i<students.size();i++)
    {
        wait(NULL);
    }
}

void ClassHandler::run()
{
    vector<string> students = get_students_name();
    create_students_process(students);
}

int main(int argc, char *argv[]){

    string curr_class_name(argv[1]);
    string curr_dir(argv[2]);
    //cout << curr_class_name << " " <<curr_dir<< endl;

    ClassHandler class_handler(curr_class_name, curr_dir);
    class_handler.run();
    return 0;
}