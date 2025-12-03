#include "../include/squirrel.h"
#include "../include/slaver.h"
#include "../include/knight.h"
#include <sstream>

Squirrel::Squirrel(int x, int y, const std::string& name) : NPC(SquirrelType, x, y, name) {}
Squirrel::Squirrel(std::istream &is) : NPC(SquirrelType, is) {}

void Squirrel::print() {
    std::cout << *this << std::endl;
}

void Squirrel::save(std::ostream &os) {
    os << SquirrelType << " ";
    NPC::save(os);
}

bool Squirrel::accept(const std::shared_ptr<NPC>& attacker) {
    auto self = std::static_pointer_cast<Squirrel>(shared_from_this());
    
    if (auto casted = std::dynamic_pointer_cast<Slaver>(attacker)) {
        return casted->fight(self);
    } 
    if (auto casted = std::dynamic_pointer_cast<Knight>(attacker)) {
        return casted->fight(self);
    } 
    if (auto casted = std::dynamic_pointer_cast<Squirrel>(attacker)) {
        return casted->fight(self);
    } 
    
    return false;
}

bool Squirrel::fight(std::shared_ptr<Slaver> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), false);
    return false;
}

bool Squirrel::fight(std::shared_ptr<Knight> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), false);
    return false;
}

bool Squirrel::fight(std::shared_ptr<Squirrel> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), true);
    return true;
}
