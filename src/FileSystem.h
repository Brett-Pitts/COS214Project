#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <map>
#include <fstream>

class Memento;
class CompanyNode;
class WorkNode;

class FileSystem {
private:
	std::map<std::string, Memento*> storage;
	void saveCompany(std::ofstream& file, CompanyNode* root);
	void saveWorkflow(std::ofstream& file, WorkNode* root);
	void loadCompany(std::ifstream& file, CompanyNode*& root);
	void loadWorkflow(std::ifstream& file, WorkNode*& root);
public:
	~FileSystem();
	void saveMemento(std::string name, Memento* memento);

	Memento* loadMemento(std::string name);

	void saveToFile(std::string name, std::string filename);
	void loadFromFile(std::string name, std::string filename);
};

#endif
