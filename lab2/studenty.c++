#ifndef __TRAINER__
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>
#endif

class Student
{
public:
	Student(const std::string &name, double average) : name_(name), average_(average) {}

	std::string name() const
	{
		return name_;
	}

	double average() const
	{
		return average_;
	}

private:
	std::string name_;
	double average_;
};

class StudentDatabase
{
public:
	bool addStudent(const Student &student)
	{
		students_.push_back(student);
		return true;
	}

	size_t getStudentsSize() const
	{
		return students_.size();
	}

	void print(std::ostream &os) const
	{
		for (const auto &student : students_)
		{
			os << "Student { " << student.name() << ", " << std::setprecision(5) << student.average() << " }\n";
		}
	}

private:
	std::vector<Student> students_;
};

#ifndef __TRAINER__
int main()
{
	std::string name = "John Doe";
	double average = 2.4;
	const Student st1(name, average);

	assert(st1.name() == name);
	assert(st1.average() == average);

	StudentDatabase db;
	assert((int)db.getStudentsSize() == 0);

	std::vector<Student> expectedStudents;
	std::vector<std::pair<std::string, double>> testStudentData = {{"John Doe", 2.4}, {"abc", 3.4901}, {"Jan Novak II", 1.0 + 1e-12}, {"$(!+", 4.0}, {"", 1.66666666}};

	for (const auto &testData : testStudentData)
	{
		expectedStudents.push_back({testData.first, testData.second});
		assert(db.addStudent({testData.first, testData.second}));
	}

	assert(db.getStudentsSize() == testStudentData.size());
	const StudentDatabase constDb = db;

	std::ostringstream oss;
	constDb.print(oss);
	std::string expectedPrint = "Student { John Doe, 2.4 }\nStudent { abc, 3.4901 }\nStudent { Jan Novak II, 1 }\nStudent { $(!+, 4 }\nStudent { , 1.6667 }\n";
	std::string actualPrint = oss.str();
	// std::cout << "actualPrint: " << std::endl << actualPrint << std::endl;
	// std::cout << "expectedPrint" << std::endl << expectedPrint << std::endl; 
	assert(actualPrint == expectedPrint);

	assert(constDb.getStudentsSize() == testStudentData.size());

	return 0;
}
#endif
