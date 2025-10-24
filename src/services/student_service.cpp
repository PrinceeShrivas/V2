#include "services/student_service.h"
#include "core/bst.h"
#include "core/scholarship.h"
#include "core/student.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void studentViewAll()
{
    cout << "\n--- All Scholarships (sorted by amount ascending) ---\n";
    bool any = false;
    inorderTraverse(root, [&](const Scholarship &s)
                    {
                        printScholarshipDetail(s);
                        any = true; });
    if (!any)
        cout << "No scholarships available.\n";
}

void studentSearchMenu()
{
    cout << "\n--- Search Scholarships ---\n";
    cout << "Enter keyword to search (title, provider, description, category, state, degree/school) or leave blank: ";
    string keyword;
    getline(cin, keyword);
    ResultList res;
    if (!keyword.empty())
    {
        collectIf(root, res, [keyword](const Scholarship &s)
                  {
                      string k = toLower(keyword);
                      return toLower(s.title).find(k) != string::npos ||
                             toLower(s.provider).find(k) != string::npos ||
                             toLower(s.description).find(k) != string::npos ||
                             toLower(s.category).find(k) != string::npos ||
                             toLower(s.state).find(k) != string::npos ||
                             toLower(s.degreeOrSchool).find(k) !=string::npos; });
    }
    else
    {
        collectIf(root, res, [](const Scholarship &)
                  { return true; });
    }
    printResultList(res);
}

void studentSortMenu()
{
    cout << "\n--- Sort Scholarships ---\n";
    cout << "1. Amount (asc)\n2. Amount (desc)\n3. MinGPA (asc)\n4. MinGPA (desc)\n5. Title (A-Z)\n0. Back\n";
    int ch = readInt("Choice: ", true);
    if (ch == 0)
        return;
    vector<Scholarship> v;
    inorderTraverse(root, [&v](const Scholarship &s)
                    { v.push_back(s); });
    if (v.empty())
    {
        cout << "No scholarships available.\n";
        return;
    }
    switch (ch)
    {
    case 1:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return a.amount < b.amount; });
        break;
    case 2:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return a.amount > b.amount; });
        break;
    case 3:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return a.minGPA < b.minGPA; });
        break;
    case 4:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return a.minGPA > b.minGPA; });
        break;
    case 5:
        sort(v.begin(), v.end(), [](auto &a, auto &b)
             { return toLower(a.title) < toLower(b.title); });
        break;
    default:
        cout << "Invalid option.\n";
        return;
    }
    cout << "Sorted result:\n";
    for (auto &s : v)
        printScholarshipDetail(s);
}

void studentFilterMenu()
{
    cout << "\n--- Filter Scholarships ---\n";
    cout << "For any criterion, press Enter to ignore it.\n";
    cout << "Min GPA (or blank): " << flush;
    string sg;
    getline(cin, sg);
    cout << "Min Amount (or blank): " << flush;
    string sa1;
    getline(cin, sa1);
    cout << "Max Amount (or blank): " << flush;
    string sa2;
    getline(cin, sa2);
    cout << "Category (or blank): " << flush;
    string cat;
    getline(cin, cat);
    cout << "State (or blank): " << flush;
    string st;
    getline(cin, st);
    cout << "Degree/School (or blank): " << flush;
    string deg;
    getline(cin, deg);

    double g = -1, a1 = -1, a2 = -1;
    if (!sg.empty())
        try
        {
            g = stod(sg);
        }
        catch (...)
        {
            g = -1;
        }
    if (!sa1.empty())
        try
        {
            a1 = stod(sa1);
        }
        catch (...)
        {
            a1 = -1;
        }
    if (!sa2.empty())
        try
        {
            a2 = stod(sa2);
        }
        catch (...)
        {
            a2 = -1;
        }

    ResultList res;
    collectIf(root, res, [g, a1, a2, cat, st, deg](const Scholarship &s)
              {
                  if (g >= 0 && !(g >= s.minGPA)) return false;
                  if (a1 >= 0 && !(s.amount >= a1)) return false;
                  if (a2 >= 0 && !(s.amount <= a2)) return false;
                  if (!cat.empty() && toLower(s.category) != toLower(cat)) return false;
                  if (!st.empty() && toLower(s.state) != toLower(st)) return false;
                  if (!deg.empty() && toLower(s.degreeOrSchool) != toLower(deg)) return false;
                  return true; });
    printResultList(res);
}

void studentProfileMenu(const string &studentID)
{
    while (true)
    {
        cout << "\n=== My Profile ===\n";
        cout << "1. View my profile\n";
        cout << "2. Update my profile\n";
        cout << "3. View my sponsors\n";
        cout << "0. Back\n";
        int ch = readInt("Choice: ", true);
        if (ch == 0)
            break;
        else if (ch == 1)
            viewStudentProfile(studentID);
        else if (ch == 2)
            updateStudentProfile(studentID);
        else if (ch == 3)
            viewMySponsors(studentID);
        else
            cout << "Invalid choice.\n";
    }
}

void studentMenuLoop()
{
    // Check if student wants to login or browse as guest
    cout << "\n=== Student Portal ===\n";
    cout << "1. Login/Register\n";
    cout << "2. Browse as Guest\n";
    cout << "0. Back\n";
    int choice = readInt("Choice: ", true);
    if (choice == 0)
        return;
    string studentID = "";
    bool loggedIn = false;
    if (choice == 1)
    {
        studentID = readNonEmptyLine("Enter Student ID: ");
        string sid = toLower(studentID);
        if (students.find(sid) == students.end())
        {
            cout << "Student not found. Would you like to create a profile? (1=Yes, 0=No): ";
            int create = readInt("", true);
            if (create == 1)
            {
                if (createStudent(sid))
                {
                    loggedIn = true;
                    studentID = sid;
                }
            }
        }
        else
        {
            string password;
            if (loginStudent(sid, password))
            {
                loggedIn = true;
                studentID = sid;
            }
        }
    }
    while (true)
    {
        cout << "\n=== Student Menu ===\n";
        cout << "1. View all scholarships\n";
        cout << "2. Search scholarships\n";
        cout << "3. Sort scholarships\n";
        cout << "4. Filter scholarships\n";
        if (loggedIn)
        {
            cout << "5. My Profile\n";
        }
        cout << "0. Back\n";
        int ch = readInt("Choice: ", true);
        if (ch == 0)
            break;
        else if (ch == 1)
            studentViewAll();
        else if (ch == 2)
            studentSearchMenu();
        else if (ch == 3)
            studentSortMenu();
        else if (ch == 4)
            studentFilterMenu();
        else if (ch == 5 && loggedIn)
            studentProfileMenu(studentID);
        else
            cout << "Invalid choice.\n";
    }
}
