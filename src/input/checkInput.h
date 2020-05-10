#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "converts.h"

class GetInput {
private:
    std::string readInput() {
        std::getline(std::cin, input);
        combString(input);
        return input;
    }
    virtual bool check(const std::string &input) = 0;
protected:
    std::string input;
    std::string getInput() {
        printMessage();
        readInput();
        if (!check(input)) {
            exceptWrongInput();
            getInput();
        }
        return input;
    }
    void exceptWrongInput() {
        std::cout << "Incorrect data format. Please fix mistakes and try again" << std::endl;
    }
    virtual void printMessage() = 0;
};

class GetUnits: public GetInput {
private:
    void printMessage() {
        std::cout << "Write troops in format: \"count of Giants\", "
                     "\"count of Archers\", \"count of Healers\"." << std::endl;
    }
    bool check(const std::string &input) {
        if (isEmpty(input) || !checkDigits(input)) {
            return 0;
        }
        std::vector<int> data;
        inputToInts(input, data);
        if (data.size() != 3) {
            return 0;
        }
        return 1;
    }
public:
    std::vector<int> returnInput() {
        getInput();
        std::vector<int> data;
        inputToInts(input, data);
        return data;
    }
};

class GetStrings: public GetInput {
private:
    void printMessage() {
        std::cout << "Please, write the names" << std::endl;
    }
    bool check(const std::string &input) {
        if (isEmpty(input) || !checkLetters(input)) {
            return 0;
        }
        return 1;
    }
public:
    std::vector<std::string> returnInput() {
        getInput();
        std::vector<std::string> data;
        inputToStrings(input, data);
        return data;
    }
};

class GetOneString: public GetInput {
protected:
    bool check(const std::string &input) {
        if (isEmpty(input) || !checkLetters(input)) {
            return 0;
        }
        std::vector<std::string> dataStrings;
        inputToStrings(input, dataStrings);
        if (dataStrings.size() != 1) {
            return 0;
        }
        std::string answer = dataStrings[0];
        return checkWord(answer);
    }
    virtual bool checkWord(const std::string& input) = 0;
public:
    std::string returnInput() {
        getInput();
        std::vector<std::string> data;
        inputToStrings(input, data);
        return data[0];
    }
};

class GetAnswerDecorator: public GetOneString {
private:
    void printMessage() {
        std::cout << "Write your answer(yes or no)" << std::endl;
    }
    bool checkWord(const std::string &answer) override {
        return answer == "yes" || answer == "no";
    }
};

class GetNameDecorator: public GetOneString {
private:
    void printMessage() {
        std::cout << "Please, write the name of new army(only symbols)" << std::endl;
    }
    bool checkWord(const std::string &answer) override {
        return checkLetters(answer);
    }
};