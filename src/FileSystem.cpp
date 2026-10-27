#include "FileSystem.h"
#include <iostream>
#include <fstream>
#include "MementoType.h"
#include "Team.h"
#include "Department.h"
#include <vector>
#include "EmployeeComponent.h"
#include "EmployeeProfession.h"
#include "EmployeeCompetency.h"
#include "Employee.h"
#include "Manager.h"
FileSystem::~FileSystem() {
	for (const auto& pair : storage) {
		delete pair.second;
	}


}
void FileSystem::saveMemento(std::string name, Memento* memento) {
	if (storage.find(name) != storage.end()) {

	}
	storage[name] = memento;
}

Memento* FileSystem::loadMemento(std::string name) {
	auto iter = storage.find(name);
	if (iter == storage.end()) {
		return nullptr;
	}
	return storage[name];
}

void FileSystem::saveCompany(std::ofstream& file, CompanyNode* root) {
	if (root->getChild(0) == nullptr) {
		// Team
		Team* team = (Team*)root;
		file << "Team\n";
		for (EmployeeComponent* emp : team->members) {
			file << "Employee\n";

			std::string kind = emp->getKind();
			while (kind != "Employee" || kind != "Manager") {

				if (kind == "Profession") {
					file << "Profession\n";
					file << ((EmployeeProfession*)emp)->professionId;
					file << "EndProfession\n";
				}
				else if (kind == "Competency") {
					file << "Competency\n";
					file << ((EmployeeCompetency*)emp)->competencyId;
					file << "EndCompetency\n";
				}
				emp = ((EmployeeCompetency*)emp)->wrapped;
				kind = emp->getKind();
			}

			// Employee or Manager
			if (kind == "Employee") {
				file << "Employee\n";
				file << emp->getName();
				file << "EndEmployee\n";
			}
			else {
				file << "Manager\n";
				file << emp->getName();
				file << "EndManager\n";
			}


			file << "EndEmployee\n";
		}
		file << "EndTeam\n";
	}
	else {
		// Department
		file << "Department\n";
		for (int i = 0;;i++) {
			if (root->getChild(i) == nullptr) {
				break;
			}
			saveCompany(file, root->getChild(i));
		}
		file << "EndDepartment\n";

	}
}

void FileSystem::saveProject(std::ofstream& file, WorkNode* root) {




}


void FileSystem::saveToFile(std::string name, std::string filename) {

	Memento* mem = loadMemento(name);
	if (mem == nullptr) {
		return;
	}

	std::ofstream file("saves/" + filename + ".txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << '\n';
		return;
	}

	file.clear();

	switch (mem->getType()) {
	case (MementoType::company):
	{
		file << "company\n";
		CompanyNode* root = (CompanyNode*)mem->getRoot();

		saveCompany(file, root);



		break;
	}
	case (MementoType::workflow):
	{


		break;
	}

	default:
		break;
	}



}

void FileSystem::loadFromFile(std::string name, std::string filename) {}
