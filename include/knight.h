#pragma once
#include "npc.h"

class Knight final : public NPC
{
public:
    Knight(int x, int y, const std::string& name);
    Knight(std::istream &is); 

    void print() override;
    void save(std::ostream &os) override;

    bool accept(const std::shared_ptr<NPC>& attacker) override;

    friend class Slaver;
    friend class Squirrel;

protected: 
    bool fight(std::shared_ptr<Slaver> other) override;
    bool fight(std::shared_ptr<Knight> other) override;
    bool fight(std::shared_ptr<Squirrel> other) override;
};
