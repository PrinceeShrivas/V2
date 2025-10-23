#include "utils/string_utils.h"
#include <bits/stdc++.h>
using namespace std;

string toLower(const string &s)
{
    string r = s;
    for (char &c : r)
    {
        c = tolower(c);
    }
    return r;
}