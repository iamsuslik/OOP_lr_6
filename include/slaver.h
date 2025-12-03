#pragma once
#include "npc.h"

class Slaver final : public NPC
{
public:
    Slaver(int x, int y, const std::string& name);
    Slaver(std::istream &is); 

    void print() override;
    void save(std::ostream &os) override;

    bool accept(std::shared_ptr<IFightVisitor> visitor) override;

    friend std::ostream &operator<<(std::ostream &os, Slaver &slaver);
};
