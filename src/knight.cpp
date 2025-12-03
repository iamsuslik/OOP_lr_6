#include "../include/knight.h"
#include "../include/visitor.h"
#include <iostream>

Knight::Knight(int x, int y, const std::string& name) : NPC(KnightType, x, y, name) {}
Knight::Knight(std::istream &is) : NPC(KnightType, is) {}

void Knight::print() { 
    std::cout << *this; 
}

void Knight::save(std::ostream &os) {
    os << KnightType << " ";
    NPC::save(os);
}

bool Knight::accept(std::shared_ptr<IFightVisitor> visitor) {
    return visitor->visit(std::dynamic_pointer_cast<Knight>(shared_from_this()));
}

std::ostream &operator<<(std::ostream &os, Knight &knight) {
    os << "Knight: " << *static_cast<NPC *>(&knight) << std::endl;
    return os;
}
