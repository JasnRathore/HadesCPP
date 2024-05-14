#include "../imgui/imgui.h"
#include <iostream>
#include <vector>
#include "functions.h"
#include <map>

std::map <std::string, bool> DisplaySwitches= { 
    {"showAddPanel", false},
    {"showDisplayPanel", false},
    {"showEditPanel", false},
    {"showDeletePanel", false},
    {"showDeleteRangePanel", false}
};

int age = 0;
int sentence = 0;
static char bufName[50] = "";
const char* Genders[] = { "Select", "Male", "Female" };
static int selectedGender = 0;
const char* Crimes[] = {"Select", "AggravatedAssault", "InsuranceFraud", "AidingandAbetting/Accessory", "Kidnapping", "Arson", "Manslaughter:Involuntary", "Assault/Battery", "Manslaughter:Voluntary", "MedicalMarijuana", "Bribery", "MIP:AMinorinPossession", "Burglary", "MoneyLaundering", "ChildAbandonment", "Murder:First-degree", "ChildAbuse", "Murder:Second-degree", "CyberCrime", "Perjury", "Conspiracy", "ProbationViolation", "Credit/DebitCardFraud", "Prostitution", "CriminalContemptofCourt", "PublicIntoxication", "Cyberbullying", "PyramidSchemes", "DisorderlyConduct", "Racketeering/RICO", "DisturbingthePeace", "Rape", "DomesticViolence", "Robbery", "DrugManufacturingandCultivation", "SecuritiesFraud", "DrugPossession", "SexualAssault", "Drug", "Trafficking/Distribution", "Shoplifting", "DUI/DWI", "Solicitation", "Embezzlement", "Stalking", "Extortion", "StatutoryRape", "Forgery", "TaxEvasion/Fraud", "Fraud", "Harassment", "Theft", "HateCrimes", "Vandalism", "Homicide", "WhiteCollarCrimes", "IdentityTheft", "WireFraud", "IndecentExposure" };
const char* headers[] = { "PN", "Name", "Age", "Gender", "Sentence", "Crime" };
static int selectedCrime = 0;

int searchPN = 0;
int searchAge = 0;
int searchSentence = 0;
static int searchGender = 0;
static int searchCrime = 0;
static char searchBufName[50] = "";

int editPN = 0;
int editAge = 0;
int editSentence = 0;
static int editGender = 0;
static int editCrime = 0;
static char editBufName[50] = "";

bool editflag = true;
bool deleteflag = true;

int deletePN = 0;


void addPanel() {
    ImGui::BeginChild("Add prisoner", ImVec2(400, 250), true);
    ImGui::Text("Name");
    ImGui::InputText("##Name", bufName, sizeof(bufName));
    ImGui::Text("Age");
    ImGui::InputInt("##Age", &age);
    ImGui::Text("Gender");
    ImGui::Combo("##Gender", &selectedGender, Genders, IM_ARRAYSIZE(Genders));
    ImGui::Text("Sentence");
    ImGui::InputInt("##Sentence", &sentence);
    ImGui::Text("Crime");
    ImGui::Combo("##Crime", &selectedCrime, Crimes, IM_ARRAYSIZE(Crimes));

    if (ImGui::Button("Submit")) {
        std::string InputName(bufName);
        std::string InputGender(Genders[selectedGender]);
        std::string InputCrime(Crimes[selectedCrime]);
        addPrisoner(InputName,age,InputGender,sentence,InputCrime);
        DisplaySwitches["showAddPanel"] = false;
        age = 0;
        selectedGender = 0;
        selectedCrime = 0;
        sentence = 0;
        strcpy_s(bufName, "");
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        DisplaySwitches["showAddPanel"] = false;
        age = 0;
        selectedGender = 0;
        selectedCrime = 0;
        sentence = 0;
        strcpy_s(bufName, "");
    }
    ImGui::EndChild();
}

