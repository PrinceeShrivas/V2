#ifndef UPLOADER_SERVICE_H
#define UPLOADER_SERVICE_H
#include <string>
using namespace std;
void uploaderAddScholarship(const string &uploaderID);
void uploaderUpdateScholarship(const string &uploaderID);
void uploaderDeleteScholarship(const string &uploaderID);
void uploaderShowAll(const string &uploaderID);
void uploaderMenuLoop(const string &uploaderID);
#endif
