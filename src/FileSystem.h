#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>

class Memento;
class CompanyNode;
class WorkNode;
class EmployeeComponent;

class FileSystem {
private:
	std::map<std::string, Memento*> storage;

	void saveCompany(std::ostream& file, CompanyNode* root);
	// for gui:
	void saveMember(std::ostream& file, EmployeeComponent* member);
	void saveWorkflow(std::ostream& file, WorkNode* root);
	// for gui:
	void savePrerequisites(std::ostream& file, WorkNode* node);

	CompanyNode* loadCompany(std::istream& file, const std::string& tag);
	// for gui:
	EmployeeComponent* loadMember(std::istream& file);
	WorkNode* loadWorkflow(std::istream& file, const std::string& tag);
	// for gui:
	bool loadPrerequisites(std::istream& file, WorkNode* node);

public:
	~FileSystem();
	void saveMemento(std::string name, Memento* memento);

	Memento* loadMemento(std::string name);

	bool saveToFile(std::string name, std::string filename);
	bool loadFromFile(std::string name, std::string filename);
};

#endif
