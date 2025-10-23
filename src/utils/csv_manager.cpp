#include "utils/csv_manager.h"
#include "core/bst.h"
#include "core/uploader.h"
#include "core/student.h"
#include "core/scholarship.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

const string SCHOLARSHIPS_CSV = "scholarships.csv";
const string UPLOADERS_CSV = "uploaders.csv";
const string STUDENTS_CSV = "students.csv";
const string SPONSORSHIPS_CSV = "sponsorships.csv";

extern unordered_map<string, string> studentPasswords;

string escapeCSV(const string &s)
{
    string result;
    for (char c : s)
    {
        if (c == '"')
            result += "\"\"";
        else if (c == '\n')
            result += "\\n";
        else if (c == '\r')
            result += "\\r";
        else
            result += c;
    }
    return "\"" + result + "\"";
}

string unescapeCSV(const string &s)
{
    if (s.length() < 2 || s.front() != '"' || s.back() != '"')
        return s;

    string result;
    for (size_t i = 1; i < s.length() - 1; i++)
    {
        if (s[i] == '"' && i + 1 < s.length() - 1 && s[i + 1] == '"')
        {
            result += '"';
            i++;
        }
        else if (s[i] == '\\' && i + 1 < s.length() - 1)
        {
            if (s[i + 1] == 'n')
            {
                result += '\n';
                i++;
            }
            else if (s[i + 1] == 'r')
            {
                result += '\r';
                i++;
            }
            else
                result += s[i];
        }
        else
            result += s[i];
    }
    return result;
}

vector<string> parseCSVLine(const string &line)
{
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); i++)
    {
        char c = line[i];

        if (c == '"')
        {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"')
            {
                field += '"';
                i++;
            }
            else
            {
                inQuotes = !inQuotes;
            }
        }
        else if (c == ',' && !inQuotes)
        {
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

void saveScholarshipsToCSV()
{
    ofstream file(SCHOLARSHIPS_CSV);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open " << SCHOLARSHIPS_CSV << " for writing.\n";
        return;
    }

    file << "id,title,provider,amount,minGPA,annualIncome,degreeOrSchool,category,state,uploaderID,contactInfo,description,endDate\n";

    inorderTraverse(root, [&file](const Scholarship &s)
                    { file << s.id << ","
                           << escapeCSV(s.title) << ","
                           << escapeCSV(s.provider) << ","
                           << s.amount << ","
                           << s.minGPA << ","
                           << s.annualIncome << ","
                           << escapeCSV(s.degreeOrSchool) << ","
                           << escapeCSV(s.category) << ","
                           << escapeCSV(s.state) << ","
                           << escapeCSV(s.uploaderID) << ","
                           << escapeCSV(s.contactInfo) << ","
                           << escapeCSV(s.description) << ","
                           << escapeCSV(s.endDate) << "\n"; });

    file.close();
    cout << "Scholarships saved to " << SCHOLARSHIPS_CSV << "\n";
}

void saveUploadersToCSV()
{
    ofstream file(UPLOADERS_CSV);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open " << UPLOADERS_CSV << " for writing.\n";
        return;
    }

    file << "uploaderID,password,scholarshipIDs\n";

    for (const auto &[id, uploader] : uploaders)
    {
        file << escapeCSV(id) << ","
             << escapeCSV(uploader.password) << ",";

        string ids;
        IDListNode *cur = uploader.head;
        while (cur)
        {
            ids += to_string(cur->scholarshipID);
            if (cur->next)
                ids += ";";
            cur = cur->next;
        }
        file << escapeCSV(ids) << "\n";
    }

    file.close();
    cout << "Uploaders saved to " << UPLOADERS_CSV << "\n";
}

void saveStudentsToCSV()
{
    ofstream file(STUDENTS_CSV);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open " << STUDENTS_CSV << " for writing.\n";
        return;
    }

    file << "studentID,password,name,email,gpa,state,degree,aboutMe,annualIncome,seekingSponsorship\n";

    for (const auto &[id, student] : students)
    {
        file << escapeCSV(student.studentID) << ","
             << escapeCSV(studentPasswords[id]) << ","
             << escapeCSV(student.name) << ","
             << escapeCSV(student.email) << ","
             << student.gpa << ","
             << escapeCSV(student.state) << ","
             << escapeCSV(student.degree) << ","
             << escapeCSV(student.aboutMe) << ","
             << student.annualIncome << ","
             << (student.seekingSponsorship ? "1" : "0") << "\n";
    }

    file.close();
    cout << "Students saved to " << STUDENTS_CSV << "\n";
}

void saveSponsorshipsToCSV()
{
    ofstream file(SPONSORSHIPS_CSV);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open " << SPONSORSHIPS_CSV << " for writing.\n";
        return;
    }

    file << "uploaderID,studentID,message,date\n";

    for (const auto &sp : sponsorships)
    {
        file << escapeCSV(sp.uploaderID) << ","
             << escapeCSV(sp.studentID) << ","
             << escapeCSV(sp.message) << ","
             << escapeCSV(sp.date) << "\n";
    }

    file.close();
    cout << "Sponsorships saved to " << SPONSORSHIPS_CSV << "\n";
}

