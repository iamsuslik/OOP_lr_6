#include "../include/factory.h"
#include "../include/slaver.h"
#include "../include/knight.h"
#include "../include/squirrel.h"
#include "../include/observer.h"
#include <iostream>

std::shared_ptr<NPC> Factory::CreateNPC(NpcType type, int x, int y, const std::string& name) {
    std::shared_ptr<NPC> result;
    try {
        switch (type) {
            case SquirrelType: result = std::make_shared<Squirrel>(x, y, name); break;
            case SlaverType:   result = std::make_shared<Slaver>(x, y, name); break;
            case KnightType:   result = std::make_shared<Knight>(x, y, name); break;
            default: break;
        }
    } catch (...) {
        return nullptr;
    }

    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }
    return result;
}

std::shared_ptr<NPC> Factory::CreateNPC(std::istream &is) {
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type) {
        try {
            switch (type) {
                case SquirrelType: result = std::make_shared<Squirrel>(is); break;
                case SlaverType:   result = std::make_shared<Slaver>(is); break;
                case KnightType:   result = std::make_shared<Knight>(is); break;
                default: break;
            }
        } catch (...) {
            return nullptr;
        }
    }
    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }
    return result;
}
