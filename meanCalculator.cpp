#include "meanCalculator.h"

MeanCalculator::MeanCalculator(char* _dir)
{
    dir = convert_to_string(_dir);
    courses = {PHYSICS, ENGLISH, MATH, LITERATURE, CHEMISTRY};
}

string MeanCalculator::convert_to_string(char* a)
{
    string s(a);
    return s;
}

vector<string> MeanCalculator::get_folders_name()
{
    DIR *curr_dir;
    struct dirent *diread;
    vector<string> files;

    string path = "./" + dir;
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

void MeanCalculator::create_classes_process(vector<string>classes_name, int class_count)
{
    for(int i=0;i<class_count;i++)
    {
        pid_t class_pid = fork();

        if(class_pid == 0)
        {
            // Child process
            char* class_name = strdup(classes_name[i].c_str());

            int ret_val;
            int pfd[2];
            char buff[128] = "";

            ret_val = pipe(pfd);                 /* Create pipe */
            if (ret_val != 0) {             /* Test for success */
            printf("Unable to create a pipe; errno=%d\n",errno);

            exit(1);                         /* Print error message and exit */
            }
            
            pid_t curr_class_pid = fork();
            if (curr_class_pid == 0) {
                /* child program */
                close(pfd[1]); /* close the write end of pipe */
                ret_val = read(pfd[0],buff,strlen(class_name)); /* Read from pipe */
                if (ret_val != strlen(class_name)) {
                    printf("Read did not return expected value\n");
                    exit(3);                       /* Print error message and exit */
                }
                //cout << "buff:" << buff << "'" << endl;

                execl("class.out", "class.out", buff, strdup(dir.c_str()), NULL);
                exit(0);
            }
            else
            {
                /* parent program */
                close(pfd[0]); /* close the read end */
                ret_val = write(pfd[1],class_name,strlen(class_name)); /*Write to pipe*/
                if (ret_val != strlen(class_name)) {
                    printf("Write did not return expected value\n");
                    exit(2);                       /* Print error message and exit */
                }
                //printf("parent writes %s to the child program\n",class_name);
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
    for(int i=0;i<courses.size();i++)
    {
        wait(NULL);
    }
}

void MeanCalculator::create_courses_process()
{
    for(int i=0;i<courses.size();i++)
    {
        pid_t course_pid = fork();

        if(course_pid == 0)
        {
            // Child process
            char* course_name = strdup(courses[i].c_str());
            //cout << courses[i] << endl;
            strcpy(course_name,strdup(courses[i].c_str()));
            //cout << course_name << "'" <<endl;
            int ret_val;
            int pfd[2];
            char buff[128] = "";

            ret_val = pipe(pfd);                 /* Create pipe */
            if (ret_val != 0) {             /* Test for success */
            printf("Unable to create a pipe; errno=%d\n",errno);

            exit(1);                         /* Print error message and exit */
            }
            
            pid_t curr_course_pid = fork();
            if (curr_course_pid == 0) {
                /* child program */
                close(pfd[1]); /* close the write end of pipe */
                ret_val = read(pfd[0],buff,strlen(course_name)); /* Read from pipe */
                if (ret_val != strlen(courses[i].c_str())) {
                    printf("Read did not return expected value\n");
                    exit(3);                       /* Print error message and exit */
                }

                //cout << "courses buff: " << buff << "'"<< endl;
                execl("course.out", "course.out", buff, NULL);
                exit(0);
            }
            else {
                /* parent program */
                close(pfd[0]); /* close the read end */
                ret_val = write(pfd[1],course_name,strlen(course_name)); /*Write to pipe*/
                if (ret_val != strlen(course_name)) {
                    printf("Write did not return expected value\n");
                    exit(2);                       /* Print error message and exit */
                }
                //printf("parent writes %s to the child program\n",course_name);
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
    for(int i=0;i<courses.size();i++)
    {
        wait(NULL);
    }
}

void MeanCalculator::run()
{
    // Create two processes for classes and courses in school
    pid_t courses_pid, classes_pid;
    (courses_pid = fork()) && (classes_pid = fork());
    if (courses_pid == 0) {
        create_courses_process();
        exit(0);
    }
    else if(classes_pid == 0){
        vector<string> classes_name = get_folders_name();
        /*for(int i=0;i<classes_name.size();i++){
            cout << classes_name[i] << endl;
        }*/
        create_classes_process(classes_name, classes_name.size());
        exit(0);
    }
    else
    {
        wait(NULL);
        wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    MeanCalculator mean_calculator(argv[1]);
    mean_calculator.run();

    return 0;
}