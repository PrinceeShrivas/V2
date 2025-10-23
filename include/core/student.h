#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct Student
{
    string studentID;
    string name;
    string email;
    double gpa;
    string state;
    string degree;
    string aboutMe;
    double annualIncome;
    bool seekingSponsorship;

    Student() = default;
    Student(const string &id, const string &n, const string &em,
            double g, const string &st, const string &deg,
            const string &about, double income, bool seeking);
};

struct SponsorshipLink
{
    string uploaderID;
    string studentID;
    string message;
    string date;

    SponsorshipLink() = default;
    SponsorshipLink(const string &uid, const string &sid,
                    const string &msg, const string &dt);
};

// Student registry - extern declarations
extern unordered_map<string, Student> students;
extern unordered_map<string, string> studentPasswords;
extern vector<SponsorshipLink> sponsorships;

// Function declarations
bool createStudent(const string &studentID);
bool loginStudent(const string &studentID, string &password);
void updateStudentProfile(const string &studentID);
void viewStudentProfile(const string &studentID);
bool createSponsorship(const string &uploaderID, const string &studentID, const string &message);
void viewSponsoredStudents(const string &uploaderID);
void viewMySponsors(const string &studentID);
void cleanupStudents();

#endif