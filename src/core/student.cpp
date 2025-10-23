#include "core/student.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

unordered_map<string, Student> students;
unordered_map<string, string> studentPasswords;
vector<SponsorshipLink> sponsorships;

Student::Student(const string &id, const string &n, const string &em,
                 double g, const string &st, const string &deg,
                 const string &about, double income, bool seeking)
    : studentID(id), name(n), email(em), gpa(g), state(st),
      degree(deg), aboutMe(about), annualIncome(income),
      seekingSponsorship(seeking) {}

SponsorshipLink::SponsorshipLink(const string &uid, const string &sid,
                                 const string &msg, const string &dt)
    : uploaderID(uid), studentID(sid), message(msg), date(dt) {}

bool createStudent(const string &studentID)
{
    string sid = toLower(studentID);
    if (students.find(sid) != students.end())
    {
        cout << "Student already exists.\n";
        return false;
    }

    string password = readNonEmptyLine("Set password: ");
    string name = readNonEmptyLine("Full Name: ");
    string email = readNonEmptyLine("Email: ");
    double gpa = readDouble("GPA: ");
    string state = readNonEmptyLine("State: ");
    string degree = readNonEmptyLine("Degree/School: ");
    cout << "Write about yourself (your story, goals, challenges): ";
    string about = readLineTrim();
    double income = readDouble("Annual Family Income: ");

    cout << "Are you seeking sponsorship? (1=Yes, 0=No): ";
    int seekInt = readInt("", true);
    bool seeking = (seekInt == 1);

    students[sid] = Student(sid, name, email, gpa, state, degree,
                            about, income, seeking);
    studentPasswords[sid] = password;

    cout << "Student profile created successfully!\n";
    return true;
}

bool loginStudent(const string &studentID, string &password)
{
    string sid = toLower(studentID);
    if (students.find(sid) == students.end())
    {
        cout << "Student not found.\n";
        return false;
    }

    password = readNonEmptyLine("Enter password: ");
    if (studentPasswords[sid] != password)
    {
        cout << "Incorrect password.\n";
        return false;
    }

    cout << "Login successful! Welcome " << students[sid].name << "!\n";
    return true;
}

void updateStudentProfile(const string &studentID)
{
    string sid = toLower(studentID);
    if (students.find(sid) == students.end())
    {
        cout << "Student not found.\n";
        return;
    }

    Student &s = students[sid];
    cout << "\n--- Update Profile ---\n";
    cout << "Leave blank to keep current value.\n";

    cout << "Name [" << s.name << "]: ";
    string name = readLineTrim();
    if (!name.empty())
        s.name = name;

    cout << "Email [" << s.email << "]: ";
    string email = readLineTrim();
    if (!email.empty())
        s.email = email;

    cout << "GPA [" << s.gpa << "]: ";
    string gpaStr = readLineTrim();
    if (!gpaStr.empty())
    {
        try
        {
            s.gpa = stod(gpaStr);
        }
        catch (...)
        {
            cout << "Invalid GPA, keeping old value.\n";
        }
    }

    cout << "State [" << s.state << "]: ";
    string state = readLineTrim();
    if (!state.empty())
        s.state = state;

    cout << "Degree/School [" << s.degree << "]: ";
    string degree = readLineTrim();
    if (!degree.empty())
        s.degree = degree;

    cout << "About Me [current: " << s.aboutMe.substr(0, 50) << "...]: ";
    string about = readLineTrim();
    if (!about.empty())
        s.aboutMe = about;

    cout << "Annual Income [" << s.annualIncome << "]: ";
    string incomeStr = readLineTrim();
    if (!incomeStr.empty())
    {
        try
        {
            s.annualIncome = stod(incomeStr);
        }
        catch (...)
        {
            cout << "Invalid income, keeping old value.\n";
        }
    }

    cout << "Seeking Sponsorship? (1=Yes, 0=No) ["
         << (s.seekingSponsorship ? "Yes" : "No") << "]: ";
    string seekStr = readLineTrim();
    if (!seekStr.empty())
    {
        s.seekingSponsorship = (seekStr == "1");
    }

    cout << "Profile updated successfully!\n";
}

void viewStudentProfile(const string &studentID)
{
    string sid = toLower(studentID);
    if (students.find(sid) == students.end())
    {
        cout << "Student not found.\n";
        return;
    }

    const Student &s = students[sid];
    cout << "\n========================================\n";
    cout << "        STUDENT PROFILE\n";
    cout << "========================================\n";
    cout << "Student ID: " << s.studentID << "\n";
    cout << "Name: " << s.name << "\n";
    cout << "Email: " << s.email << "\n";
    cout << "GPA: " << s.gpa << "\n";
    cout << "State: " << s.state << "\n";
    cout << "Degree/School: " << s.degree << "\n";
    cout << "Annual Family Income: " << s.annualIncome << "\n";
    cout << "Seeking Sponsorship: " << (s.seekingSponsorship ? "Yes" : "No") << "\n";
    cout << "\n--- About Me ---\n";
    cout << s.aboutMe << "\n";
    cout << "========================================\n";
}

bool createSponsorship(const string &uploaderID, const string &studentID,
                       const string &message)
{
    string sid = toLower(studentID);
    if (students.find(sid) == students.end())
    {
        cout << "Student not found.\n";
        return false;
    }

    // Check if already sponsored
    for (const auto &sp : sponsorships)
    {
        if (sp.uploaderID == uploaderID && sp.studentID == sid)
        {
            cout << "You are already sponsoring this student.\n";
            return false;
        }
    }

    string date = "2025-10-23"; // In real system, get current date
    sponsorships.push_back(SponsorshipLink(uploaderID, sid, message, date));
    cout << "Sponsorship created successfully!\n";
    return true;
}

void viewSponsoredStudents(const string &uploaderID)
{
    cout << "\n--- My Sponsored Students ---\n";
    bool found = false;

    for (const auto &sp : sponsorships)
    {
        if (sp.uploaderID == uploaderID)
        {
            found = true;
            const Student &s = students[sp.studentID];
            cout << "\n========================================\n";
            cout << "Student: " << s.name << " (" << s.studentID << ")\n";
            cout << "Email: " << s.email << "\n";
            cout << "GPA: " << s.gpa << " | State: " << s.state << "\n";
            cout << "Degree: " << s.degree << "\n";
            cout << "Family Income: " << s.annualIncome << "\n";
            cout << "\n--- Student's Story ---\n";
            cout << s.aboutMe << "\n";
            cout << "\n--- Your Message to Student ---\n";
            cout << sp.message << "\n";
            cout << "Sponsored on: " << sp.date << "\n";
            cout << "========================================\n";
        }
    }

    if (!found)
    {
        cout << "You are not sponsoring any students yet.\n";
    }
}

void viewMySponsors(const string &studentID)
{
    string sid = toLower(studentID);
    cout << "\n--- My Sponsors ---\n";
    bool found = false;

    for (const auto &sp : sponsorships)
    {
        if (sp.studentID == sid)
        {
            found = true;
            cout << "\n========================================\n";
            cout << "Sponsor: " << sp.uploaderID << "\n";
            cout << "Message: " << sp.message << "\n";
            cout << "Sponsored since: " << sp.date << "\n";
            cout << "========================================\n";
        }
    }

    if (!found)
    {
        cout << "You don't have any sponsors yet.\n";
        cout << "Keep your profile updated to attract sponsors!\n";
    }
}

void cleanupStudents()
{
    students.clear();
    studentPasswords.clear();
    sponsorships.clear();
}