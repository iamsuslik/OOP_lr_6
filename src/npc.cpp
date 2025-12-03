#include "../include/npc.h"
#include "../include/slaver.h"
#include "../include/knight.h"
#include "../include/squirrel.h"
#include <stdexcept>
#include <cmath>

NPC::NPC(NpcType t, int _x, int _y, const std::string& _name) 
: type(t), x(_x), y(_y), name(_name) 
{
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::runtime_error("Coordinates must be in range 0-500");
    }
}

NPC::NPC(NpcType t, std::istream &is) : type(t)
{
    is >> x;
    is >> y;
    is >> name;
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::runtime_error("Coordinates must be in range 0-500");
    }
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer)
{
    observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> attacker, bool win)
{
    for (auto &o : observers) {
        if (o) {
            o->on_fight(attacker, shared_from_this(), win);
        }
    }
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const
{
    if (std::pow(x - other->x, 2) + std::pow(y - other->y, 2) <= std::pow(distance, 2))
        return true;
    else
        return false;
}

void NPC::save(std::ostream &os)
{
    os << x << std::endl;
    os << y << std::endl;
    os << name << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc)
{
    os << "{name: \"" << npc.name << "\", x:" << npc.x << ", y:" << npc.y << "}";
    return os;
}
