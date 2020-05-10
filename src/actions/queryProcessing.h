#pragma once

#include "src/components/structureArmy.h"
#include "src/input/checkInput.h"
#include <iostream>
#include <vector>

class Handler {
protected:
    bool answer() {
        GetAnswerDecorator getAnswer;
        std::string command = getAnswer.returnInput();
        return command == "yes";
    }
    virtual void askQuestion(const int &coins) = 0;
public:
    virtual Handler* setNext(Handler* handler) = 0;
    virtual void Handle(Component* army, Monster* monster, int &cost) = 0;
};

class AbstractHandler: public Handler {
protected:
    Handler* nextHandler = nullptr;
public:
    Handler* setNext(Handler* handler) override {
        nextHandler = handler;
        return handler;
    }
    virtual void Handle(Component* army, Monster* monster, int &cost) override {
        if (nextHandler) {
            nextHandler->Handle(army, monster, cost);
        }
    }
};

class CreateArmyHandler: public AbstractHandler {
private:
    void askQuestion(const int &coins) override {
        std::cout << "Do you want create some troops?" << std::endl;
        std::cout << "You have " << coins << " coins" << std::endl;
    }
    bool canCreate(Component* newDivision, int &coins) {
        if (newDivision->cost() > coins) {
            std::cout << "You haven't enought money, please, create a smaller army" << std::endl;
            return 0;
        }
        return 1;
    }
    void create(Component* army, int &coins) {
        Leaf* newDivision = new Leaf("input");
        if (newDivision->isAlive() && canCreate(newDivision, coins)) {
            coins -= newDivision->cost();
            army->insert(newDivision);
        }
    }
public:
    void Handle(Component* army, Monster* monster, int &coins) override {
        askQuestion(coins);
        if (!answer()) {
            AbstractHandler::Handle(army, monster, coins);
            return;
        }
        create(army, coins);
        Handle(army, monster, coins);
    }
};

class UniteArmyHandler: public AbstractHandler {
private:
    void askQuestion(const int &coins) override {
        std::cout << "Do you want to union some armies?" << std::endl;
    }
    bool canUnite(Component* army) {
        std::cout << "So enter the name of union (one word)" << std::endl;
        GetNameDecorator getName;
        std::string name = getName.returnInput();
        GetStrings getArmies;
        std::vector<std::string> armies = getArmies.returnInput();
        if (armies.size() < 2 || !army->unite(armies, name)) {
            return false;
        }
        army->write();
        return true;
    }
public:
    void Handle(Component* army, Monster* monster, int &coins) override {
        askQuestion(coins);
        if (!answer()) {
            AbstractHandler::Handle(army, monster, coins);
            return;
        }
        if (!canUnite(army)) {
            std::cout << "Sorry, but names of armies you have written were incorrect." << std::endl;
            Handle(army, monster, coins);
        }
        else {
            AbstractHandler::Handle(army, monster, coins);
        }
    }
};

class BuySpellHandler: public AbstractHandler {
protected:
    int cost = 0;
    bool canBuy(const int &coins) {
        return coins >= cost;
    }
    virtual void useSpell(Component* army, Monster* monster) = 0;
public:
    void Handle(Component* army, Monster* monster, int &coins) override {
        askQuestion(coins);
        if (!answer()) {
            AbstractHandler::Handle(army, monster, coins);
            return;
        }
        if (!canBuy(coins)) {
            std::cout << "Unfortunately, but you can't buy "
                         "this spell, you haven't enough money" << std::endl;
        }
        else {
            coins -= cost;
            useSpell(army, monster);
            if (!monster->isAlive()) {
                return;
            }
            AbstractHandler::Handle(army, monster,coins);
        }
    }
};

class BuyHealSpellHandler: public BuySpellHandler {
private:
    int heal = 100;
    void askQuestion(const int &coins) override {
        std::cout << "Do you want to buy Healing spell?" << std::endl;
        std::cout << "You have " << coins << " coins" << std::endl;
    }
    void useSpell(Component* army, Monster* monster) override {
        army->getHeal(heal / (double) army->countUnits());
        army->write();
    }
public:
    BuyHealSpellHandler() {
        cost = 25;
    }
};

class BuyLightningSpellHandler: public BuySpellHandler {
private:
    int damage = 50;
    void askQuestion(const int &coins) override {
        std::cout << "Do you want to buy Lightning spell?" << std::endl;
        std::cout << "You have " << coins << " coins" << std::endl;
    }
    void useSpell(Component* army, Monster* monster) override {
        monster->getDamage(damage);
    }
public:
    BuyLightningSpellHandler() {
        cost = 20;
    }
};