
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// key of hash map : student name
// value: number of student with same name
unordered_map<string, int> parseFile(string fileName) {
    unordered_map<string, int> studentMap;
    string line;
    ifstream myfile;
    myfile.open(fileName);
    if (myfile.is_open()) {
        while (getline (myfile,line)) {
            if (line.substr(0,8)=="Student#") {
                break;
            }
        }
        while (getline (myfile,line)) {
            if (isdigit(line[0])) {
                int startIndexLastName = 0;
                int lenLastName = 0;
                int startFirstName = -1;
                int endFirstName;
                bool flag = false;
                for (int i = 0; i < line.size(); i++) {
                    if (i == startFirstName) {
                        flag = true;
                    }
                    if (isalpha(line[i]) && startIndexLastName == 0) {
                        startIndexLastName = i;
                    } else if (line[i] == ',') {
                        lenLastName = i - startIndexLastName;
                        startFirstName = i+2;
                    } else if (flag == true && isspace(line[i])) {
                        endFirstName = i - startFirstName;
                        break;
                    }
                }
                string lastName = line.substr(startIndexLastName, lenLastName);
                string firstName = line.substr(startFirstName, endFirstName);
                studentMap[lastName + " " + firstName]++;
            }
        }
    }
    myfile.close();
    return studentMap;
}

vector<string> getAbsentStudent() {
    cout<<"absent students: (enter q to finish input)"<<endl;
    cout<<"example: ZHU ZHANYAN (Last name[space]First Name)"<<endl;
    vector<string> absentStudent;
    while (true) {
        string studentName;
        getline(cin, studentName);
        if (studentName != "q") {
            std::transform(studentName.begin(), studentName.end(),studentName.begin(), ::toupper);
            absentStudent.push_back(studentName);
        } else {
            break;
        }
    }
    return absentStudent;
}

void shuffle(vector<string> &ret) {
    srand(time(NULL));
    for (int i = 0;i < ret.size();i++) {
        int pos = rand()%(ret.size()-i);
        swap(ret[i+pos], ret[i]);
    }
}

vector<string> getStudentList(unordered_map<string, int> studentCountMap) {
    vector<string> ret;
    for (auto item : studentCountMap) {
        for (int j = 0; j < item.second; j++) {
            ret.push_back(item.first);
        }
    }
    return ret;
}

vector<string> assignGroup(vector<string> studentList, int groupSize) {
    vector<string> groupVector;
    int numStudent = studentList.size();
    int remain = numStudent % groupSize;
    int groupNum = numStudent/groupSize;
    int i = 0; 
    int currGroup = 1;
    while (i < groupNum*groupSize) {
        string groupStr;
        groupStr += to_string(currGroup) + ".\n" ;
        for (int j = 0; j < groupSize; j++) {
                groupStr += studentList[i];
                groupStr += "\n";
                i++;
        }
        groupVector.push_back(groupStr);
        currGroup++;
    }
    if (remain != 0) {
        for (int i = 0; i < remain; i++) {
            groupVector[groupVector.size()-1-i] += studentList[i];
            groupVector[groupVector.size()-1-i] += "\n";
        }
    }
    return groupVector;
}

void getGroupSize(int& groupSize) {
    cout<<"group size: "<<endl;
    cin>>groupSize;
}

int main() {
    int groupSize;
    unordered_map<string, int> studentCountMap;
    string fileName = "roster.txt";
    vector<string> absentStudent;
    studentCountMap = parseFile(fileName);
    getGroupSize(groupSize);
    absentStudent = getAbsentStudent();
    for (int i = 0; i < absentStudent.size(); i++) {
        studentCountMap[absentStudent[i]]--;
    }
    vector<string> studetList;
    studetList = getStudentList(studentCountMap);
    shuffle(studetList);
    vector<string> retList = assignGroup(studetList, groupSize);
    for (int i = 0; i < retList.size(); i++) {
        cout<<retList[i]<<endl;
    }
    //cout<<"GroupSize="<<groupSize;
    return 0;
}