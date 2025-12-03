#include "../include/visitor.h"

BattleVisitor::BattleVisitor(std::shared_ptr<NPC> attacker) 
    : attacker_type(attacker->get_type()), attacker(attacker) {}

bool BattleVisitor::visit(std::shared_ptr<Slaver> other) {
    if (attacker_type == KnightType) {
        other->fight_notify(attacker, true);
        return true;
    }
    else if (attacker_type == SquirrelType) {
        other->fight_notify(attacker, false);
        return false;
    }
    else if (attacker_type == SlaverType) {
        other->fight_notify(attacker, false);
        return false;
    }
    return false;
}

bool BattleVisitor::visit(std::shared_ptr<Knight> other) {
    if (attacker_type == SquirrelType) {
        other->fight_notify(attacker, false);
        return false;
    }
    else if (attacker_type == SlaverType) {
        other->fight_notify(attacker, false);
        return false;
    }
    else if (attacker_type == KnightType) {
        other->fight_notify(attacker, false);
        return false;
    }
    return false;
}

bool BattleVisitor::visit(std::shared_ptr<Squirrel> other) {
    if (attacker_type == SlaverType) {
        other->fight_notify(attacker, true);
        return true;
    }
    else if (attacker_type == KnightType) {
        other->fight_notify(attacker, false);
        return false;
    }
    else if (attacker_type == SquirrelType) {
        other->fight_notify(attacker, true);
        return true;
    }
    return false;
}
