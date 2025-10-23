#include "services/uploader_service.h"
#include "core/bst.h"
#include "core/scholarship.h"
#include "core/uploader.h"
#include "core/student.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include <iostream>
using namespace std;

void uploaderAddScholarship(const string &uploaderID)
{
    cout << "\n--- Add New Scholarship ---\n";
    string title = readNonEmptyLine("Title: ");
    string provider = readNonEmptyLine("Provider Name: ");
    double amount = readDouble("Amount: ");
    double gpa = readDouble("Minimum GPA: ");
    double income = readDouble("Maximum Annual Income: ");
    string degreeOrSchool = readNonEmptyLine("Degree or School (e.g., degree/school): ");
    string category = readNonEmptyLine("Category (e.g., none/SC/OBC/ST): ");
    string state = readNonEmptyLine("State: ");
    string contact = readNonEmptyLine("Contact Info (email/phone/website): ");
    string description = readNonEmptyLine("Description & Website: ");
    string endDate;
    while (true)
    {
        endDate = readNonEmptyLine("End Date (YYYY-MM-DD): ");
        if (isValidDateFormat(endDate))
            break;
        cout << "Invalid date format. Please use YYYY-MM-DD.\n";
    }

    int id = nextScholarshipID++;
    Scholarship s(id, title, provider, amount, gpa, income, degreeOrSchool,
                  category, state, uploaderID, contact, description, endDate);
    root = insertBST(root, s);
    addToUploaderList(uploaderID, id);
    cout << "Scholarship added with ID " << id << ".\n";
}

void uploaderUpdateScholarship(const string &uploaderID)
{
    cout << "\n--- Update Scholarship ---\n";
    int id = readInt("Enter scholarship ID: ");
    BSTNode *node = findByID(root, id);
    if (!node)
    {
        cout << "Scholarship not found.\n";
        return;
    }
    if (node->data.uploaderID != uploaderID)
    {
        cout << "You can only update scholarships uploaded by you.\n";
        return;
    }
    printScholarshipDetail(node->data);
    cout << "Enter new values or leave blank to keep current.\n";
    string title = readLineTrim();
    if (title.empty())
        cout << "Keeping current title.\n";
    string provider = readLineTrim();
    if (provider.empty())
        cout << "Keeping current provider.\n";
    cout << "New Amount (or blank): " << flush;
    string sAmount;
    getline(cin, sAmount);
    cout << "New Min GPA (or blank): " << flush;
    string sGpa;
    getline(cin, sGpa);
    cout << "New Max Annual Income (or blank): " << flush;
    string sIncome;
    getline(cin, sIncome);
    string degreeOrSchool = readLineTrim();
    if (degreeOrSchool.empty())
        cout << "Keeping current degree/school.\n";
    string category = readLineTrim();
    if (category.empty())
        cout << "Keeping current category.\n";
    string state = readLineTrim();
    if (state.empty())
        cout << "Keeping current state.\n";
    string contact = readLineTrim();
    if (contact.empty())
        cout << "Keeping current contact info.\n";
    string description = readLineTrim();
    if (description.empty())
        cout << "Keeping current description.\n";
    string endDate = readLineTrim();
    if (!endDate.empty() && !isValidDateFormat(endDate))
    {
        cout << "Invalid date format. Keeping current end date.\n";
        endDate = node->data.endDate;
    }
    else if (endDate.empty())
    {
        cout << "Keeping current end date.\n";
    }

    Scholarship updated = node->data;
    bool isValid = true;

    if (!title.empty())
        updated.title = title;
    if (!provider.empty())
        updated.provider = provider;
    if (!sAmount.empty())
    {
        try
        {
            double newAmount = stod(sAmount);
            if (newAmount < 0)
            {
                cout << "Amount cannot be negative. Keeping old amount.\n";
                isValid = false;
            }
            else
            {
                updated.amount = newAmount;
            }
        }
        catch (...)
        {
            cout << "Invalid amount input; keeping old.\n";
            isValid = false;
        }
    }
    if (!sGpa.empty())
    {
        try
        {
            double newGpa = stod(sGpa);
            if (newGpa < 0)
            {
                cout << "GPA cannot be negative. Keeping old GPA.\n";
                isValid = false;
            }
            else
            {
                updated.minGPA = newGpa;
            }
        }
        catch (...)
        {
            cout << "Invalid GPA input; keeping old.\n";
            isValid = false;
        }
    }
    if (!sIncome.empty())
    {
        try
        {
            double newIncome = stod(sIncome);
            if (newIncome < 0)
            {
                cout << "Annual income cannot be negative. Keeping old income.\n";
                isValid = false;
            }
            else
            {
                updated.annualIncome = newIncome;
            }
        }
        catch (...)
        {
            cout << "Invalid income input; keeping old.\n";
            isValid = false;
        }
    }
    if (!degreeOrSchool.empty())
        updated.degreeOrSchool = degreeOrSchool;
    if (!category.empty())
        updated.category = category;
    if (!state.empty())
        updated.state = state;
    if (!contact.empty())
        updated.contactInfo = contact;
    if (!description.empty())
        updated.description = description;
    if (!endDate.empty())
        updated.endDate = endDate;

    if (isValid && updated.amount != node->data.amount)
    {
        root = deleteByKey(root, node->data.amount, node->data.id);
        root = insertBST(root, updated);
        cout << "Updated scholarship (amount changed, reinserted).\n";
    }
    else if (isValid)
    {
        node->data = updated;
        cout << "Updated scholarship in place.\n";
    }
    else
    {
        cout << "Update partially applied due to invalid inputs.\n";
    }

    cout << "\n--- Updated Scholarship Details ---\n";
    printScholarshipDetail(updated);
}

