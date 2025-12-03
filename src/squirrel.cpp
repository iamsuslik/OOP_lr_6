#include "../include/squirrel.h"
#include "../include/visitor.h"
#include <iostream>

Squirrel::Squirrel(int x, int y, const std::string& name) : NPC(SquirrelType, x, y, name) {}
Squirrel::Squirrel(std::istream &is) : NPC(SquirrelType, is) {}

void Squirrel::print() { 
    std::cout << *this; 
}

void Squirrel::save(std::ostream &os) {
    os << SquirrelType << " ";
    NPC::save(os);
}

bool Squirrel::accept(std::shared_ptr<IFightVisitor> visitor) {
    return visitor->visit(std::dynamic_pointer_cast<Squirrel>(shared_from_this()));
}

std::ostream &operator<<(std::ostream &os, Squirrel &squirrel) {
    os << "Squirrel: " << *static_cast<NPC *>(&squirrel) << std::endl;
    return os;
}
