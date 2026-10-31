#include "FileSystem.h"
#include <iostream>
#include <cerrno>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include "MementoType.h"
#include "Team.h"
#include "Department.h"
#include <vector>
#include "EmployeeComponent.h"
#include "EmployeeDetail.h"
#include "EmployeeProfession.h"
#include "EmployeeCompetency.h"
#include "Employee.h"
#include "Manager.h"
#include "WorkUnit.h"
#include "WorkNode.h"
#include "Project.h"
#include "Memento.h"

static bool readLine(std::istream& in, std::string& line) {
	if (!std::getline(in, line)) {
		return false;
	}
	if (!line.empty() && line.back() == '\r') {
		line.pop_back();
	}
	return true;
}

static std::string oneLine(std::string text) {
	for (char& c : text) {
		if (c == '\n' || c == '\r') {
			c = ' ';
		}
	}
	return text;
}

static std::string::size_type lastSeparator(const std::string& path) {
	return path.find_last_of("/\\");
}

static bool hasDirectory(const std::string& path) {
	return lastSeparator(path) != std::string::npos;
}

static bool hasExtension(const std::string& path) {
	std::string::size_type separator = lastSeparator(path);
	std::string::size_type dot = path.find_last_of('.');
	if (dot == std::string::npos) {
		return false;
	}
	if (separator != std::string::npos && dot < separator) {
		return false;
	}
	return dot + 1 < path.size();
}

static std::string resolvePath(const std::string& filename) {
	std::string path = filename;
	if (!hasDirectory(path)) {
		path = "saves/" + path;
	}
	if (!hasExtension(path)) {
		path += ".txt";
	}
	return path;
}

static void makeDirectory(const std::string& directory) {
#ifdef _WIN32
	_mkdir(directory.c_str());
#else
	mkdir(directory.c_str(), 0755);
#endif
}

static void createParentDirectories(const std::string& path) {
	std::string::size_type last = lastSeparator(path);
	if (last == std::string::npos) {
		return;
	}
	for (std::string::size_type i = 0; i < last; i++) {
		bool isSeparator = path[i] == '/' || path[i] == '\\';
		if (!isSeparator || i == 0) {
			continue;
		}
		if (path[i - 1] == ':') {
			continue;
		}
		makeDirectory(path.substr(0, i));
	}
	makeDirectory(path.substr(0, last));
}

FileSystem::~FileSystem() {
	for (const auto& pair : storage) {
		delete pair.second;
	}
}

void FileSystem::saveMemento(std::string name, Memento* memento) {
	auto iter = storage.find(name);
	if (iter != storage.end() && iter->second != nullptr && iter->second != memento) {
		delete iter->second;
	}
	storage[name] = memento;
}

Memento* FileSystem::loadMemento(std::string name) {
	auto iter = storage.find(name);
	if (iter == storage.end()) {
		return nullptr;
	}
	return iter->second;
}

// for gui:
void FileSystem::saveMember(std::ostream& file, EmployeeComponent* member) {
	file << "Member\n";

	EmployeeComponent* emp = member;
	while (emp != nullptr) {
		std::string kind = emp->getKind();
		if (kind == "Profession") {
			file << "Profession\n" << oneLine(((EmployeeProfession*)emp)->professionName) << "\nEndProfession\n";
			emp = ((EmployeeDetail*)emp)->wrapped;
		}
		else if (kind == "Competency") {
			file << "Competency\n" << oneLine(((EmployeeCompetency*)emp)->getCompetency()) << "\nEndCompetency\n";
			emp = ((EmployeeDetail*)emp)->wrapped;
		}
		else if (kind == "Manager") {
			file << "Manager\n" << oneLine(emp->getName()) << "\nEndManager\n";
			break;
		}
		else {
			file << "Employee\n" << oneLine(emp->getName()) << "\nEndEmployee\n";
			break;
		}
	}

	file << "EndMember\n";
}

