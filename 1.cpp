#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <math.h>
using namespace std;
#define COMMA ','
#define BACK_SLASH '/'
#define DASH '-'
#define NUMBER_OF_COURSE_ADDRESS_IN_INPUT_ORDER 0
#define NUMBER_OF_GRADE_ADDRESS_IN_INPUT_ORDER 1
#define COURSE_ADDRESS 1
#define GRADE_ADDRESS 2
#define CAN_TAKE_CURRENT_COURSE 1
#define CANNOT_TAKE_CURRENT_COURSE 0
#define COURSE_ID 1
#define COURSE_NAME 2
#define COURSE_UNITS 3
#define COURSE_SCHEDULE 4
#define COURSE_PREREQUISITES 5
#define SCHEDULE_DAY 1
#define SCHEDULE_START_TIME 2
#define SCHEDULE_END_TIME 3
#define FIRST_VALID_LINE 1
#define GRADES_ID 1
#define GRADES_GRADE 2
#define MINIMUM_PASSED_GRADE 10
#define FIRST_ELEMENT 0
#define INPUT_ORDERS 2
#define EMPTY 0

typedef struct course Course;
typedef vector<Course> CourseTable;
typedef struct schedule Schedule;
typedef vector<Schedule> CourseSchedule;
typedef vector<int> Prerequisites;
typedef struct grade Grade;
typedef vector<Grade> CourseGrade;

struct schedule {
	string dey;
	int start_time;
	int end_time;
};

struct course {
	int id;
	string name;
	int units;
	vector<Schedule> schedule;
	Prerequisites prerequisites;
};

struct grade {
	int id;
	long double course_grade;
};

CourseTable read_course_info(vector<string> input_address);
vector<string> read_file_lines(vector<string> input_address, int file_number);
Course create_new_course(string line);
int number_string_to_int(string line_element);
CourseSchedule string_to_schedule_vector(string line_element);
Schedule create_new_schedule(string day_schedule);
int time_string_to_int(string time_string);
Prerequisites string_to_prerequisites_vector(string line_element);
CourseGrade read_grades_info(vector<string> input_address);
Grade create_new_course_grade(string line);
long double number_string_to_long_double(string line_element);
CourseTable find_all_takable_courses(CourseTable course_table_info, CourseGrade course_grade_info);
Prerequisites find_passed_course_id(CourseGrade course_grade_info);
int check_takable_current_course(Course current_course, Prerequisites passed_courses_id);
int check_not_take_current_course_before(Course current_course, Prerequisites passed_courses_id);
int ckeck_taked_current_course_prerequisites_before(Course current_course, Prerequisites passed_courses_id);
CourseTable generate_output(CourseTable takable_courses);
CourseTable takable_courses_swap(CourseTable takable_courses, int current_takable_course, int swap_element);
void print_result(CourseTable takable_courses);

int main(int argc, char* argv[])
{
	vector<string> input_address(INPUT_ORDERS);
	input_address[NUMBER_OF_COURSE_ADDRESS_IN_INPUT_ORDER] = argv[COURSE_ADDRESS];
	input_address[NUMBER_OF_GRADE_ADDRESS_IN_INPUT_ORDER] = argv[GRADE_ADDRESS];
	CourseTable course_table_info = read_course_info(input_address);
	CourseGrade course_grade_info = read_grades_info(input_address);
	CourseTable all_takable_courses = find_all_takable_courses(course_table_info, course_grade_info);
	all_takable_courses = generate_output(all_takable_courses);
	print_result(all_takable_courses);
	return 0;
}

CourseTable read_course_info(vector<string> input_address)
{
	CourseTable course_table_info;
	vector<string> file_lines = read_file_lines(input_address, NUMBER_OF_COURSE_ADDRESS_IN_INPUT_ORDER);
	for (int current_line = FIRST_VALID_LINE; current_line < file_lines.size(); current_line++)
		course_table_info.push_back(create_new_course(file_lines[current_line]));
	return course_table_info;
}

vector<string> read_file_lines(vector<string> input_address, int file_number)
{
	vector<string> read_file_lines;
	ifstream course_file(input_address[file_number].c_str());
	string line;
	while (getline(course_file, line))
		read_file_lines.push_back(line);
	return read_file_lines;
}

