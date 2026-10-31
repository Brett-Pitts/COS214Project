#include "FileSystem.h"
#include <iostream>
#include <exception>
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


// Reads one line and strips a trailing '\r' so files saved on Windows load on Linux.
static bool readLine(std::istream& file, std::string& line) {
	if (!std::getline(file, line)) {
		return false;
	}
	if (!line.empty() && line[line.size() - 1] == '\r') {
		line.erase(line.size() - 1);
	}
	return true;
}

static void writePrerequisites(std::ostream& file, WorkNode* node) {
	file << "Prerequisites\n";
	for (const std::string& prereq : node->getPrerequisites()) {
		file << prereq << '\n';
	}
	file << "EndPrerequisites\n";
}

// Expects the "Prerequisites" line, then reads names until "EndPrerequisites".
static void readPrerequisites(std::istream& file, WorkNode* node) {
	std::string line;
	if (!readLine(file, line) || line != "Prerequisites") {
		std::cerr << "Error reading file, Prerequisites not found\n";
		return;
	}
	while (readLine(file, line)) {
		if (line == "EndPrerequisites") {
			break;
		}
		node->addPrerequisite(line);
	}
}

// Reads one decorator or core part of an employee (the line after the tag is the
// name, followed by the matching End line). Returns nullptr for an unknown tag.
static EmployeeComponent* readEmployeePart(std::istream& file, const std::string& tag) {
	std::string value;
	std::string end;
	EmployeeComponent* part = nullptr;

	if (tag != "Profession" && tag != "Competency" && tag != "Employee" && tag != "Manager") {
		return nullptr;
	}
	if (!readLine(file, value)) {
		return nullptr;
	}

	if (tag == "Profession") {
		part = new EmployeeProfession(value);
	}
	else if (tag == "Competency") {
		part = new EmployeeCompetency(value);
	}
	else if (tag == "Employee") {
		part = new Employee(value);
	}
	else {
		part = new Manager(value);
	}

	if (!readLine(file, end) || end != "End" + tag) {
		std::cerr << "Error reading file, End" << tag << " not found\n";
	}
	return part;
}

// ---------------------------------------------------------------------------
// Storage of mementos
// ---------------------------------------------------------------------------

FileSystem::~FileSystem() {
	for (const auto& pair : storage) {
		delete pair.second;
	}
}

