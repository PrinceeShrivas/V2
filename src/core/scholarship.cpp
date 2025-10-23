#include "core/scholarship.h"
#include <iostream>
using namespace std;

Scholarship::Scholarship(int _id, const string &t, const string &prov, double a, double g, double income,
                         const string &deg, const string &c, const string &s, const string &u,
                         const string &contact, const string &desc, const string &end)
    : id(_id), title(t), provider(prov), amount(a), minGPA(g), annualIncome(income),
      degreeOrSchool(deg), category(c), state(s), uploaderID(u),
      contactInfo(contact), description(desc), endDate(end) {}

int cmpScholarshipKey(double amountA, int idA, const Scholarship &b)
{
    if (amountA < b.amount)
        return -1;
    if (amountA > b.amount)
        return 1;
    if (idA < b.id)
        return -1;
    if (idA > b.id)
        return 1;
    return 0;
}

int cmpScholarship(const Scholarship &a, const Scholarship &b)
{
    if (a.amount < b.amount)
        return -1;
    if (a.amount > b.amount)
        return 1;
    if (a.id < b.id)
        return -1;
    if (a.id > b.id)
        return 1;
    return 0;
}

void printScholarshipShort(const Scholarship &s)
{
    cout << "[" << s.id << "] " << s.title << " | Amount: " << s.amount
         << " | MinGPA: " << s.minGPA << " | Cat: " << s.category
         << " | State: " << s.state << " | Degree/School: " << s.degreeOrSchool
         << " | By: " << s.uploaderID << '\n';
}

void printScholarshipDetail(const Scholarship &s)
{
    cout << "ID: " << s.id << '\n';
    cout << "Title: " << s.title << '\n';
    cout << "Provider: " << s.provider << '\n';
    cout << "Amount: " << s.amount << '\n';
    cout << "Minimum GPA: " << s.minGPA << '\n';
    cout << "Maximum Annual Income: " << s.annualIncome << '\n';
    cout << "Degree/School: " << s.degreeOrSchool << '\n';
    cout << "Category: " << s.category << '\n';
    cout << "State: " << s.state << '\n';
    cout << "Uploader: " << s.uploaderID << '\n';
    cout << "Contact Info: " << s.contactInfo << '\n';
    cout << "Description: " << s.description << '\n';
    cout << "End Date: " << s.endDate << '\n';
    cout << "----------------------------------------\n";
}