void FileSystem::saveCompany(std::ostream& file, CompanyNode* root) {
	if (root == nullptr) {
		return;
	}

	Team* team = dynamic_cast<Team*>(root);
	if (team != nullptr) {
		file << "Team\n";
		file << oneLine(team->getName()) << '\n';
		file << oneLine(team->getProfession()) << '\n';
		for (EmployeeComponent* emp : team->members) {
			saveMember(file, emp);
		}
		file << "EndTeam\n";
	}
	else {
		file << "Department\n";
		file << oneLine(root->getName()) << '\n';
		for (int i = 0;; i++) {
			CompanyNode* child = root->getChild(i);
			if (child == nullptr) {
				break;
			}
			saveCompany(file, child);
		}
		file << "EndDepartment\n";
	}
}

// for gui:
void FileSystem::savePrerequisites(std::ostream& file, WorkNode* node) {
	file << "Prerequisites\n";
	for (const std::string& prereq : node->getPrerequisites()) {
		file << oneLine(prereq) << '\n';
	}
	file << "EndPrerequisites\n";
}

void FileSystem::saveWorkflow(std::ostream& file, WorkNode* root) {
	if (root == nullptr) {
		return;
	}

	WorkUnit* unit = dynamic_cast<WorkUnit*>(root);
	if (unit != nullptr) {
		file << "Unit\n";
		file << oneLine(unit->getName()) << '\n';
		file << oneLine(unit->getRequiredProfession()) << '\n';
		file << unit->getStateName() << '\n';
		savePrerequisites(file, unit);
		file << "EndUnit\n";
	}
	else {
		file << "Project\n";
		file << oneLine(root->getName()) << '\n';
		file << root->getMaxRetries() << '\n';
		file << root->getStateName() << '\n';
		savePrerequisites(file, root);
		file << "Children\n";
		for (int i = 0;; i++) {
			WorkNode* child = root->getChild(i);
			if (child == nullptr) {
				break;
			}
			saveWorkflow(file, child);
		}
		file << "EndChildren\n";
		file << "EndProject\n";
	}
}

bool FileSystem::saveToFile(std::string name, std::string filename) {
	Memento* mem = loadMemento(name);
	if (mem == nullptr || mem->getRoot() == nullptr) {
		return false;
	}

	std::string path = resolvePath(filename);
	createParentDirectories(path);

	std::ofstream file(path.c_str());
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << '\n';
		return false;
	}

	switch (mem->getType()) {
	case MementoType::company:
		file << "Company\n";
		saveCompany(file, (CompanyNode*)mem->getRoot());
		break;
	case MementoType::workflow:
		file << "Workflow\n";
		saveWorkflow(file, (WorkNode*)mem->getRoot());
		break;
	default:
		return false;
	}

	file.flush();
	return file.good();
}

// for gui:
EmployeeComponent* FileSystem::loadMember(std::istream& file) {
	EmployeeComponent* head = nullptr;
	std::string line;

	while (readLine(file, line)) {
		if (line == "EndMember") {
			if (head == nullptr) {
				return nullptr;
			}
			std::string coreKind = head->getCore()->getKind();
			if (coreKind != "Employee" && coreKind != "Manager") {
				delete head;
				return nullptr;
			}
			return head;
		}

		std::string tag = line;
		std::string value;
		std::string end;
		if (!readLine(file, value) || !readLine(file, end) || end != "End" + tag) {
			std::cerr << "Error reading file, End" << tag << " not found\n";
			delete head;
			return nullptr;
		}

		EmployeeComponent* node = nullptr;
		if (tag == "Profession") {
			node = new EmployeeProfession(value);
		}
		else if (tag == "Competency") {
			node = new EmployeeCompetency(EmployeeCompetency::getCompetencyLevel(value));
		}
		else if (tag == "Employee") {
			node = new Employee(value);
		}
		else if (tag == "Manager") {
			node = new Manager(value);
		}
		else {
			delete head;
			return nullptr;
		}

		if (head == nullptr) {
			head = node;
		}
		else {
			head->wrap(node);
		}
	}

	delete head;
	return nullptr;
}

