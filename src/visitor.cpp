#include "../include/visitor.h"

BattleVisitor::BattleVisitor(std::shared_ptr<NPC> attacker) 
    : attacker_type(attacker->get_type()), attacker(attacker) {}

bool BattleVisitor::visit(std::shared_ptr<Slaver> other) {
    // Работорговец защищается
    if (attacker_type == KnightType) {
        // Рыцарь убивает работорговца
        other->fight_notify(attacker, true);
        return true;
    }
    else if (attacker_type == SquirrelType) {
        // Белка не убивает работорговца
        other->fight_notify(attacker, false);
        return false;
    }
    else if (attacker_type == SlaverType) {
        // Работорговец не убивает работорговца
        other->fight_notify(attacker, false);
        return false;
    }
    return false;
}

bool BattleVisitor::visit(std::shared_ptr<Knight> other) {
    // Рыцарь защищается
    if (attacker_type == SquirrelType) {
        // Белка не убивает рыцаря
        other->fight_notify(attacker, false);
        return false;
    }
    else if (attacker_type == SlaverType) {
        // Работорговец не убивает рыцаря
        other->fight_notify(attacker, false);
        return false;
    }
    else if (attacker_type == KnightType) {
        // Рыцарь не убивает рыцаря
        other->fight_notify(attacker, false);
        return false;
    }
    return false;
}

bool BattleVisitor::visit(std::shared_ptr<Squirrel> other) {
    // Белка защищается
    if (attacker_type == SlaverType) {
        // Работорговец убивает белку
        other->fight_notify(attacker, true);
        return true;
    }
    else if (attacker_type == KnightType) {
        // Рыцарь не убивает белку
        other->fight_notify(attacker, false);
        return false;
    }
    else if (attacker_type == SquirrelType) {
        // Белка убивает белку
        other->fight_notify(attacker, true);
        return true;
    }
    return false;
}
