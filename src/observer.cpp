#include "../include/observer.h"
#include <iostream>

static std::string NpcTypeToString(NpcType type) {
    switch (type) {
        case SquirrelType: return "Squirrel";
        case SlaverType: return "Slaver";
        case KnightType: return "Knight";
        default: return "Unknown";
    }
}

std::shared_ptr<IFightObserver> TextObserver::get() {
    static TextObserver instance;
    return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
}

void TextObserver::on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) {
    if (win) {
        std::cout << "\nMurder --------" << std::endl;
        std::cout << "Killer: ";
        attacker->print();
        std::cout << "Victim: ";
        defender->print();
    }
}

FileObserver::FileObserver() {
    file.open("log.txt", std::ios::app);
}

std::shared_ptr<IFightObserver> FileObserver::get() {
    static FileObserver instance;
    return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
}

void FileObserver::on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) {
    if (win) {
        file << "\nMurder --------" << std::endl;
        file << "Killer: " << NpcTypeToString(attacker->get_type()) 
             << " \"" << attacker->get_name() << "\" {x:" << attacker->get_x() << ", y:" << attacker->get_y() << "}" << std::endl;
        
        file << "Victim: " << NpcTypeToString(defender->get_type()) 
             << " \"" << defender->get_name() << "\" {x:" << defender->get_x() << ", y:" << defender->get_y() << "}" << std::endl;
    }
}
