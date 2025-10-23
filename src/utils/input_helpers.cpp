#include "utils/input_helpers.h"
#include <iostream>
#include <limits>
#include <regex>
using namespace std;

void clearCin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string readLineTrim()
{
    string s;
    getline(cin, s);
    auto l = s.find_first_not_of(" \t\r\n");
    if (l == string::npos)
        return "";
    auto r = s.find_last_not_of(" \t\r\n");
    return s.substr(l, r - l + 1);
}

string readNonEmptyLine(const string &prompt)
{
    string s;
    while (true)
    {
        cout << prompt << flush;
        s = readLineTrim();
        if (!s.empty())
            return s;
        cout << "Input cannot be empty. Try again.\n";
    }
}

double readDouble(const string &prompt, bool allowNegative)
{
    while (true)
    {
        cout << prompt << flush;
        string s;
        getline(cin, s);
        try
        {
            double v = stod(s);
            if (!allowNegative && v < 0)
            {
                cout << "Value cannot be negative.\n";
                continue;
            }
            return v;
        }
        catch (...)
        {
            cout << "Please enter a valid number.\n";
        }
    }
}

int readInt(const string &prompt, bool allowNegative)
{
    while (true)
    {
        cout << prompt << flush;
        string s;
        getline(cin, s);
        try
        {
            long v = stol(s);
            if (!allowNegative && v < 0)
            {
                cout << "Value cannot be negative.\n";
                continue;
            }
            return int(v);
        }
        catch (...)
        {
            cout << "Please enter a valid integer.\n";
        }
    }
}

bool isValidDateFormat(const string &date)
{
    regex datePattern("\\d{4}-\\d{2}-\\d{2}");
    return regex_match(date, datePattern);
}