void FileSystem::saveMemento(std::string name, Memento* memento) {
	std::map<std::string, Memento*>::iterator it = storage.find(name);
	if (it != storage.end()) {
		if (it->second == memento) {
			return; // Same memento saved again: deleting it would leave a dangling pointer.
		}
		delete it->second;
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

// ---------------------------------------------------------------------------
// Saving
// ---------------------------------------------------------------------------

void FileSystem::saveCompany(std::ofstream& file, CompanyNode* root) {
	if (root == nullptr) {
		return;
	}

	Team* team = dynamic_cast<Team*>(root);
	if (team != nullptr) {
		file << "Team\n";
		file << team->profession << '\n';

		for (EmployeeComponent* emp : team->members) {
			file << "Member\n";

			// Walk from the outermost decorator down to the core, writing each part.
			EmployeeComponent* current = emp;
			while (current != nullptr) {
				std::string kind = current->getKind();

				if (kind == "Profession") {
					file << "Profession\n";
					file << static_cast<EmployeeProfession*>(current)->professionName << '\n';
					file << "EndProfession\n";
					current = static_cast<EmployeeDetail*>(current)->wrapped;
				}
				else if (kind == "Competency") {
					file << "Competency\n";
					file << static_cast<EmployeeCompetency*>(current)->competencyName << '\n';
					file << "EndCompetency\n";
					current = static_cast<EmployeeDetail*>(current)->wrapped;
				}
				else if (kind == "Employee") {
					file << "Employee\n";
					file << current->getName() << '\n';
					file << "EndEmployee\n";
					break;
				}
				else if (kind == "Manager") {
					file << "Manager\n";
					file << current->getName() << '\n';
					file << "EndManager\n";
					break;
				}
				else {
					break;
				}
			}

			file << "EndMember\n";
		}
		file << "EndTeam\n";
	}
	else {
		// Department (this also handles an empty department correctly)
		file << "Department\n";
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

void FileSystem::saveWorkflow(std::ofstream& file, WorkNode* root) {
	if (root == nullptr) {
		return;
	}

	WorkUnit* unit = dynamic_cast<WorkUnit*>(root);
	if (unit != nullptr) {
		file << "Unit\n";
		file << unit->getName() << '\n';
		file << unit->getRequiredProfession() << '\n';
		file << unit->getStateName() << '\n';
		writePrerequisites(file, unit);
		file << "EndUnit\n";
	}
	else {
		// Project (this also handles an empty project correctly)
		file << "Project\n";
		file << root->getName() << '\n';
		file << root->getMaxRetries() << '\n';
		file << root->getStateName() << '\n';
		writePrerequisites(file, root);

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

void FileSystem::saveToFile(std::string name, std::string filename) {

	Memento* mem = loadMemento(name);
	if (mem == nullptr || mem->getRoot() == nullptr) {
		std::cerr << "Nothing to save under the name: " << name << '\n';
		return;
	}



	std::ofstream file("saves/" + filename + ".tvd");
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << '\n';
		return;
	}

	switch (mem->getType()) {
	case (MementoType::company):
	{
		file << "Company\n";
		CompanyNode* root = (CompanyNode*)mem->getRoot();
		saveCompany(file, root);
		break;
	}
	case (MementoType::workflow):
	{
		file << "Workflow\n";
		WorkNode* root = (WorkNode*)mem->getRoot();
		saveWorkflow(file, root);
		break;
	}
	default:
		break;
	}

	if (!file) {
		std::cerr << "Error while writing file: " << filename << '\n';
	}
}

// ---------------------------------------------------------------------------
// Loading
// ---------------------------------------------------------------------------

void FileSystem::loadCompany(std::ifstream& file, CompanyNode*& root) {
	std::string line;
	if (!readLine(file, line)) {
		return;
	}

	if (line == "EndDepartment") {
		return;
	}

	if (line == "Department") {
		Department* department = new Department();

		while (true) {
			CompanyNode* child = nullptr;
			loadCompany(file, child);
			if (child == nullptr) {
				break;
			}
			department->add(child);
		}
		root = department;
	}
	else if (line == "Team") {

		std::string profession;
		std::streampos position = file.tellg();
		if (!readLine(file, profession)) {
			return;
		}
		if (profession == "Member" || profession == "EndTeam") {
			file.clear();
			file.seekg(position);
			profession = "";
		}

		Team* team = new Team(profession);

		while (readLine(file, line)) {
			if (line == "EndTeam") {
				break;
			}

			if (line == "Member") {
				EmployeeComponent* member = nullptr;

				while (readLine(file, line)) {
					if (line == "EndMember") {
						break;
					}
					EmployeeComponent* part = readEmployeePart(file, line);
					if (part == nullptr) {
						continue;
					}
					if (member == nullptr) {
						member = part;
					}
					else {
						member->wrap(part);
					}
				}

				if (member != nullptr && !team->addMember(member)) {
					std::cerr << "Could not add member " << member->getName() << " to the team\n";
					delete member;
				}
			}
		}
		root = team;
	}
}

void FileSystem::loadWorkflow(std::ifstream& file, WorkNode*& root) {

	std::string line;
	if (!readLine(file, line)) {
		return;
	}

	if (line == "EndProject" || line == "EndChildren") {
		return;
	}

	if (line == "Project") {
		std::string name;
		std::string maxRetries;
		std::string stateName;
		if (!readLine(file, name) || !readLine(file, maxRetries) || !readLine(file, stateName)) {
			std::cerr << "Error reading file, project header incomplete\n";
			return;
		}

		int retries = 0;
		try {
			retries = std::stoi(maxRetries);
		}
		catch (const std::exception&) {
			std::cerr << "Error reading file, bad retry count: " << maxRetries << '\n';
			return;
		}

		Project* project = new Project(retries, name);
		project->setState(stateName);
		readPrerequisites(file, project);

		if (readLine(file, line) && line == "Children") {
			while (true) {
				WorkNode* child = nullptr;
				loadWorkflow(file, child);
				if (child == nullptr) {
					break; // "EndChildren" (consumed above) or an error
				}
				project->add(child);
			}

			// Consume this project's own end marker so the parent's loop stays in step.
			if (!readLine(file, line) || line != "EndProject") {
				std::cerr << "Error reading file, EndProject not found!\n";
			}
		}
		else {
			std::cerr << "Error reading file, Children not found!\n";
		}
		root = project;
	}

	else if (line == "Unit") {

		std::string name;
		std::string required;
		if (!readLine(file, name) || !readLine(file, required)) {
			std::cerr << "Error reading file, unit incomplete\n";
			return;
		}

		WorkUnit* unit = new WorkUnit(name, required);

		// Newer files store the state and prerequisites; older ones go straight to EndUnit.
		if (!readLine(file, line)) {
			std::cerr << "Error reading file, EndUnit not found!\n";
		}
		else if (line != "EndUnit") {
			unit->setState(line);
			readPrerequisites(file, unit);
			if (!readLine(file, line) || line != "EndUnit") {
				std::cerr << "Error reading file, EndUnit not found!\n";
			}
		}
		root = unit;
	}
}

void FileSystem::loadFromFile(std::string name, std::string filename) {

	std::ifstream file("saves/" + filename + ".tvd");
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << '\n';
		return;
	}

	std::string line;
	if (!readLine(file, line)) {
		std::cerr << "File is empty: " << filename << '\n';
		return;
	}

	Memento* mem = nullptr;

	if (line == "Company") {
		CompanyNode* root = nullptr;
		loadCompany(file, root);
		if (root == nullptr) {
			std::cerr << "Could not read a company from: " << filename << '\n';
			return;
		}
		mem = new Memento(root, MementoType::company);
	}
	else if (line == "Workflow") {
		WorkNode* root = nullptr;
		loadWorkflow(file, root);
		if (root == nullptr) {
			std::cerr << "Could not read a workflow from: " << filename << '\n';
			return;
		}
		mem = new Memento(root, MementoType::workflow);
	}
	else {
		std::cerr << "Unknown file type in: " << filename << '\n';
		return;
	}

	saveMemento(name, mem);
}
