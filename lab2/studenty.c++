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

	auto find_ (const std::string& name) const {
		auto it = std::ranges::partition_point(by_name, [&](const StudentWithPos* student) {return student->first.name() < name;});

		
		return std::make_pair(it != by_name.end() && (*it)->first.name() == name, it);
	}


	auto find_pos_(const StudentWithPos* s) {
		return std::ranges::partition_point(by_pos, [&](const StudentWithPos* x) {return x->second < s->second;});
	}

public:

	StudentDatabase() = default;


	StudentDatabase(const StudentDatabase&) = delete;
	StudentDatabase& operator = (const StudentDatabase&) = delete;

	~StudentDatabase() {
		for(auto student : by_name) delete student;
	}

	size_t getStudentsSize() const
	{
		return by_name.size();
	}

	bool addStudent(const Student & s)
	{
		auto [succ, it] = find_(s.name());
		if(succ) return false;

		auto new_student = new StudentWithPos{s, last++};
		by_name.insert(it, new_student);
		by_pos.push_back(new_student);
		return true;
	}

	const Student* findStudent(const std::string &name) const
	{
		auto [succ, it] = find_(name);
		return succ ? &(*it)->first : nullptr;
	}

	bool deleteStudent(const std::string &name)
	{
		auto [succ ,by_name_it] = find_(name);
		if (!succ) return false;
		by_pos.erase(find_pos_(*by_name_it));
		delete *by_name_it;
		by_name.erase(by_name_it);
		return true;
	}

	void print(std::ostream& out) const {
		out << std::setprecision(5);
		for (auto&& s : by_pos) out << s->first << "\n";
	  }
	

private:
	size_t last = 0;
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