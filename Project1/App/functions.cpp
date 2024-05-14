#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>

const char* PrisonerCSVfile = "data.csv";
const char* appCSVfile = "app.csv";
class prisData {
public:
	std::vector<std::string> PN;
	std::vector<std::string> Name;
	std::vector<std::string> Age;
	std::vector<std::string> Gender;
	std::vector<std::string> Sentence;
	std::vector<std::string> Crime;
};

class appData {
public:
	std::vector<std::string> emptyPN;
	std::map<std::string, std::string> Users;
};

// basic function
std::vector<std::string> splitString(const std::string& str, char delimiter) {
	std::vector<std::string> result;
	std::size_t start = 0;
	std::size_t end = str.find(delimiter);
	while (end != std::string::npos) {
		result.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delimiter, start);
	}
	result.push_back(str.substr(start));
	return result;
}

//main functions

prisData readPrisCSV() {
	std::vector<std::string> prisoners;
	std::string record;
	std::ifstream data;
	int j = 0;
	prisData readdata;
	data.open(PrisonerCSVfile);
	while (data.peek() != EOF)
	{
		getline(data, record);
		std::vector<std::string>splited = splitString(record, ',');
		for (unsigned int i = 0; i < splited.size(); i++) {
			switch (i) {
			case 0:
				(readdata.PN).push_back(splited[i]);
				break;
			case 1:
				(readdata.Name).push_back(splited[i]);
				break;
			case 2:
				(readdata.Age).push_back(splited[i]);
				break;
			case 3:
				(readdata.Gender).push_back(splited[i]);
				break;
			case 4:
				(readdata.Sentence).push_back(splited[i]);
				break;
			case 5:
				(readdata.Crime).push_back(splited[i]);
				break;
			}
		}
	}
	return readdata;
}

void writePrisCSV(prisData tempData) {
	std::ofstream file(PrisonerCSVfile);
	std::vector<std::vector<std::string>> tempDataVec;
	for (unsigned int i = 0; i < (tempData.PN).size(); i++) {
		std::vector<std::string> tempLine = { tempData.PN[i], tempData.Name[i],tempData.Age[i],tempData.Gender[i],tempData.Sentence[i],tempData.Crime[i] };
		tempDataVec.push_back(tempLine);
	}
	for (const auto& row : tempDataVec) {
		for (const auto& cell : row) {
			file << cell << ",";
		}
			file << std::endl;
	}
	file.close();

}

appData readAppCSV() {
	std::string record;
	appData readData;
	std::ifstream data;
	data.open(appCSVfile);
	for (int j = 0; j < 2; j++) {
		getline(data, record);
		switch (j) {
		case 0:
			readData.emptyPN = splitString(record, ',');
		case 1:
			std::vector<std::string> UP = splitString(record, ',');
			for (int i = 0; i < UP.size(); i++) {
				std::vector<std::string> TempUP = splitString(UP[i], ':');
				readData.Users.insert({TempUP[0],TempUP[1]});
			}
		}
	}
}

void writeAppCSV(appData tempData) {
	std::ofstream file(appCSVfile);
	std::vector<std::vector<std::string>> tempDataVec;
	std::vector<std::string> users;
	tempDataVec.push_back(tempData.emptyPN);
	for (auto it: tempData.Users) {
		std::string tempPU = it.first+":"+ it.second;
		users.push_back(tempPU);
	}
	for (const auto& row : tempDataVec) {
		for (const auto& cell : row) {
			file << cell << ",";
		}
		file << std::endl;
	}
	file.close();
}

