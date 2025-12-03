#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <cmath>

class NPC;
class Slaver;
class Squirrel;
class Knight;
class IFightObserver;
class IFightVisitor;

using set_t = std::set<std::shared_ptr<NPC>>;

enum NpcType
{
    Unknown = 0,
    SquirrelType = 1,
    SlaverType = 2,
    KnightType = 3
};

class IFightObserver {
public:
    virtual ~IFightObserver() = default;
    virtual void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) = 0;
};

class IFightVisitor {
public:
    virtual ~IFightVisitor() = default;
    virtual bool visit(std::shared_ptr<Slaver> other) = 0;
    virtual bool visit(std::shared_ptr<Knight> other) = 0;
    virtual bool visit(std::shared_ptr<Squirrel> other) = 0;
};

class NPC : public std::enable_shared_from_this<NPC>
{
protected:
    NpcType type;
    int x{0};
    int y{0};
    std::string name;
    std::vector<std::shared_ptr<IFightObserver>> observers;

public:
    NPC(NpcType t, int _x, int _y, const std::string& _name);
    NPC(NpcType t, std::istream &is);
    virtual ~NPC() = default;

    void subscribe(std::shared_ptr<IFightObserver> observer);
    void fight_notify(const std::shared_ptr<NPC> attacker, bool win);

    // Изменено: теперь используется Visitor вместо accept с типами
    virtual bool accept(std::shared_ptr<IFightVisitor> visitor) = 0;

    bool is_close(const std::shared_ptr<NPC> &other, size_t distance) const;
    NpcType get_type() const { return type; }
    int get_x() const { return x; }
    int get_y() const { return y; }
    std::string get_name() const { return name; }
    virtual void print() = 0;
    virtual void save(std::ostream &os);

    friend std::ostream &operator<<(std::ostream &os, NPC &npc);
};
