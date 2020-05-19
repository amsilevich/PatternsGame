#include <iostream>
#include <vector>
#include "src/actions/queryProcessing.h"
#include "src/components/structureArmy.h"

class Game {
private:
    int coins = 180;
    Component* army = new Composite("Your Army");
    bool youWon = true;
    int lastLvl = 3;
    Monster* monster = new Monster;
public:
    void startMessage() {
        std::cout << "Welcome to the battlefield, brave knight! It's time to show off your skills!" << std::endl;
    }
    void killMonster() {
        std::cout << "You have beaten this monster!" << std::endl;
        coins += monster->killingPrice();
        monster->upgrade();
    }
    void AttackMessage() {
        std::cout << "FIIIGHT!!" << std::endl;
    }
    void MonsterAttackMessage() {
        std::cout << "Monster attacks!!" << std::endl;
    }
    void ArmyAttackMessage() {
        std::cout << "Army attacks!!" << std::endl;
    }
    void ArmyHealMessage() {
        std::cout << "Army gets heal from Healers!!" << std::endl;
    }
    void fight() {
        army->write();
        monster->write();
        CreateArmyHandler* CreateArmy = new CreateArmyHandler;
        UniteArmyHandler* UniteArmy = new UniteArmyHandler;
        BuyHealSpellHandler* BuyHealSpell = new BuyHealSpellHandler;
        BuyLightningSpellHandler* BuyLightningSpell = new BuyLightningSpellHandler;
        CreateArmy->setNext(UniteArmy)->setNext(BuyHealSpell)->setNext(BuyLightningSpell);
        CreateArmy->Handle(army, monster, coins);
        if (!monster->isAlive()) {
            killMonster();
            return;
        }
        AttackMessage();
        MonsterAttackMessage();
        monster->attack(army);
        if (!army->isAlive()) {
            youWon = false;
            return;
        }
        ArmyHealMessage();
        army->getHeal();
        ArmyAttackMessage();
        monster->getDamage(army);
        if (!monster->isAlive()) {
            killMonster();
            return;
        }
        this->fight();
    }
    void beatMonster() {
        if (!youWon || monster->lvlMonster() > lastLvl) {
            return;
        }
        std::cout << "Oh no! There is a huge " << monster->lvlMonster() << " lvl monster!" << std::endl;
        fight();
        beatMonster();
    }
    void play() {
        startMessage();
        beatMonster();
        if (youWon) {
            std::cout << "Yeahhh!! You have beaten all the monsters and Kingdom is rescued now!"
                      << std::endl << "You won." << std::endl;
        }
        else {
            std::cout << "Unfortunately, the monster was more powerful than you!"
                      << std::endl << "You lose." << std::endl;
        }
    }
};