CompanyNode* FileSystem::loadCompany(std::istream& file, const std::string& tag) {
	std::string line;

	if (tag == "Department") {
		Department* department = new Department();
		if (!readLine(file, line)) {
			delete department;
			return nullptr;
		}
		department->setName(line);

		while (readLine(file, line)) {
			if (line == "EndDepartment") {
				return department;
			}
			CompanyNode* child = nullptr;
			if (line == "Department" || line == "Team") {
				child = loadCompany(file, line);
			}
			if (child == nullptr) {
				delete department;
				return nullptr;
			}
			department->add(child);
		}
		delete department;
		return nullptr;
	}

	if (tag == "Team") {
		std::string name;
		std::string profession;
		if (!readLine(file, name) || !readLine(file, profession)) {
			return nullptr;
		}
		Team* team = new Team(name, profession);

		while (readLine(file, line)) {
			if (line == "EndTeam") {
				return team;
			}
			if (line != "Member") {
				delete team;
				return nullptr;
			}
			EmployeeComponent* member = loadMember(file);
			if (member == nullptr) {
				delete team;
				return nullptr;
			}
			if (!team->addMember(member)) {
				delete member;
			}
		}
		delete team;
		return nullptr;
	}

	return nullptr;
}

// for gui:
bool FileSystem::loadPrerequisites(std::istream& file, WorkNode* node) {
	std::string line;
	if (!readLine(file, line) || line != "Prerequisites") {
		return false;
	}
	while (readLine(file, line)) {
		if (line == "EndPrerequisites") {
			return true;
		}
		node->addPrerequisite(line);
	}
	return false;
}

WorkNode* FileSystem::loadWorkflow(std::istream& file, const std::string& tag) {
	std::string line;

	if (tag == "Project") {
		std::string name;
		std::string maxRetries;
		std::string stateName;
		if (!readLine(file, name) || !readLine(file, maxRetries) || !readLine(file, stateName)) {
			return nullptr;
		}

		int retries = 3;
		try {
			retries = std::stoi(maxRetries);
		}
		catch (...) {
			return nullptr;
		}

		Project* project = new Project(retries, name);
		project->setState(stateName);

		if (!loadPrerequisites(file, project) || !readLine(file, line) || line != "Children") {
			delete project;
			return nullptr;
		}

		while (readLine(file, line)) {
			if (line == "EndChildren") {
				if (!readLine(file, line) || line != "EndProject") {
					delete project;
					return nullptr;
				}
				return project;
			}
			WorkNode* child = nullptr;
			if (line == "Project" || line == "Unit") {
				child = loadWorkflow(file, line);
			}
			if (child == nullptr) {
				delete project;
				return nullptr;
			}
			project->add(child);
		}
		delete project;
		return nullptr;
	}

	if (tag == "Unit") {
		std::string name;
		std::string required;
		if (!readLine(file, name) || !readLine(file, required)) {
			return nullptr;
		}

		WorkUnit* unit = new WorkUnit(name, required);

		if (!readLine(file, line)) {
			delete unit;
			return nullptr;
		}
		if (line == "EndUnit") {
			return unit;
		}

		unit->setState(line);
		if (!loadPrerequisites(file, unit) || !readLine(file, line) || line != "EndUnit") {
			std::cerr << "Error reading file, EndUnit not found!\n";
			delete unit;
			return nullptr;
		}
		return unit;
	}

	return nullptr;
}

bool FileSystem::loadFromFile(std::string name, std::string filename) {
	std::ifstream file(resolvePath(filename).c_str());
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << '\n';
		return false;
	}

	std::string header;
	std::string tag;
	if (!readLine(file, header) || !readLine(file, tag)) {
		return false;
	}

	Memento* mem = nullptr;
	if (header == "Company") {
		CompanyNode* root = loadCompany(file, tag);
		if (root == nullptr) {
			return false;
		}
		mem = new Memento(root, MementoType::company);
	}
	else if (header == "Workflow") {
		WorkNode* root = loadWorkflow(file, tag);
		if (root == nullptr) {
			return false;
		}
		mem = new Memento(root, MementoType::workflow);
	}
	else {
		return false;
	}

	saveMemento(name, mem);
	return true;
}
