#include "utils/json_manager.h"
#include "core/bst.h"
#include "core/uploader.h"
#include "core/student.h"
#include "core/scholarship.h"
#include "utils/json.hpp"
using json = nlohmann::json;
#include <fstream>
#include <iostream>
using namespace std;
using json = nlohmann::json;

const string SCHOLARSHIPS_JSON = "scholarships.json";
const string UPLOADERS_JSON = "uploaders.json";
const string STUDENTS_JSON = "students.json";
const string SPONSORSHIPS_JSON = "sponsorships.json";

extern unordered_map<string, string> studentPasswords;

// ============ SAVE FUNCTIONS ============

void saveScholarshipsToJSON()
{
    json j = json::array();

    inorderTraverse(root, [&j](const Scholarship &s)
                    {
        json scholarship = {
            {"id", s.id},
            {"title", s.title},
            {"provider", s.provider},
            {"amount", s.amount},
            {"minGPA", s.minGPA},
            {"annualIncome", s.annualIncome},
            {"degreeOrSchool", s.degreeOrSchool},
            {"category", s.category},
            {"state", s.state},
            {"uploaderID", s.uploaderID},
            {"contactInfo", s.contactInfo},
            {"description", s.description},
            {"endDate", s.endDate}
        };
        j.push_back(scholarship); });

    ofstream file(SCHOLARSHIPS_JSON);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open " << SCHOLARSHIPS_JSON << " for writing.\n";
        return;
    }

    file << j.dump(2); // Pretty print with 2-space indent
    file.close();
    cout << "Scholarships saved to " << SCHOLARSHIPS_JSON << "\n";
}

void saveUploadersToJSON()
{
    json j = json::array();

    for (const auto &[id, uploader] : uploaders)
    {
        json scholarshipIDs = json::array();
        IDListNode *cur = uploader.head;
        while (cur)
        {
            scholarshipIDs.push_back(cur->scholarshipID);
            cur = cur->next;
        }

        json uploaderData = {
            {"uploaderID", id},
            {"password", uploader.password},
            {"scholarshipIDs", scholarshipIDs}};
        j.push_back(uploaderData);
    }

    ofstream file(UPLOADERS_JSON);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open " << UPLOADERS_JSON << " for writing.\n";
        return;
    }

    file << j.dump(2);
    file.close();
    cout << "Uploaders saved to " << UPLOADERS_JSON << "\n";
}

void saveStudentsToJSON()
{
    json j = json::array();

    for (const auto &[id, student] : students)
    {
        json studentData = {
            {"studentID", student.studentID},
            {"password", studentPasswords[id]},
            {"name", student.name},
            {"email", student.email},
            {"gpa", student.gpa},
            {"state", student.state},
            {"degree", student.degree},
            {"aboutMe", student.aboutMe},
            {"annualIncome", student.annualIncome},
            {"seekingSponsorship", student.seekingSponsorship}};
        j.push_back(studentData);
    }

    ofstream file(STUDENTS_JSON);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open " << STUDENTS_JSON << " for writing.\n";
        return;
    }

    file << j.dump(2);
    file.close();
    cout << "Students saved to " << STUDENTS_JSON << "\n";
}

void saveSponsorshipsToJSON()
{
    json j = json::array();

    for (const auto &sp : sponsorships)
    {
        json sponsorship = {
            {"uploaderID", sp.uploaderID},
            {"studentID", sp.studentID},
            {"message", sp.message},
            {"date", sp.date}};
        j.push_back(sponsorship);
    }

    ofstream file(SPONSORSHIPS_JSON);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open " << SPONSORSHIPS_JSON << " for writing.\n";
        return;
    }

    file << j.dump(2);
    file.close();
    cout << "Sponsorships saved to " << SPONSORSHIPS_JSON << "\n";
}

void saveAllData()
{
    cout << "\n--- Saving all data to JSON files ---\n";
    saveScholarshipsToJSON();
    saveUploadersToJSON();
    saveStudentsToJSON();
    saveSponsorshipsToJSON();
    cout << "All data saved successfully!\n";
}

// ============ LOAD FUNCTIONS ============

