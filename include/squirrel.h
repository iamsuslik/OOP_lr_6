#pragma once
#include "npc.h"

class Squirrel final : public NPC
{
public:
    Squirrel(int x, int y, const std::string& name);
    Squirrel(std::istream &is); 

    void print() override;
    void save(std::ostream &os) override;

    bool accept(std::shared_ptr<IFightVisitor> visitor) override;

    friend std::ostream &operator<<(std::ostream &os, Squirrel &squirrel);
};
