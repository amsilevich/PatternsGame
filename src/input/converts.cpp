#include <vector>
#include <string>

bool isDigit(char symbol) {
    return symbol <= '9' && symbol >= '0';
}

bool isLetter(char symbol) {
    return (symbol <= 'z' && symbol >= 'a') || (symbol <= 'Z' && symbol >= 'A');
}

bool checkDigits(const std::string &input) {
    for (size_t i = 0; i < input.size(); ++i) {
        if (!isDigit(input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

bool checkLetters(const std::string &input) {
    for (size_t i = 0; i < input.size(); ++i) {
        if (!isLetter(input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

bool emptyArray(const std::vector<int> &a) {
    for (auto i: a) {
        if (i != 0) {
            return 0;
        }
    }
    return 1;
}

bool isEmpty(const std::string &input) {
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

void toLower(std::string &s) {
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = tolower(s[i]);
        }
    }
}

void combString(std::string &input) {
    toLower(input);
    if (isEmpty(input)) {
        input = "";
        return;
    }
    bool oneSpace = input[0] == ' ';
    int curInd = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] != ' ') {
            oneSpace = 0;
        }
        if (!oneSpace) {
            input[curInd] = input[i];
            ++curInd;
        }
        if (input[i] == ' ' && !oneSpace) {
            oneSpace = 1;
        }
    }
    while (input.size() != curInd) {
        input.pop_back();
    }
    if (input.back() != ' ') {
        input += " ";
    }
    return;
};

void inputToStrings(const std::string &input, std::vector<std::string> &dataStrings) {
    std::string curString = "";
    for (size_t i = 0; i < input.size(); ++i) {
        while(input[i] != ' ') {
            curString += input[i];
            i++;
        }
        dataStrings.push_back(curString);
        curString = "";
    }
}

void inputToInts(const std::string &input, std::vector<int> &data) {
    std::vector<std::string> dataStrings;
    inputToStrings(input, dataStrings);
    for (size_t i = 0; i < dataStrings.size(); ++i) {
        data.push_back(atoi(dataStrings[i].c_str()));
    }
}

void stringInArray(const std::string &input, const std::vector<std::string> &data) {
    for (auto i: data) {
        if (i == input) {
            return 1;
        }
    }
    return 0;
}