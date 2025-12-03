#include "../include/slaver.h"
#include "../include/knight.h"
#include "../include/squirrel.h"
#include <sstream>

Slaver::Slaver(int x, int y, const std::string& name) : NPC(SlaverType, x, y, name) {}
Slaver::Slaver(std::istream &is) : NPC(SlaverType, is) {}

void Slaver::print() {
    std::cout << *this << std::endl;
}

void Slaver::save(std::ostream &os) {
    os << SlaverType << " ";
    NPC::save(os);
}

bool Slaver::accept(const std::shared_ptr<NPC>& attacker) {
    auto self = std::static_pointer_cast<Slaver>(shared_from_this());
    
    if (auto casted = std::dynamic_pointer_cast<Knight>(attacker)) {
        return casted->fight(self);
    } 
    if (auto casted = std::dynamic_pointer_cast<Squirrel>(attacker)) {
        return casted->fight(self);
    } 
    if (auto casted = std::dynamic_pointer_cast<Slaver>(attacker)) {
        return casted->fight(self);
    } 
    
    return false;
}

bool Slaver::fight(std::shared_ptr<Slaver> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), false);
    return false;
}

bool Slaver::fight(std::shared_ptr<Knight> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), false);
    return false;
}

bool Slaver::fight(std::shared_ptr<Squirrel> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), true);
    return true;
}
