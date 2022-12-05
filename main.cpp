#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class time_type {
public:
    int day;
    int hour;
    int minute;
    time_type(int _day, int _hour, int _minute): day(_day), hour(_hour), minute(_minute) {}
    bool operator< (time_type &rhs) {
        if(day < rhs.day) return true;
        if(day == rhs.day && hour < rhs.hour) return true;
        if(day == rhs.day && hour == rhs.hour && minute < rhs.minute) return true;
        return false;
    }
    void print() {
        std::cout << hour << ":";
        if(minute < 10) std::cout << 0;
        std::cout << minute;
    }
};

class Course {
public:
    std::string name;
    std::string link;
    Course(std::string _name, std::string _link): name(_name), link(_link) {}
    Course() { name = std::string(); link = "Unknown"; }
};

class Course_time {
public:
    Course course;
    time_type start_time;
    time_type end_time;
    Course_time(Course _course, time_type _start_time, time_type _end_time): course(_course), start_time(_start_time), end_time(_end_time) {}
    bool operator< (Course_time &rhs) {
        if(end_time < rhs.end_time) return true;
        else return false;
    }
    void print(bool empha = false) {
        if(empha) SetConsoleTextAttribute(hConsole, 15);
        std::cout << "  " << course.name << "\t ";
        start_time.print();
        std::cout << " - ";
        end_time.print();
        std::cout << "\t link: ";
        if(empha) SetConsoleTextAttribute(hConsole, 11);
        std::cout << course.link << std::endl;
        SetConsoleTextAttribute(hConsole, 7); //reset
    }
};

class Schedule {
public:
    std::map <std::string, Course> course_list;
    std::vector <Course_time> time_table;

    Schedule() {}

    void add_course(std::string _name, std::string _link) {
        course_list.insert(make_pair(_name, Course(_name, _link)));
    }

    void add_course_time(std::string _name, time_type _start_time, time_type _end_time) {
        time_table.push_back(Course_time(course_list.at(_name), _start_time, _end_time));
    }

    void sort_course() {
        std::sort(time_table.begin(), time_table.end());
    }

    void read_course(std::string file_name) {
        std::ifstream fin;
        fin.open(file_name);
        while(true) {
            std::string buffer_name;
            std::string buffer_link;
            fin >> buffer_name;
            if(buffer_name == "end") break;
            fin >> buffer_link;
            add_course(buffer_name, buffer_link);
        }
        fin.close();
    }

    void read_course_time(std::string file_name) {
        std::ifstream fin;
        fin.open(file_name);
        while(true) {
            std::string buffer_name;
            int buffer_day, buffer_hour_1, buffer_minute_1, buffer_hour_2, buffer_minute_2;
            fin >> buffer_name;
            if(buffer_name == "end") break;
            fin >> buffer_day >> buffer_hour_1 >> buffer_minute_1 >> buffer_hour_2 >> buffer_minute_2;
            add_course_time(buffer_name, time_type(buffer_day, buffer_hour_1, buffer_minute_1), time_type(buffer_day, buffer_hour_2, buffer_minute_2));
        }
        fin.close();
    }

    void print_table() {
        for(auto &el: time_table) {
            std::cout << el.course.name << " " << el.start_time.day << " " << el.start_time.hour << ":" << el.start_time.minute << std::endl;
        }
    }

    void get_next_course(time_type _time) {
        for(int i = 0; i < time_table.size(); i++) {
            if(_time < time_table.at(i).end_time) {
                if(time_table.at(i).start_time < _time) {
                    std::cout << "Current course: " << std::endl;
                    time_table.at(i).print(true);
                    std::cout << "Upcoming course: " << std::endl;
                }
                else {
                    std::cout << "Next course: " << std::endl;
                    time_table.at(i).print(true);
                    std::cout << "Upcoming course: " << std::endl;
                }
                i ++;
                while(i < time_table.size()) {
                    if(time_table.at(i).start_time.day != time_table.at(i - 1).start_time.day) break;
                    time_table.at(i).print();
                    i ++;
                }
                return;
            }
        }
        std::cout << "No more course found this week" << std::endl;
    }

};

void print_stat(SYSTEMTIME time) {
    std::array <std::string, 8> name_of_week = {"null", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    std::cout << time.wYear << "/" << time.wMonth << "/" << time.wDay << " " << name_of_week.at(time.wDayOfWeek) << " ";
    std::cout << time.wHour << ":";
    if(time.wMinute < 10) std::cout << 0;
    std::cout << time.wMinute << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
}

int main() {
    SYSTEMTIME cur_systime;
    Schedule my_schedule;
    my_schedule.read_course("course.txt");
    my_schedule.read_course_time("course_time.txt");
    my_schedule.sort_course();

    while(true) {
        GetLocalTime(&cur_systime);
        print_stat(cur_systime);
        time_type cur_time(cur_systime.wDayOfWeek, cur_systime.wHour, cur_systime.wMinute);
        my_schedule.get_next_course(cur_time);
        Sleep(29500);
        std::cout << "(auto refreshing)" << std::endl;
        Sleep(500);
        system("cls");
    }
    
    getchar();
}