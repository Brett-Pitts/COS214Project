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
#include "WorkUnit.h"
#include "WorkNode.h"
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
			file << "Member\n";

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


			file << "EndMember\n";
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

void FileSystem::saveWorkflow(std::ofstream& file, WorkNode* root) {

	if (root->getChild(0) == nullptr) {
		// Team
		WorkUnit* unit = (WorkUnit*)root;
		file << "Unit\n";
		file << unit->getName() << '\n';
		file << unit->getRequiredProfession() << '\n';
		file << "EndUnit\n";
	}
	else {
		// Department
		file << "Project\n";
		file << root->getName() << '\n';
		file << root->getMaxRetries() << '\n';
		file << root->getStateName() << '\n';
		file << "Prerequisites\n";

		for (std::string prereq : root->prerequisites) {
			file << prereq << '\n';
		}

		file << "EndPrerequisites\n";
		file << "Children\n";


		for (int i = 0;;i++) {
			if (root->getChild(i) == nullptr) {
				break;
			}
			saveWorkflow(file, root->getChild(i));
		}
		file << "EndChildren\n";

		file << "EndProject\n";

	}


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
}

void FileSystem::loadCompany(std::ifstream &file, CompanyNode *&root)
{
	std::string line;
	std::getline(file, line);

	if (line == "EndDepartment"){
		return;
		}

	if (line == "Department"){
		root = (CompanyNode*)new Department();

		CompanyNode* child = nullptr;
		do{
			child = nullptr;
			
			loadCompany(file, child);
			if (child != nullptr){
				root->add(child);
			}
		} while(child != nullptr);


		
		
	}

	else if (line == "Team"){
		Team* team = new Team();
		root = (CompanyNode*)team;

		while (std::getline(file, line)){
			if (line == "EndTeam"){
				break;				
			}

			if (line == "Member"){
				EmployeeComponent* member = nullptr;
				EmployeeComponent* temp = nullptr;
				while (std::getline(file, line)){
					if (line == "EndMember"){
						break;
					}
					if (line == "Profession"){
						std::getline(file, line);
						temp = new EmployeeProfession(line);
						std::getline(file, line);
						if (line != "EndProfession"){
							std::cerr << "Error reading file, EndProfession not found\n";
						}
					}
					else if (line == "Competency"){
						std::getline(file, line);
						temp = new EmployeeCompetency(line);
						std::getline(file, line);
						if (line != "EndCompetency"){
							std::cerr << "Error reading file, EndCompetency not found\n";
						}
					}
					else if (line == "Employee"){
						std::getline(file, line);
						temp = new Employee(line);
						std::getline(file, line);
						if (line != "EndEmployee"){
							std::cerr << "Error reading file, EndEmployee not found\n";
						}
					}
					else if (line == "Manager"){
						std::getline(file, line);
						temp = new Manager(line);
						std::getline(file, line);
						if (line != "EndManager"){
							std::cerr << "Error reading file, EndManager not found\n";
						}
					}
					if (member == nullptr){
						member = temp;
					}else{
						member->wrap(temp);
					}
					temp = nullptr;
				}
				team->addMember(member);


			}
		}

		

	}




}

void FileSystem::loadWorkflow(std::ifstream &file, WorkNode *&root)
{

	std::string line;
	std::getline(file, line);

	if (line == "EndProject" || line == "EndChildren"){
		return;
	}

	if (line == "Project"){
		std::string name;
		std::string maxRetries;
		std::string StateName;
		std::getline(file, name);
		std::getline(file, maxRetries);
		std::getline(file, StateName);

		root = (WorkNode*)new Project(std::stoi(maxRetries), name);
		root->setState(StateName);

		std::getline(file, line);
		if (line == "Prerequisites"){
			while (std::getline(file, line)){
				if (line == "EndPrerequisites"){
					break;
				}
				root->addPrerequisite(line);
			}
		}
		std::getline(file, line);
		if (line == "Children"){

			
			WorkNode* child = nullptr;
			do{
				child = nullptr;
				
				loadWorkflow(file, child);
				if (child != nullptr){
					root->add(child);
				}
			} while(child != nullptr);
			
		}

		
		
	}

	else if (line == "Unit"){

		std::string name;
		std::string required;

		std::getline(file, name);
		std::getline(file, required);


		WorkUnit* unit = new WorkUnit(name, required);
		root = (WorkNode*)unit;

		std::getline(file, line);
		if (line != "EndUnit"){
			std::cerr << "Error reading file, EndUnit not found!\n";
		}

		

	}





}

void FileSystem::loadFromFile(std::string name, std::string filename) {

	Memento* mem = new Memento(nullptr, workflow);

	std::ifstream file("saves/" + filename + ".txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << '\n';
		return;
	}
	file.clear();
	MementoType type;
	std::string line;

	std::getline(file, line);

	if (line == "Company"){
		type = company;
		CompanyNode* root;
		loadCompany(file, root);
		mem->setRoot(root, type);
		
	}else if (line == "Workflow"){
		type = workflow;
		WorkNode* root;
		loadWorkflow(file, root);
		mem->setRoot(root, type);
	}else{
		return;
	}
	saveMemento(name, mem);
}
