#ifndef UPLOADER_H
#define UPLOADER_H

#include "core/bst.h"
#include <string>
#include <unordered_map>
using namespace std;

struct Uploader
{
    string password;
    IDListNode *head;
    Uploader();
    Uploader(const string &pw);
};

extern unordered_map<string, Uploader> uploaders;

bool createUploader(const string &userID);
bool loginUploader(const string &userID);
void addToUploaderList(const string &uploaderID, int scholarshipID);
void removeFromUploaderList(const string &uploaderID, int scholarshipID);
void cleanupUploaders();

#endif