Course create_new_course(string line)
{
	string line_element;
	Course new_course;
	stringstream string_stream(line);
	int line_order_count = COURSE_ID;
	while (getline(string_stream, line_element, COMMA))
	{
		if (line_order_count == COURSE_ID)
			new_course.id = number_string_to_int(line_element);
		if (line_order_count == COURSE_NAME)
			new_course.name = line_element;
		if (line_order_count == COURSE_UNITS)
			new_course.units = number_string_to_int(line_element);;
		if (line_order_count == COURSE_SCHEDULE)
			new_course.schedule = string_to_schedule_vector(line_element);
		if (line_order_count == COURSE_PREREQUISITES)
			new_course.prerequisites = string_to_prerequisites_vector(line_element);
		line_order_count++;
	}
	return new_course;
}

int number_string_to_int(string line_element)
{
	int number = 0;
	for (int current = 0; current < line_element.size(); current++)
	{
		if (current > 0)
			number *= 10;
		number += (int(line_element[current]) - 48);
	}
	return number;
}

CourseSchedule string_to_schedule_vector(string line_element)
{
	CourseSchedule current_course_schedule;
	string day_schedule;
	stringstream string_stream(line_element);
	while (getline(string_stream, day_schedule, BACK_SLASH))
		current_course_schedule.push_back(create_new_schedule(day_schedule));
	return current_course_schedule;
}

Schedule create_new_schedule(string day_schedule)
{
	int element_count = SCHEDULE_DAY;
	Schedule current_schedule;
	string day_schedule_details;
	stringstream string_stream(day_schedule);
	while (getline(string_stream, day_schedule_details, DASH))
	{
		if (element_count == SCHEDULE_DAY)
			current_schedule.dey = day_schedule_details;
		if (element_count == SCHEDULE_START_TIME)
			current_schedule.start_time = time_string_to_int(day_schedule_details);
		if (element_count == SCHEDULE_END_TIME)
			current_schedule.end_time = time_string_to_int(day_schedule_details);
		element_count++;
	}
	return current_schedule;
}

int time_string_to_int(string time_string)
{
	int time = 0;
	for (int current = 0; current < time_string.size(); current++)
		if (current != ((time_string.size() + 1) / 2 - 1))
		{
			if (current > 0)
				time *= 10;
			time += (int(time_string[current]) - 48);
		}
	return time;
}

Prerequisites string_to_prerequisites_vector(string line_element)
{
	Prerequisites current_course_prerequisites;
	string prerequisites_details;
	stringstream string_stream(line_element);
	while (getline(string_stream, prerequisites_details, DASH))
		current_course_prerequisites.push_back(number_string_to_int(prerequisites_details));
	return current_course_prerequisites;
}

CourseGrade read_grades_info(vector<string> input_address)
{
	CourseGrade course_grade_info;
	vector<string> file_lines = read_file_lines(input_address, NUMBER_OF_GRADE_ADDRESS_IN_INPUT_ORDER);
	for (int current_line = FIRST_VALID_LINE; current_line < file_lines.size(); current_line++)
		course_grade_info.push_back(create_new_course_grade(file_lines[current_line]));
	return course_grade_info;
}

Grade create_new_course_grade(string line)
{
	string line_element;
	Grade new_course_grade;
	stringstream string_stream(line);
	int line_order_count = GRADES_ID;
	while (getline(string_stream, line_element, COMMA))
	{
		if (line_order_count == GRADES_ID)
			new_course_grade.id = number_string_to_int(line_element);
		if (line_order_count == GRADES_GRADE)
			new_course_grade.course_grade = number_string_to_long_double(line_element);
		line_order_count++;
	}
	return new_course_grade;
}

long double number_string_to_long_double(string line_element)
{
	long double grade = 0;
	int decimalous_number = 0;
	for (int current = 0;current < line_element.size(); current++) 
		if (current != 2)
		{
			if (current == 1)
				grade *= 10;
			if (current > 2)
			{
				decimalous_number++;
				long double number = pow(0.1, decimalous_number);
				grade += ((int(line_element[current]) - 48)*number);
				continue;
			}
			grade += (int(line_element[current]) - 48);
		}
	return grade;
}

