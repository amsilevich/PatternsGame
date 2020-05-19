#pragma once

#include <vector>
#include <string>
#include "src/unit/Units.h"
#include "src/input/checkInput.h"

class Monster;

class Component {
protected:
public:
    std::string name;
    virtual ~Component() {}
    virtual int cost() {}//+//+
    virtual double power() {}//+//+
    virtual bool isAlive() {}//+//+
    virtual bool isComposite() {}//+
    virtual int heal() {}//+//+
    virtual void insert(Component* newDivision) {}//+//+
    virtual bool unite(const std::vector<std::string> &armies, const std::string &name) {}//+
    virtual void getHeal(double hp = -1) {}//+//+
    virtual void getDamage(const double &damage) {}//+//+
    virtual void write(std::string s = "") {}//+//+
    virtual int countUnits() {}//+//+
    virtual void update() {}//+//+
};

class Leaf: public Component {
private:
    Giants G;
    Archers A;
    Healers H;
public:
    Leaf(const std::string &s) {
        GetNameDecorator get;
        std::string inputName = get.returnInput();
        name = inputName;
        GetUnits create;
        std::vector<int> inputUnits = create.returnInput();
        G = Giants(inputUnits[0]);
        A = Archers(inputUnits[1]);
        H = Healers(inputUnits[2]);
    }
    int cost() override {
        return G.costSquad() + A.costSquad() + H.costSquad();
    }
    double power() override {
        return G.powerSquad() + A.powerSquad();
    }
    bool isAlive() override {
        return G.isAlive() + A.isAlive() + H.isAlive();
    }
    bool isComposite() override {
        return false;
    }
    int heal() override {
        return H.healSquad();
    }
    void getHeal(double hp = -1) override {
        if (hp == -1) {
            hp = H.healSquad() / (double) (A.countUnits() + G.countUnits());
        }
        else {
            H.getHeal(hp);
        }
        G.getHeal(hp);
        A.getHeal(hp);
    }
    void getDamage(const double &damage) override {
        G.getDamage(damage);
        A.getDamage(damage);
        H.getDamage(damage);
    }
    void write(std::string s = "") override {
        std::cout << s << name << ":" << std::endl;
        std::cout << s << "  " << "Giants: " << G.countUnits() << std::endl;
        std::cout << s << "  " << "Archers: " << A.countUnits() << std::endl;
        std::cout << s << "  " << "Healers: " << H.countUnits() << std::endl;
    }
    int countUnits() override {
        return G.countUnits() + A.countUnits() + H.countUnits();
    }
};

class Composite: public Component {
private:
    std::vector<Component*> children;
public:
    Composite(std::string _name) {
        name = _name;
    }
    Composite(const std::vector<Component*> &_children, const std::string &_name) {
        children = _children;
        name = _name;
    }
    int cost() override {
        int costArmy = 0;
        for (auto i: children) {
            costArmy += i->cost();
        }
        return costArmy;
    }
    double power() override {
        double powerArmy = 0;
        for (auto i: children) {
            powerArmy += i->power();
        }
        powerArmy *= 1.25;
        return powerArmy;
    }
    bool isAlive() override {
        if (children.empty()) {
            return 0;
        }
        bool isAliveArmy = true;
        for (auto i: children) {
            isAliveArmy = std::min(isAliveArmy, i->isAlive());
        }
        return isAliveArmy;
    }
    bool isComposite() override {
        return true;
    }
    int heal() override {
        int healArmy = 0;
        for (auto i: children) {
            healArmy += i->heal();
        }
        return healArmy;
    }
    void insert(Component* newDivision) override {
        children.push_back(newDivision);
    }
    void getHeal(double hp = -1) override {
        for (auto i: children) {
            i->getHeal(hp);
        }
    }
    void getDamage(const double &damage) override {
        for (auto i : children) {
            i->getDamage(damage);
        }
    }
    void write(std::string indent = "") override {
        std::cout << indent << name << ":" << std::endl;
        indent += "  ";
        for (auto i : children) {
            i->write(indent);
        }
        indent.pop_back();
        indent.pop_back();
    }
    int countUnits() override {
        int count = 0;
        for (auto i: children) {
            count += i->countUnits();
        }
        return count;
    }
    void update() override {
        for (auto i : children) {
            if (isComposite()) {
                i->update();
            }
        }
        std::vector<Component*> newChildren;
        for (auto i : children) {
            if (i->isAlive()) {
                newChildren.push_back(i);
            }
        }
        children = newChildren;
    }
    bool unite(const std::vector<std::string> &armies, const std::string &unionName) override {
        bool can = false;
        for (auto i: children) {
            if (i->isComposite()) {
                can = std::max(can, i->unite(armies, unionName));
            }
        }
        if (can) {
            return true;
        }
        std::vector<Component*> newChildren, disconnectedArmies;
        for (auto i : children) {
            if (stringInArray(i->name, armies)) {
                disconnectedArmies.push_back(i);
            }
            else {
                newChildren.push_back(i);
            }
        }
        if (disconnectedArmies.size() == armies.size()) {
            Composite* newArmy = new Composite(disconnectedArmies, unionName);
            newChildren.push_back(newArmy);
            children = newChildren;
            return true;
        }
        return false;
    }
};

class Monster {
private:
    int health = 300;
    int maxHealth = 300;
    int damage = 100;
    int lvl = 1;
    int price = 150;
public:
    void attack(Component* army) {
        army->getDamage(damage / (double) army->countUnits());
    }
    void getDamage(Component* army) {
        health -= army->power();
    }
    void getDamage(const int &damage) {
        health -= damage;
    }
    int lvlMonster() {
        return lvl;
    }
    int killingPrice() {
        return price;
    }
    bool isAlive() {
        return health > 0;
    }
    void upgrade() {
        ++lvl;
        maxHealth += 200;
        health = maxHealth;
        damage += 100;
        price += 150;
    }
    void write() {
        std::cout << "Monster:" << std::endl;
        std::cout << "   Here i am!!! Huge evil monster who wants to eat youuuu!!" << std::endl;
        std::cout << "      My health: " << health << std::endl;
        std::cout << "      My power: " << damage << std::endl;
        std::cout << "      My level: " << lvl << std::endl;
        std::cout << "      Award of killing me: " << price << std::endl;
    }
};