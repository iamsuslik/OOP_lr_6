#include "include/npc.h"
#include "include/slaver.h"
#include "include/knight.h"
#include "include/squirrel.h"

#include <memory>
#include <random>
#include <fstream>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>


std::string NpcTypeToString(NpcType type) {
    switch (type) {
        case SquirrelType: return "Squirrel";
        case SlaverType: return "Slaver";
        case KnightType: return "Knight";
        default: return "Unknown";
    }
}



class TextObserver : public IFightObserver
{
private:
    TextObserver(){};
public:
    static std::shared_ptr<IFightObserver> get()
    {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (win)
        {
            std::cout << std::endl << "Murder --------" << std::endl;
            std::cout << "Killer: ";
            attacker->print();
            std::cout << "Victim: ";
            defender->print();
        }
    }
};

class FileObserver : public IFightObserver
{
private:
    std::ofstream file;
    FileObserver()
    {
        file.open("log.txt", std::ios::app);
    }
public:
    static std::shared_ptr<IFightObserver> get()
    {
        static FileObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (win)
        {
            file << std::endl << "Murder --------" << std::endl;
            file << "Killer: ";

            switch(attacker->get_type()) {
                case SlaverType:
                    file << "Slaver";
                    break;
                case KnightType:
                    file << "Knight";
                    break;
                case SquirrelType:
                    file << "Squirrel";
                    break;
                default:
                    file << "Unknown";
            }
            file << " \"" << attacker->get_name() << "\" {x:" << attacker->get_x() << ", y:" << attacker->get_y() << "}" << std::endl;
            file << "Victim: ";

            switch(defender->get_type()) {
                case SlaverType:
                    file << "Slaver";
                    break;
                case KnightType:
                    file << "Knight";
                    break;
                case SquirrelType:
                    file << "Squirrel";
                    break;
                default:
                    file << "Unknown";
            }
            file << " \"" << defender->get_name() << "\" {x:" << defender->get_x() << ", y:" << defender->get_y() << "}" << std::endl;
        }
    }
};


std::shared_ptr<NPC> factory(std::istream &is)
{
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type)
    {
        try {
            switch (static_cast<NpcType>(type))
            {
            case SquirrelType:
                result = std::make_shared<Squirrel>(is);
                break;
            case SlaverType:
                result = std::make_shared<Slaver>(is);
                break;
            case KnightType:
                result = std::make_shared<Knight>(is);
                break;
            default:
                std::cerr << "Unexpected NPC type: " << type << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error loading NPC: " << e.what() << std::endl;
        }
    }

    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, int x, int y, const std::string& name)
{
    std::shared_ptr<NPC> result;
    try {
        switch (type)
        {
        case SquirrelType:
            result = std::make_shared<Squirrel>(x, y, name);
            break;
        case SlaverType:
            result = std::make_shared<Slaver>(x, y, name);
            break;
        case KnightType:
            result = std::make_shared<Knight>(x, y, name);
            break;
        default:
            std::cerr << "Unknown NPC type: " << static_cast<int>(type) << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error creating NPC: " << e.what() << std::endl;
    }

    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

void save(const set_t &array, const std::string &filename)
{
    std::ofstream fs(filename);
    if (!fs.is_open()) {
        std::cerr << "Error opening file for saving: " << filename << std::endl;
        return;
    }
    fs << array.size() << std::endl;
    for (auto &n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load(const std::string &filename)
{
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open())
    {
        int count;
        if (is >> count) {
            for (int i = 0; i < count; ++i) {
                auto npc = factory(is);
                if (npc) {
                    result.insert(npc);
                } else {
                }
            }
        }
        is.close();
    }
    else
        std::cerr << "Error opening file for loading: " << filename << ". " << std::strerror(errno) << std::endl;
    return result;
}

std::ostream &operator<<(std::ostream &os, const set_t &array) {
    for (auto &n : array)
        n->print();
    return os;
}

set_t fight(const set_t &array, size_t distance)
{
    set_t dead_list;

    for (const auto &attacker : array)
        for (const auto &defender : array)
            if ((attacker != defender) && 
                (attacker->is_close(defender, distance)) &&
                !dead_list.count(defender) &&
                !dead_list.count(attacker))
            {
                bool success = defender->accept(attacker); 
                if (success)
                    dead_list.insert(defender);
            }

    return dead_list;
}

std::string generate_name() {
    static std::vector<std::string> names = {
        "Rab", "Groz", "Kharon", "Vand", "Siriu", "Cheshir", 
        "Sir", "Lady", "Spark", "Ezhik", "Robin", "Kira"
    };
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, names.size() - 1);
    return names[dis(gen)] + "_" + std::to_string(rd() % 1000);
}

int main()
{
    // Очищаем лог-файл при старте
    std::ofstream("log.txt", std::ios::trunc).close(); 
    
    set_t array;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> coord_dis(0, 500);
    std::uniform_int_distribution<> type_dis(1, 3);

    for (size_t i = 0; i < 20; ++i) {
        array.insert(factory(
            static_cast<NpcType>(type_dis(gen)),
            coord_dis(gen),
            coord_dis(gen),
            generate_name()
        ));
    }

    save(array, "npc.txt");
    array.clear();
    array = load("npc.txt");

    std::cout << "Current NPCs" << std::endl;
    std::cout << array;

    std::cout << "\nBattle Mode" << std::endl;
    for (size_t distance = 20; (distance <= 100) && !array.empty(); distance += 20) {
        auto dead_list = fight(array, distance);

        for (auto &d : dead_list){
            array.erase(d);
        }

        std::cout << "Battle stats ----------" << std::endl;
        std::cout << "Distance:" << distance << "m" << std::endl;
        std::cout << "Killed:" << dead_list.size() << std::endl;
        std::cout << "Survivors:" << array.size() << std::endl << std::endl;
    }

    std::cout << "\nFinal Survivors" << std::endl;
    if (array.empty()) {
        std::cout << "No survivors left" << std::endl;
    } else {
        std::cout << array;
    }

    return 0;
}
