#include "core/bst.h"
#include "core/uploader.h"
#include "core/scholarship.h"
#include "core/student.h"
#include "services/student_service.h"
#include "services/uploader_service.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include "utils/csv_manager.h"
#include <iostream>
using namespace std;

void seedData()
{
    uploaders["admin"] = Uploader("adminpw");

    Scholarship a(nextScholarshipID++, "Mahindra Excellence", "Mahindra Foundation", 50000.0, 8.5, 100000, "degree", "OPEN", "Tamil Nadu", "admin", "contact@mh.org", "Support for students which are underprivileged.", "2025-12-31");
    Scholarship b(nextScholarshipID++, "Arts Support", "Arts Council", 20000.0, 8.0, 100000, "school", "SC", "Delhi", "admin", "arts@support.org", "Grants for arts students.", "2025-11-30");
    Scholarship c(nextScholarshipID++, "Merit Grant", "Merit Foundation", 300000.0, 9.5, 120000, "Any", "General", "Jharkhand", "admin", "info@merit.org", "Merit-based scholarship.", "2025-10-15");

    root = insertBST(root, a);
    root = insertBST(root, b);
    root = insertBST(root, c);

    addToUploaderList("admin", a.id);
    addToUploaderList("admin", b.id);
    addToUploaderList("admin", c.id);

    // Sample student data
    extern unordered_map<string, string> studentPasswords;
    students["john"] = Student("john", "John Doe", "john@email.com", 8.5, "Tamil Nadu", "Computer Science",
                               "I am a passionate CS student from a rural background. Despite financial challenges, I have maintained a strong GPA and dream of becoming a software engineer to help my family.",
                               50000, true);
    studentPasswords["john"] = "john123";

    students["priya"] = Student("priya", "Priya Kumar", "priya@email.com", 9.0, "Karnataka", "Medicine",
                                "Coming from a farmer's family, I aspire to become a doctor and serve rural communities. Education is my path to making a difference in healthcare access.",
                                40000, true);
    studentPasswords["priya"] = "priya123";
}

void displayWelcome()
{
    cout << "=========================================\n";
    cout << "            ScholarMate      \n";
    cout << "   Connecting Students & Opportunities  \n";
    cout << "=========================================\n\n";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    displayWelcome();

    // Try to load existing data from CSV
    cout << "Checking for existing data...\n";
    loadAllData();

    // If no data was loaded, seed with default data
    if (uploaders.empty())
    {
        cout << "No existing data found. Loading default data...\n";
        seedData();
    }

    while (true)
    {
        cout << "\n========== Main Menu ==========\n";
        cout << "1. Uploader/Sponsor Portal\n";
        cout << "2. Student Portal\n";
        cout << "3. Save all data to CSV\n";
        cout << "0. Exit\n";
        cout << "===============================\n";

        int ch = readInt("Choice: ", true);

        if (ch == 0)
            break;

        if (ch == 1)
        {
            cout << "\n--- Uploader/Sponsor Portal ---\n";
            string userID = readNonEmptyLine("Enter userID: ");
            string uidLower = toLower(userID);

            if (uploaders.find(uidLower) == uploaders.end())
            {
                cout << "User does not exist. Creating new uploader.\n";
                if (!createUploader(uidLower))
                    continue;
            }

            if (!loginUploader(uidLower))
                continue;

            uploaderMenuLoop(uidLower);
        }
        else if (ch == 2)
        {
            studentMenuLoop();
        }
        else if (ch == 3)
        {
            saveAllData();
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    // Auto-save on exit
    cout << "\nSaving data before exit...\n";
    saveAllData();

    // Cleanup
    cleanupBST(root);
    cleanupUploaders();
    cleanupStudents();

    cout << "\nThank you for using ScholarMate!\n";
    cout << "Empowering education, one scholarship at a time.\n";
    cout << "Goodbye!\n";

    return 0;
}