prisData search(int PN,std::string name, int age, std::string gender, int sentence, std::string crime) {
	prisData Data = readPrisCSV();
	prisData filteredData;
	std::string strAge = std::to_string(age);
	std::string strSentence = std::to_string(sentence);
	std::vector<std::string> filter = { name, strAge, gender, strSentence, crime };
	if (PN == 0 && filter[0] == "" && filter[1] == "0" && filter[2] == "Select" && filter[3] == "0" && filter[4] == "Select") {
		return Data;
	}
	else if ( PN > Data.PN.size()) {
		return filteredData;
	}
	else if (PN != 0 && PN <= Data.PN.size()) {
		if (filter[0] != Data.Name[PN - 1] && filter[0] != "") {
		}
		else if (filter[1] != Data.Age[PN - 1] && filter[1] != "0") {
		}
		else if (filter[2] != Data.Gender[PN - 1] && filter[2] != "Select") {
		}
		else if (filter[3] != Data.Sentence[PN - 1] && filter[3] != "0") {
		}
		else if (filter[4] != Data.Crime[PN - 1] && filter[4] != "Select") {
		}
		else {
			filteredData.PN.push_back(Data.PN[PN - 1]);
			filteredData.Name.push_back(Data.Name[PN - 1]);
			filteredData.Age.push_back(Data.Age[PN - 1]);
			filteredData.Gender.push_back(Data.Gender[PN - 1]);
			filteredData.Sentence.push_back(Data.Sentence[PN - 1]);
			filteredData.Crime.push_back(Data.Crime[PN - 1]);
		}
		return filteredData;
	}
	for (unsigned int i = 0; i < (Data.PN).size(); i++) {
		if (filter[0] != Data.Name[i] && filter[0] != "") {
		}
		else if (filter[1] != Data.Age[i] && filter[1] != "0") {
		}
		else if (filter[2] != Data.Gender[i] && filter[2] != "Select") {
		}
		else if (filter[3] != Data.Sentence[i] && filter[3] != "0") {
		}
		else if (filter[4] != Data.Crime[i] && filter[4] != "Select") {
		}
		else {
			filteredData.PN.push_back(Data.PN[i]);
			filteredData.Name.push_back(Data.Name[i]);
			filteredData.Age.push_back(Data.Age[i]);
			filteredData.Gender.push_back(Data.Gender[i]);
			filteredData.Sentence.push_back(Data.Sentence[i]);
			filteredData.Crime.push_back(Data.Crime[i]);
		}
	}
	return filteredData;
}

void addPrisoner(std::string name, int age, std::string gender, int sentence, std::string crime) {
	bool flag = true;
	prisData Data = readPrisCSV();
	std::string strAge = std::to_string(age);
	std::string strSentence = std::to_string(sentence);

	for (unsigned int i = 0; i < Data.PN.size(); i++) {
		if (Data.Name[i] == "") {
			flag = false;
			Data.Name[i] = name;
			Data.Age[i] = strAge;
			Data.Gender[i] = gender;
			Data.Sentence[i] = strSentence;
			Data.Crime[i] = crime;
			break;
		}
	}
	if (flag == true)
	{
		std::string lastPN = std::to_string((Data.PN).size() + 1);
		Data.PN.push_back(lastPN);
		Data.Name.push_back(name);
		Data.Age.push_back(strAge);
		Data.Gender.push_back(gender);
		Data.Sentence.push_back(strSentence);
		Data.Crime.push_back(crime);
		}
	writePrisCSV(Data);
}

bool editPrisoner(int PN, std::string name, int age, std::string gender, int sentence, std::string crime) {
	prisData Data = readPrisCSV();
	std::string strAge = std::to_string(age);
	std::string strSentence = std::to_string(sentence);
	if (PN != 0 && PN <= Data.PN.size()) {
		int index = PN - 1;
		if (name != "") {
			Data.Name[index] = name;
		}
		if (age != 0) {
			Data.Age[index] = strAge;
		}
		if (gender != "Select") {
			Data.Gender[index] = gender;
		}
		if (strSentence != "0") {
			Data.Sentence[index] = strSentence;
		}
		if (crime != "Select") {
			Data.Crime[index] = crime;
		}
		writePrisCSV(Data);
		return true;
	}
	return false;
}

bool deletePrisoner(int PN) {
	prisData Data = readPrisCSV();
	if (PN <= Data.PN.size() && PN != 0) {
		Data.Name[PN - 1] = "";
		Data.Age[PN - 1] = "";
		Data.Gender[PN - 1] = "";
		Data.Sentence[PN - 1] = "";
		Data.Crime[PN - 1] = "";
		writePrisCSV(Data);
		return true;
	}
	return false;
}
