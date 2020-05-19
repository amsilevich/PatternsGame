#pragma once

class AbstractUnits {
protected:
    double health = 0, maxHealth = 0;
    int cost = 0,
        count = 0;
public:
    double healthSquad() {
        return count * health;
    }
    void getDamage(const double &damage) {
        if (!count) return;
        health -= damage;
    }
    void getHeal(const double &points) {
        health = std::min(maxHealth, health + points);
    }
    int costSquad() {
        return count * cost;
    }
    int countUnits() {
        return count;
    }
    int costUnit() {
        return cost;
    }
    bool isAlive() {
        return health > 0 && count > 0;
    }
    virtual double powerSquad() {};
    virtual int healSquad() {};
};

class Healers: public AbstractUnits {
private:
    int heal = 5;
public:
    Healers(int _count = 0) {
        health = 10;
        maxHealth = 30;
        cost = 10;
        count = _count;
    }
    int healSquad() override {
        return count * heal;
    }
};

class Giants: public AbstractUnits {
private:
    double attack = 5;
public:
    Giants(int _count = 0) {
        health = 70;
        maxHealth = 70;
        attack = 5;
        cost = 15;
        count = _count;
    }
    double powerSquad() override {
        return count * attack;
    }
};

class Archers: public AbstractUnits {
private:
    double attack = 0;
public:
    Archers(int _count = 0) {
        health = 15;
        maxHealth = 15;
        attack = 7;
        cost = 5;
        count = _count;
    }
    double powerSquad() override {
        return count * attack;
    }
};