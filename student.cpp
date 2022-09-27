#include "student.h"

Student::Student(string _dir, string _class_name, string _student_name)
{
    dir = _dir;
    class_name = _class_name;
    student_name = _student_name;
}

void Student::send_grade_to_course_process(string course_name, string grade)
{
    int fd;

    // FIFO file path
    string myfifo_str = "./tmp/" + course_name;
    char* myfifo = strdup(myfifo_str.c_str());

    //cout << myfifo << endl;

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    // Open FIFO for write only
    fd = open(myfifo, O_WRONLY);

    char arr1[100];
    strcpy(arr1,strdup(grade.c_str()));

    //cout << myfifo << endl;
    //cout << arr1 << endl;

    // Write the input arr2ing on FIFO
    // and close it
    write(fd, arr1, strlen(arr1)+1);

    //cout << myfifo << endl;
    //cout << arr1 << endl;

    close(fd);
}

vector<vector<string>> Student::read_csv()
{
	string path = "./" + dir + "/" + class_name + "/" + student_name;
    //cout << path << endl;
	vector<vector<string>> content;
	vector<string> row;
	string line, word;
 
	fstream file (path, ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();
 
			stringstream str(line);
 
			while(getline(str, word, ','))
            {
                row.push_back(word);
            }
			content.push_back(row);
		}
	}
	else
    {
        cout<<"Could not open the file\n";
    }

    return content;
 
	/*for(int i=0;i<content.size();i++)
	{
		for(int j=0;j<content[i].size();j++)
		{
			cout<<content[i][j]<< endl;
		}
	}*/
}

void Student::run()
{

    vector<vector<string>> content = read_csv();

    pid_t physics_pid, english_pid, math_pid, literature_pid, chemistry_pid;
    (physics_pid = fork()) && (english_pid = fork()) && (math_pid = fork()) && (literature_pid = fork()) && (chemistry_pid = fork());

    if (physics_pid == 0)
    {
        send_grade_to_course_process(content[0][0], content[0][1]);
        exit(0);
    }
    else if (english_pid == 0)
    {
        send_grade_to_course_process(content[1][0], content[1][1]);
        exit(0);
    }
    else if (math_pid == 0)
    {
        send_grade_to_course_process(content[2][0], content[2][1]);
        exit(0);
    }
    else if (literature_pid == 0)
    {
        send_grade_to_course_process(content[3][0], content[3][1]);
        exit(0);
    }
    else if (chemistry_pid == 0)
    {
        send_grade_to_course_process(content[4][0], content[4][1]);
        exit(0);
    }
    else
    {
        wait(NULL);
        wait(NULL);
        wait(NULL);
        wait(NULL);
        wait(NULL);
    }
}

int main(int argc, char *argv[]){
    string curr_dir(argv[1]);
    string curr_class_name(argv[2]);
    string curr_student_name(argv[3]);
    //cout << curr_dir << endl;
    Student student(curr_dir, curr_class_name, curr_student_name);

    student.run();
    return 0;
}