#pragma once
#include "npc.h"
#include "slaver.h"
#include "knight.h"
#include "squirrel.h"

class BattleVisitor : public IFightVisitor {
private:
    NpcType attacker_type;
    std::shared_ptr<NPC> attacker;
    
public:
    BattleVisitor(std::shared_ptr<NPC> attacker);
    bool visit(std::shared_ptr<Slaver> other) override;
    bool visit(std::shared_ptr<Knight> other) override;
    bool visit(std::shared_ptr<Squirrel> other) override;
};
