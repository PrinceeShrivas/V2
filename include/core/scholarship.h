#ifndef SCHOLARSHIP_H
#define SCHOLARSHIP_H

#include <string>
using namespace std;

struct Scholarship
{
    int id;
    string title;
    double amount;
    double minGPA;
    string category;
    string state;
    string uploaderID;
    string provider;
    double annualIncome;
    string degreeOrSchool;
    string contactInfo;
    string description;
    string endDate;

    Scholarship() = default;
    Scholarship(int _id, const string &t, const string &prov, double a, double g, double income,
                const string &deg, const string &c, const string &s, const string &u,
                const string &contact, const string &desc, const string &end);
};

int cmpScholarshipKey(double amountA, int idA, const Scholarship &b);
int cmpScholarship(const Scholarship &a, const Scholarship &b);

void printScholarshipShort(const Scholarship &s);
void printScholarshipDetail(const Scholarship &s);

#endif