void loadScholarshipsFromJSON()
{
    ifstream file(SCHOLARSHIPS_JSON);
    if (!file.is_open())
    {
        cout << "No existing scholarships file found. Starting fresh.\n";
        return;
    }

    try
    {
        json j;
        file >> j;

        int maxID = 0;
        for (const auto &item : j)
        {
            int id = item["id"];
            string title = item["title"];
            string provider = item["provider"];
            double amount = item["amount"];
            double minGPA = item["minGPA"];
            double annualIncome = item["annualIncome"];
            string degreeOrSchool = item["degreeOrSchool"];
            string category = item["category"];
            string state = item["state"];
            string uploaderID = item["uploaderID"];
            string contactInfo = item["contactInfo"];
            string description = item["description"];
            string endDate = item["endDate"];

            Scholarship s(id, title, provider, amount, minGPA, annualIncome,
                          degreeOrSchool, category, state, uploaderID,
                          contactInfo, description, endDate);

            root = insertBST(root, s);

            if (id > maxID)
                maxID = id;
        }

        nextScholarshipID = maxID + 1;
        cout << "Loaded scholarships from " << SCHOLARSHIPS_JSON << "\n";
    }
    catch (const exception &e)
    {
        cerr << "Error loading scholarships: " << e.what() << "\n";
    }

    file.close();
}

void loadUploadersFromJSON()
{
    ifstream file(UPLOADERS_JSON);
    if (!file.is_open())
    {
        cout << "No existing uploaders file found. Starting fresh.\n";
        return;
    }

    try
    {
        json j;
        file >> j;

        for (const auto &item : j)
        {
            string uploaderID = item["uploaderID"];
            string password = item["password"];

            uploaders[uploaderID] = Uploader(password);

            if (item.contains("scholarshipIDs"))
            {
                for (const auto &id : item["scholarshipIDs"])
                {
                    addToUploaderList(uploaderID, id);
                }
            }
        }

        cout << "Loaded uploaders from " << UPLOADERS_JSON << "\n";
    }
    catch (const exception &e)
    {
        cerr << "Error loading uploaders: " << e.what() << "\n";
    }

    file.close();
}

void loadStudentsFromJSON()
{
    ifstream file(STUDENTS_JSON);
    if (!file.is_open())
    {
        cout << "No existing students file found. Starting fresh.\n";
        return;
    }

    try
    {
        json j;
        file >> j;

        for (const auto &item : j)
        {
            string studentID = item["studentID"];
            string password = item["password"];
            string name = item["name"];
            string email = item["email"];
            double gpa = item["gpa"];
            string state = item["state"];
            string degree = item["degree"];
            string aboutMe = item["aboutMe"];
            double annualIncome = item["annualIncome"];
            bool seeking = item["seekingSponsorship"];

            students[studentID] = Student(studentID, name, email, gpa, state,
                                          degree, aboutMe, annualIncome, seeking);
            studentPasswords[studentID] = password;
        }

        cout << "Loaded students from " << STUDENTS_JSON << "\n";
    }
    catch (const exception &e)
    {
        cerr << "Error loading students: " << e.what() << "\n";
    }

    file.close();
}

void loadSponsorshipsFromJSON()
{
    ifstream file(SPONSORSHIPS_JSON);
    if (!file.is_open())
    {
        cout << "No existing sponsorships file found. Starting fresh.\n";
        return;
    }

    try
    {
        json j;
        file >> j;

        for (const auto &item : j)
        {
            string uploaderID = item["uploaderID"];
            string studentID = item["studentID"];
            string message = item["message"];
            string date = item["date"];

            sponsorships.push_back(SponsorshipLink(uploaderID, studentID, message, date));
        }

        cout << "Loaded sponsorships from " << SPONSORSHIPS_JSON << "\n";
    }
    catch (const exception &e)
    {
        cerr << "Error loading sponsorships: " << e.what() << "\n";
    }

    file.close();
}

void loadAllData()
{
    cout << "\n--- Loading data from JSON files ---\n";
    loadScholarshipsFromJSON();
    loadUploadersFromJSON();
    loadStudentsFromJSON();
    loadSponsorshipsFromJSON();
    cout << "All data loaded successfully!\n";
}