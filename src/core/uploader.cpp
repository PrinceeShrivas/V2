#include "core/uploader.h"
#include "utils/input_helpers.h"
#include "utils/string_utils.h"
#include <iostream>
using namespace std;

unordered_map<string, Uploader> uploaders;

Uploader::Uploader() : head(nullptr) {}
Uploader::Uploader(const string &pw) : password(pw), head(nullptr) {}

bool createUploader(const string &userID)
{
    string uid = toLower(userID);
    if (uploaders.find(uid) != uploaders.end())
    {
        cout << "User already exists.\n";
        return false;
    }
    string pw = readNonEmptyLine("Set password for new user: ");
    uploaders[uid] = Uploader(pw);
    cout << "Uploader '" << uid << "' created.\n";
    return true;
}

bool loginUploader(const string &userID)
{
    string uid = toLower(userID);
    auto it = uploaders.find(uid);
    if (it == uploaders.end())
    {
        cout << "No such user found.\n";
        return false;
    }
    string pw = readNonEmptyLine("Enter password: ");
    if (pw == it->second.password)
    {
        cout << "Login successful.\n";
        return true;
    }
    else
    {
        cout << "Incorrect password.\n";
        return false;
    }
}

void addToUploaderList(const string &uploaderID, int scholarshipID)
{
    auto it = uploaders.find(uploaderID);
    if (it == uploaders.end())
        return;
    IDListNode *n = new IDListNode(scholarshipID);
    n->next = it->second.head;
    it->second.head = n;
}

void removeFromUploaderList(const string &uploaderID, int scholarshipID)
{
    auto it = uploaders.find(uploaderID);
    if (it == uploaders.end())
        return;
    IDListNode *cur = it->second.head;
    IDListNode *prev = nullptr;
    while (cur)
    {
        if (cur->scholarshipID == scholarshipID)
        {
            if (prev)
                prev->next = cur->next;
            else
                it->second.head = cur->next;
            delete cur;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void cleanupUploaders()
{
    for (auto &p : uploaders)
    {
        IDListNode *cur = p.second.head;
        while (cur)
        {
            IDListNode *nx = cur->next;
            delete cur;
            cur = nx;
        }
        p.second.head = nullptr;
    }
}