void displayPanel() {
    ImVec2 windowSizedp = ImGui::GetWindowSize();
    float panelWidthdp = (windowSizedp.x - 15);
    prisData readData = readPrisCSV();
    ImGui::BeginChild("display prisoner", ImVec2(panelWidthdp, 300), true);
    
    ImGui::Columns(6, "myColumns");
    for (int i = 0; i < 6; i++)
    {
        ImGui::SetColumnWidth(i, 100);
        ImGui::Text("%s", headers[i]);
        ImGui::NextColumn();
    }
    ImGui::Separator();

    for (unsigned int i = 0; i < (readData.PN.size()); i++)
    {
        ImGui::Text("%s", readData.PN[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Name[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Age[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Gender[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Sentence[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Crime[i].c_str());
        ImGui::NextColumn();
        
    }
    if (ImGui::Button("Done")) {
        DisplaySwitches["showDisplayPanel"] = false;
    }
    ImGui::EndChild();
}

void searchpanel() {
    ImGui::Text("Search");
    ImGui::Text("PN      :");
    ImGui::SameLine();
    ImGui::InputInt("##SearchPN", &searchPN);
    ImGui::Text("Name    :");
    ImGui::SameLine();
    ImGui::InputText("##Name", searchBufName, sizeof(searchBufName));
    ImGui::Text("Age     :");
    ImGui::SameLine();
    ImGui::InputInt("##Age", &searchAge);
    ImGui::Text("Gender  :");
    ImGui::SameLine();
    ImGui::Combo("##Gender", &searchGender, Genders, IM_ARRAYSIZE(Genders));
    ImGui::Text("Sentence:");
    ImGui::SameLine();
    ImGui::InputInt("##Sentence", &searchSentence);
    ImGui::Text("Crime   :");
    ImGui::SameLine();
    ImGui::Combo("##Crime", &searchCrime, Crimes, IM_ARRAYSIZE(Crimes));

    std::string searchName(searchBufName);
    std::string strSearchGender(Genders[searchGender]);
    std::string strSearchCrime(Crimes[searchCrime]);
    prisData readData = search(searchPN, searchName, searchAge, strSearchGender, searchSentence, strSearchCrime);
    if (ImGui::Button("Reset")) {
        searchPN = 0;
        searchAge = 0;
        searchSentence = 0;
        searchGender = 0;
        searchCrime = 0;
        strcpy_s(searchBufName, "");
    }
    ImGui::Columns(6, "myColumns");
    for (int i = 0; i < 6; i++)
    {
        switch (i) {
        case 0:
            ImGui::SetColumnWidth(i, 50);
            break;
        case 1:
            ImGui::SetColumnWidth(i, 100);
            break;
        case 2:
            ImGui::SetColumnWidth(i, 90);
            break;
        case 3:
            ImGui::SetColumnWidth(i, 60);
            break;
        case 4:
            ImGui::SetColumnWidth(i, 100);
            break;
        case 5:
            ImGui::SetColumnWidth(i, 200);
            break;
        }
        
        ImGui::Text("%s", headers[i]);
        ImGui::NextColumn();
    }
    ImGui::Separator();

    for (unsigned int i = 0; i < (readData.PN.size()); i++)
    {
        ImGui::Text("%s", readData.PN[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Name[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Age[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Gender[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Sentence[i].c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", readData.Crime[i].c_str());
        ImGui::NextColumn();

    }
}
void editPanel() {
    ImGui::BeginChild("Edit prisoner", ImVec2(400, 250), true);
    ImGui::Text("PN      :");
    ImGui::SameLine();
    ImGui::InputInt("##SearchPN", &editPN);
    ImGui::Text("Name    :");
    ImGui::SameLine();
    ImGui::InputText("##Name", editBufName, sizeof(editBufName));
    ImGui::Text("Age     :");
    ImGui::SameLine();
    ImGui::InputInt("##Age", &editAge);
    ImGui::Text("Gender  :");
    ImGui::SameLine();
    ImGui::Combo("##Gender", &editGender, Genders, IM_ARRAYSIZE(Genders));
    ImGui::Text("Sentence:");
    ImGui::SameLine();
    ImGui::InputInt("##Sentence", &editSentence);
    ImGui::Text("Crime   :");
    ImGui::SameLine();
    ImGui::Combo("##Crime", &editCrime, Crimes, IM_ARRAYSIZE(Crimes));

    if (ImGui::Button("submit")) {
        std::string editName(editBufName);
        std::string streditGender(Genders[editGender]);
        std::string streditCrime(Crimes[editCrime]);
        if (!(editPrisoner(editPN, editName, editAge, streditGender, editSentence, streditCrime))) {
            editflag = false;
        }
        if (editflag == true) {
            DisplaySwitches["showEditPanel"] = false;
            editPN = 0;
            editAge = 0;
            editGender = 0;
            editCrime = 0;
            editSentence = 0;
            strcpy_s(editBufName, "");
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        DisplaySwitches["showEditPanel"] = false;
        editPN = 0;
        editAge = 0;
        editGender = 0;
        editCrime = 0;
        editSentence = 0;
        strcpy_s(editBufName, "");
    }
    if (editflag == false) {
        ImGui::Text("Invalid PN");
    }
    ImGui::EndChild();
    
}

void deletePanel() {
    ImGui::BeginChild("Edit prisoner", ImVec2(400, 250), true);
    ImGui::Text("PN      :");
    ImGui::SameLine();
    ImGui::InputInt("##SearchPN", &deletePN);
    if (ImGui::Button("submit")) {
        if (!(deletePrisoner(deletePN))) {
            deleteflag = false;
        }
        if (deleteflag == true) {
            deletePN = 0;
            DisplaySwitches["showDeletePanel"] = false;
        }

    }
    if (ImGui::Button("Cancel")) {
        deletePN = 0;
        DisplaySwitches["showDeletePanel"] = false;
    }
    if (deleteflag == false) {
        ImGui::Text("Invalid PN");
    }
    ImGui::EndChild();
}
    

void prisonerMenu() {
    if (ImGui::Button("Add prisoner")) {
        DisplaySwitches["showAddPanel"] = true;
    }
    if (ImGui::Button("View Records")) {
        DisplaySwitches["showDisplayPanel"] = true;
    }
    if (ImGui::Button("Edit Records")) {
        DisplaySwitches["showEditPanel"] = true;
    }
    if (ImGui::Button("Delete Record")) {
        DisplaySwitches["showDeletePanel"] = true;
    }
    if (DisplaySwitches["showAddPanel"] == true) {
        addPanel();
    }
    if (DisplaySwitches["showDisplayPanel"] == true) {
        displayPanel();
    }
    if (DisplaySwitches["showEditPanel"] == true) {
        editPanel();
    }
    if (DisplaySwitches["showDeletePanel"] == true) {
        deletePanel();
    }
}

void mainMenu() {
    
    ImVec2 windowSize = ImGui::GetWindowSize();
    float panelWidth = windowSize.x * 0.5f;

    ImGui::BeginChild("LeftPanel", ImVec2(panelWidth, 0), true);
    if (ImGui::BeginTabBar("TabBar1"))
    {
        if (ImGui::BeginTabItem("Cronus"))
        {
            prisonerMenu();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Settings"))
        {
            ImGui::Text("This is Tab 2");

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("RightPanel", ImVec2(panelWidth, 0), true);

    searchpanel();
  
    ImGui::EndChild();
    
}