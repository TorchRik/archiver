#pragma once
#include <vector>

using LetterCode = std::vector<bool>;

const LetterCode FILENAME_END = {1,0,0,0,0,0,0,0,0};
const LetterCode ONE_MORE_FILE = {1,0,0,0,0,0,0,0,1};
const LetterCode ARCHIVE_END = {1,0,0,0,0,0,0,1,0};

LetterCode GetLetterCodeFromChar(char ch);
LetterCode GetLetterCodeFromInt(int values);

char GetCharFromLetterCode(LetterCode code);
int GetIntFromLetterCode(LetterCode code);
