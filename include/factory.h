#pragma once
#include "npc.h"
#include <iostream>
#include <memory>

class Factory {
public:
    static std::shared_ptr<NPC> CreateNPC(NpcType type, int x, int y, const std::string& name);
    static std::shared_ptr<NPC> CreateNPC(std::istream &is);
};
