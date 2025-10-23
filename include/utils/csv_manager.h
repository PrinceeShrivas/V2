#ifndef CSV_MANAGER_H
#define CSV_MANAGER_H

#include <string>
using namespace std;

// File paths
extern const string SCHOLARSHIPS_CSV;
extern const string UPLOADERS_CSV;
extern const string STUDENTS_CSV;
extern const string SPONSORSHIPS_CSV;

// Save functions
void saveScholarshipsToCSV();
void saveUploadersToCSV();
void saveStudentsToCSV();
void saveSponsorshipsToCSV();
void saveAllData();

// Load functions
void loadScholarshipsFromCSV();
void loadUploadersFromCSV();
void loadStudentsFromCSV();
void loadSponsorshipsFromCSV();
void loadAllData();

// Helper functions
string escapeCSV(const string &s);
string unescapeCSV(const string &s);

#endif