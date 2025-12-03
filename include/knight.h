#pragma once
#include "npc.h"

class Knight final : public NPC
{
public:
    Knight(int x, int y, const std::string& name);
    Knight(std::istream &is); 

    void print() override;
    void save(std::ostream &os) override;

    bool accept(std::shared_ptr<IFightVisitor> visitor) override;

    friend std::ostream &operator<<(std::ostream &os, Knight &knight);
};
