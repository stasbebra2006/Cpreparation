#ifndef __TRAINER__
#include <string>
#include <vector>
#include <cassert>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#endif

class Student
{
public:
	Student(const std::string &name, double average) : name_(name), average_(average) {}

	const std::string &name() const
	{
		return name_;
	}

	double average() const
	{
		return average_;
	}

	friend std::ostream &operator<<(std::ostream &out, const Student &student)
	{
		return out << "Student { " << student.name_ << ", " << student.average_ << " }";
	}

private:
	std::string name_;
	double average_;
};

class StudentDatabase
{
	using StudentWithPos = std::pair<Student, size_t>;


public:

	StudentDatabase() = default;


	StudentDatabase(const StudentDatabase&) = delete;
	StudentDatabase& operator = (const StudentDatabase&) = delete;

	size_t getStudentsSize() const
	{
		return students_.size();
	}

	bool addStudent(const Student &student)
	{
		for (auto &existingStudent : students_)
		{
			if (existingStudent.name() == student.name())
			{
				return false;
			}
		}
		students_.push_back(student);
		return true;
	}

	const Student *findStudent(const std::string &name) const
	{
		for (auto &existingStudent : students_)
		{
			if (existingStudent.name() == name)
			{
				return &existingStudent;
			}
		}
		return nullptr;
	}

	bool deleteStudent(const std::string &name)
	{
		for (auto e_student = students_.begin(); e_student != students_.end(); e_student++)
		{
			if (e_student->name() == name)
			{
				students_.erase(e_student);
				return true;
			}
		}
		return false;
	}

	void print(std::ostream &out) const
	{
		// 	for (const auto &student : students_)
		// 	{
		// 		os << "Student { " << student.name() << ", " << std::setprecision(5) << student.average() << " }\n";
		// 	}
		out << std::setprecision(5);
		for (const auto student : students_)
			out << student << std::endl;
	}

private:
	std::vector<StudentWithPos*> by_name;
	std::vector<StudentWithPos*> by_pos;
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

	assert(!db.addStudent({"John Doe", 2.0}));

	std::vector<std::pair<Student, bool>> testFindData = {{{"John Doe", 2.4}, true}, {{"John Doee", 2.4}, false}, {{"Jan Novak II", 1.0 + 1e-12}, true}};

	for (const auto &findData : testFindData)
	{
		const Student *foundStudent = db.findStudent(findData.first.name());
		assert((foundStudent != nullptr) == findData.second);
		if (findData.second)
		{
			assert(foundStudent->name() == findData.first.name());
			assert(foundStudent->average() == findData.first.average());
		}
	}

	const StudentDatabase &constDb = db;

	std::ostringstream oss;
	constDb.print(oss);
	std::string expectedPrint = "Student { John Doe, 2.4 }\nStudent { abc, 3.4901 }\nStudent { Jan Novak II, 1 }\nStudent { $(!+, 4 }\nStudent { , 1.6667 }\n";
	std::string actualPrint = oss.str();
	assert(actualPrint == expectedPrint);

	assert(constDb.getStudentsSize() == testStudentData.size());

	std::vector<std::pair<std::string, bool>> testDeleteData = {{"John Doe", true}, {"John Doee", false}, {"Jan Novak II", true}};

	for (const auto &deleteData : testDeleteData)
	{
		assert(db.deleteStudent(deleteData.first) == deleteData.second);
	}

	oss.str("");
	oss.clear();
	constDb.print(oss);
	expectedPrint = "Student { abc, 3.4901 }\nStudent { $(!+, 4 }\nStudent { , 1.6667 }\n";
	actualPrint = oss.str();
	assert(actualPrint == expectedPrint);

	return 0;
}
#endif