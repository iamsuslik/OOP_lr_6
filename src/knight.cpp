#include "../include/knight.h"
#include "../include/slaver.h"
#include "../include/squirrel.h"
#include <sstream>

Knight::Knight(int x, int y, const std::string& name) : NPC(KnightType, x, y, name) {}
Knight::Knight(std::istream &is) : NPC(KnightType, is) {}

void Knight::print() {
    std::cout << *this << std::endl;
}

void Knight::save(std::ostream &os) {
    os << KnightType << " ";
    NPC::save(os);
}

bool Knight::accept(const std::shared_ptr<NPC>& attacker) {
    auto self = std::static_pointer_cast<Knight>(shared_from_this());
    
    if (auto casted = std::dynamic_pointer_cast<Knight>(attacker)) {
        return casted->fight(self);
    }
    if (auto casted = std::dynamic_pointer_cast<Slaver>(attacker)) {
        return casted->fight(self);
    }
    if (auto casted = std::dynamic_pointer_cast<Squirrel>(attacker)) {
        return casted->fight(self);
    }
    
    return false;
}

bool Knight::fight(std::shared_ptr<Slaver> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), true);
    return true;
}

bool Knight::fight(std::shared_ptr<Knight> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), false);
    return false;
}

bool Knight::fight(std::shared_ptr<Squirrel> other) {
    if (!other) return false;
    other->fight_notify(shared_from_this(), false);
    return false;
}
