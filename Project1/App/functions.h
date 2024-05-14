#pragma once
#include <iostream>
#include <string>
class prisData {
public:
	std::vector<std::string> PN;
	std::vector<std::string> Name;
	std::vector<std::string> Age;
	std::vector<std::string> Gender;
	std::vector<std::string> Sentence;
	std::vector<std::string> Crime;
};
prisData readPrisCSV();
prisData search(int PN, std::string name, int age, std::string gender, int sentence, std::string crime);
void addPrisoner(std::string name, int age, std::string gender, int sentence, std::string crime);
bool editPrisoner(int PN, std::string name, int age, std::string gender, int sentence, std::string crime);
bool deletePrisoner(int PN);