void saveAllData()
{
    cout << "\n--- Saving all data to CSV files ---\n";
    saveScholarshipsToCSV();
    saveUploadersToCSV();
    saveStudentsToCSV();
    saveSponsorshipsToCSV();
    cout << "All data saved successfully!\n";
}

void loadScholarshipsFromCSV()
{
    ifstream file(SCHOLARSHIPS_CSV);
    if (!file.is_open())
    {
        cout << "No existing scholarships file found. Starting fresh.\n";
        return;
    }

    string line;
    getline(file, line); // Skip header

    int maxID = 0;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<string> fields = parseCSVLine(line);
        if (fields.size() < 13)
            continue;

        int id = stoi(fields[0]);
        string title = unescapeCSV(fields[1]);
        string provider = unescapeCSV(fields[2]);
        double amount = stod(fields[3]);
        double minGPA = stod(fields[4]);
        double annualIncome = stod(fields[5]);
        string degreeOrSchool = unescapeCSV(fields[6]);
        string category = unescapeCSV(fields[7]);
        string state = unescapeCSV(fields[8]);
        string uploaderID = unescapeCSV(fields[9]);
        string contactInfo = unescapeCSV(fields[10]);
        string description = unescapeCSV(fields[11]);
        string endDate = unescapeCSV(fields[12]);

        Scholarship s(id, title, provider, amount, minGPA, annualIncome,
                      degreeOrSchool, category, state, uploaderID,
                      contactInfo, description, endDate);

        root = insertBST(root, s);

        if (id > maxID)
            maxID = id;
    }

    nextScholarshipID = maxID + 1;
    file.close();
    cout << "Loaded scholarships from " << SCHOLARSHIPS_CSV << "\n";
}

void loadUploadersFromCSV()
{
    ifstream file(UPLOADERS_CSV);
    if (!file.is_open())
    {
        cout << "No existing uploaders file found. Starting fresh.\n";
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<string> fields = parseCSVLine(line);
        if (fields.size() < 3)
            continue;

        string uploaderID = unescapeCSV(fields[0]);
        string password = unescapeCSV(fields[1]);
        string idsStr = unescapeCSV(fields[2]);

        uploaders[uploaderID] = Uploader(password);

        if (!idsStr.empty())
        {
            stringstream ss(idsStr);
            string idStr;
            while (getline(ss, idStr, ';'))
            {
                int scholarshipID = stoi(idStr);
                addToUploaderList(uploaderID, scholarshipID);
            }
        }
    }

    file.close();
    cout << "Loaded uploaders from " << UPLOADERS_CSV << "\n";
}

void loadStudentsFromCSV()
{
    ifstream file(STUDENTS_CSV);
    if (!file.is_open())
    {
        cout << "No existing students file found. Starting fresh.\n";
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<string> fields = parseCSVLine(line);
        if (fields.size() < 10)
            continue;

        string studentID = unescapeCSV(fields[0]);
        string password = unescapeCSV(fields[1]);
        string name = unescapeCSV(fields[2]);
        string email = unescapeCSV(fields[3]);
        double gpa = stod(fields[4]);
        string state = unescapeCSV(fields[5]);
        string degree = unescapeCSV(fields[6]);
        string aboutMe = unescapeCSV(fields[7]);
        double annualIncome = stod(fields[8]);
        bool seeking = (fields[9] == "1");

        students[studentID] = Student(studentID, name, email, gpa, state,
                                      degree, aboutMe, annualIncome, seeking);
        studentPasswords[studentID] = password;
    }

    file.close();
    cout << "Loaded students from " << STUDENTS_CSV << "\n";
}

void loadSponsorshipsFromCSV()
{
    ifstream file(SPONSORSHIPS_CSV);
    if (!file.is_open())
    {
        cout << "No existing sponsorships file found. Starting fresh.\n";
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<string> fields = parseCSVLine(line);
        if (fields.size() < 4)
            continue;

        string uploaderID = unescapeCSV(fields[0]);
        string studentID = unescapeCSV(fields[1]);
        string message = unescapeCSV(fields[2]);
        string date = unescapeCSV(fields[3]);

        sponsorships.push_back(SponsorshipLink(uploaderID, studentID, message, date));
    }

    file.close();
    cout << "Loaded sponsorships from " << SPONSORSHIPS_CSV << "\n";
}

void loadAllData()
{
    cout << "\n--- Loading data from CSV files ---\n";
    loadScholarshipsFromCSV();
    loadUploadersFromCSV();
    loadStudentsFromCSV();
    loadSponsorshipsFromCSV();
    cout << "All data loaded successfully!\n";
}