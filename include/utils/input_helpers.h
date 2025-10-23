#ifndef INPUT_HELPERS_H
#define INPUT_HELPERS_H

#include <string>
using namespace std;

void clearCin();
string readLineTrim();
string readNonEmptyLine(const string &prompt);
double readDouble(const string &prompt, bool allowNegative = false);
int readInt(const string &prompt, bool allowNegative = false);
bool isValidDateFormat(const string &date);

#endif