void uploaderDeleteScholarship(const string &uploaderID)
{
    cout << "\n--- Delete Scholarship ---\n";
    int id = readInt("Enter scholarship ID to delete: ");
    BSTNode *node = findByID(root, id);
    if (!node)
    {
        cout << "Scholarship not found.\n";
        return;
    }
    if (node->data.uploaderID != uploaderID)
    {
        cout << "You can only delete scholarships uploaded by you.\n";
        return;
    }
    root = deleteByKey(root, node->data.amount, node->data.id);
    removeFromUploaderList(uploaderID, id);
    cout << "Scholarship deleted.\n";
}

void uploaderShowAll(const string &uploaderID)
{
    cout << "\n--- My Scholarships ---\n";
    auto it = uploaders.find(uploaderID);
    if (it == uploaders.end())
    {
        cout << "Uploader not found.\n";
        return;
    }
    IDListNode *cur = it->second.head;
    if (!cur)
    {
        cout << "You have not uploaded any scholarships yet.\n";
        return;
    }
    while (cur)
    {
        BSTNode *n = findByID(root, cur->scholarshipID);
        if (n)
            printScholarshipDetail(n->data);
        cur = cur->next;
    }
}

void uploaderSponsorStudent(const string &uploaderID)
{
    cout << "\n--- Sponsor a Student ---\n";
    cout << "1. Browse students seeking sponsorship\n";
    cout << "2. Sponsor specific student by ID\n";
    cout << "0. Back\n";

    int choice = readInt("Choice: ", true);

    if (choice == 0)
        return;

    if (choice == 1)
    {
        cout << "\n--- Students Seeking Sponsorship ---\n";
        bool found = false;

        for (const auto &[id, student] : students)
        {
            if (student.seekingSponsorship)
            {
                found = true;
                cout << "\n========================================\n";
                cout << "Student ID: " << student.studentID << "\n";
                cout << "Name: " << student.name << "\n";
                cout << "Email: " << student.email << "\n";
                cout << "GPA: " << student.gpa << " | State: " << student.state << "\n";
                cout << "Degree: " << student.degree << "\n";
                cout << "Family Income: " << student.annualIncome << "\n";
                cout << "\n--- Their Story ---\n";
                cout << student.aboutMe << "\n";
                cout << "========================================\n";
            }
        }

        if (!found)
        {
            cout << "No students are currently seeking sponsorship.\n";
            return;
        }

        cout << "\nWould you like to sponsor a student? (1=Yes, 0=No): ";
        int sponsor = readInt("", true);
        if (sponsor != 1)
            return;
    }

    string studentID = readNonEmptyLine("Enter Student ID to sponsor: ");
    string sid = toLower(studentID);

    cout << "Enter a message for the student: ";
    string message = readLineTrim();
    if (message.empty())
        message = "I am pleased to sponsor your education. Keep up the good work!";

    if (createSponsorship(uploaderID, sid, message))
    {
        cout << "\nThank you for sponsoring a student!\n";
        cout << "You can view your sponsored students anytime from the menu.\n";
    }
}

void uploaderMenuLoop(const string &uploaderID)
{
    while (true)
    {
        cout << "\n=== Uploader Menu (" << uploaderID << ") ===\n";
        cout << "1. Add new scholarship\n";
        cout << "2. Update scholarship\n";
        cout << "3. Delete scholarship\n";
        cout << "4. Show all my scholarships\n";
        cout << "5. Sponsor a student\n";
        cout << "6. View my sponsored students\n";
        cout << "0. Logout\n";

        int ch = readInt("Choice: ", true);

        if (ch == 0)
        {
            cout << "Logging out...\n";
            break;
        }

        if (ch == 1)
            uploaderAddScholarship(uploaderID);
        else if (ch == 2)
            uploaderUpdateScholarship(uploaderID);
        else if (ch == 3)
            uploaderDeleteScholarship(uploaderID);
        else if (ch == 4)
            uploaderShowAll(uploaderID);
        else if (ch == 5)
            uploaderSponsorStudent(uploaderID);
        else if (ch == 6)
            viewSponsoredStudents(uploaderID);
        else
            cout << "Invalid option.\n";
    }
}