CourseTable find_all_takable_courses(CourseTable course_table_info, CourseGrade course_grade_info)
{
	CourseTable takable_courses;
	Prerequisites passed_courses_id = find_passed_course_id(course_grade_info);
	for (int current_course = 0; current_course < course_table_info.size(); current_course++)
		if (check_takable_current_course(course_table_info[current_course], passed_courses_id))
			takable_courses.push_back(course_table_info[current_course]);
	return takable_courses;
}

Prerequisites find_passed_course_id(CourseGrade course_grade_info)
{
	Prerequisites passed_courses_id;
	for (int current = 0; current < course_grade_info.size(); current++)
		if (course_grade_info[current].course_grade >= MINIMUM_PASSED_GRADE)
			passed_courses_id.push_back(course_grade_info[current].id);
	return passed_courses_id;
}

int check_takable_current_course(Course current_course, Prerequisites passed_courses_id)
{
	if (!(check_not_take_current_course_before(current_course, passed_courses_id)))
		return CANNOT_TAKE_CURRENT_COURSE;
	if (!(ckeck_taked_current_course_prerequisites_before(current_course, passed_courses_id)))
		return CANNOT_TAKE_CURRENT_COURSE;
	return CAN_TAKE_CURRENT_COURSE;
}

int check_not_take_current_course_before(Course current_course, Prerequisites passed_courses_id)
{
	for (int current_passed_courses_id = 0; current_passed_courses_id < passed_courses_id.size(); current_passed_courses_id++)
		if (passed_courses_id[current_passed_courses_id] == current_course.id)
			return CANNOT_TAKE_CURRENT_COURSE;
	return CAN_TAKE_CURRENT_COURSE;
}

int ckeck_taked_current_course_prerequisites_before(Course current_course, Prerequisites passed_courses_id)
{
	int current_takable_course_result = CAN_TAKE_CURRENT_COURSE;
	if (current_course.prerequisites[FIRST_ELEMENT] != EMPTY) 
		for (int current_course_prerequisites = 0; current_course_prerequisites < current_course.prerequisites.size(); current_course_prerequisites++)
		{
			for (int current_passed_course_id = 0; current_passed_course_id < passed_courses_id.size(); current_passed_course_id++)
				if (passed_courses_id[current_passed_course_id] == current_course.prerequisites[current_course_prerequisites])
				{
					current_takable_course_result = CAN_TAKE_CURRENT_COURSE;
					break;
				}
				else
					current_takable_course_result = CANNOT_TAKE_CURRENT_COURSE;
			if (current_takable_course_result == CANNOT_TAKE_CURRENT_COURSE)
				break;
		}
	return current_takable_course_result;
}

CourseTable generate_output(CourseTable takable_courses)
{
	for (int current_takable_course = 0; current_takable_course < takable_courses.size(); current_takable_course++)
	{
		string current_takable_course_name_for_compare = takable_courses[current_takable_course].name;
		int swap_element = current_takable_course;
		for (int current_takable_course_for_compare = current_takable_course + 1; current_takable_course_for_compare < takable_courses.size(); current_takable_course_for_compare++)
			if (current_takable_course_name_for_compare > takable_courses[current_takable_course_for_compare].name)
			{
				current_takable_course_name_for_compare = takable_courses[current_takable_course_for_compare].name;
				swap_element = current_takable_course_for_compare;
			}
		takable_courses = takable_courses_swap(takable_courses, current_takable_course, swap_element);
	}
	return takable_courses;
}

CourseTable takable_courses_swap(CourseTable takable_courses, int current_takable_course, int swap_element)
{
	Course temp;
	temp.id = takable_courses[current_takable_course].id;
	temp.name = takable_courses[current_takable_course].name;
	temp.prerequisites = takable_courses[current_takable_course].prerequisites;
	temp.schedule = takable_courses[current_takable_course].schedule;
	temp.units = takable_courses[current_takable_course].units;
	takable_courses[current_takable_course] = takable_courses[swap_element];
	takable_courses[swap_element] = temp;
	return takable_courses;
}

void print_result(CourseTable takable_courses)
{
	for (int current = 0; current < takable_courses.size(); current++)
		cout << takable_courses[current].id << endl;
}