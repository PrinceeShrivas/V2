#ifndef JSON_MANAGER_H
#define JSON_MANAGER_H

#include <string>
using namespace std;

// File paths
extern const string SCHOLARSHIPS_JSON;
extern const string UPLOADERS_JSON;
extern const string STUDENTS_JSON;
extern const string SPONSORSHIPS_JSON;

// Save functions
void saveScholarshipsToJSON();
void saveUploadersToJSON();
void saveStudentsToJSON();
void saveSponsorshipsToJSON();
void saveAllData();

// Load functions
void loadScholarshipsFromJSON();
void loadUploadersFromJSON();
void loadStudentsFromJSON();
void loadSponsorshipsFromJSON();
void loadAllData();

#endif