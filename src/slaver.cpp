#include "../include/slaver.h"
#include "../include/visitor.h"
#include <iostream>

Slaver::Slaver(int x, int y, const std::string& name) : NPC(SlaverType, x, y, name) {}
Slaver::Slaver(std::istream &is) : NPC(SlaverType, is) {}

void Slaver::print() { 
    std::cout << *this; 
}

void Slaver::save(std::ostream &os) {
    os << SlaverType << " ";
    NPC::save(os);
}

bool Slaver::accept(std::shared_ptr<IFightVisitor> visitor) {
    return visitor->visit(std::dynamic_pointer_cast<Slaver>(shared_from_this()));
}

std::ostream &operator<<(std::ostream &os, Slaver &slaver) {
    os << "Slaver: " << *static_cast<NPC *>(&slaver) << std::endl;
    return os;
}
