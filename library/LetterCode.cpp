#include "LetterCode.h"
#include <iostream>
LetterCode GetLetterCodeFromChar(char ch) {
    LetterCode letter_code;
    for (int i = 0; i < 8; ++i) {
        letter_code.push_back((ch >> (7 - i)) % 2);
    }
    letter_code.push_back(false);
    return letter_code;
}

LetterCode GetLetterCodeFromInt(int values) {
    LetterCode code;
    int val = 256;
    for (int i = 0; i < 9; i ++) {
        if (val <= values) {
            code.push_back(true);
            values -= val;
        } else {
            code.push_back(false);
        }
        val /= 2;
    }
    std::reverse(code.begin(), code.end());
    return code;
}

char GetCharFromLetterCode(LetterCode code) {
    char ch = 0;
    for (int i = 0; i < 8; ++i) {
        ch <<= 1;
        ch += code[i];
    }
    return ch;
}

int GetIntFromLetterCode(LetterCode code) {
    int val = 1;
    int answer = 0;
    for (int i = 0; i < 9; ++i) {
        if (code[i]) {
            answer += val;
        }
        val *= 2;
    }
    return answer;
}

