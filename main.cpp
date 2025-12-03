#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include "include/factory.h"
#include "include/visitor.h"

void printMenu() {
    std::cout << "\n=== Balagur Fate 3 - Dungeon Editor ===\n";
    std::cout << "1. Add new NPC\n";
    std::cout << "2. Show all NPCs\n";
    std::cout << "3. Save NPCs to file\n";
    std::cout << "4. Load NPCs from file\n";
    std::cout << "5. Start battle mode\n";
    std::cout << "6. Show battle log (log.txt)\n";
    std::cout << "7. Clear battle log\n";
    std::cout << "8. Exit\n";
    std::cout << "Choose option: ";
}

void addNPC(std::vector<std::shared_ptr<NPC>>& npcs) {
    int type, x, y;
    std::string name;
    
    std::cout << "\n--- Add NPC ---\n";
    std::cout << "NPC types:\n";
    std::cout << "1. Squirrel\n";
    std::cout << "2. Slaver\n";
    std::cout << "3. Knight\n";
    std::cout << "Select type (1-3): ";
    std::cin >> type;
    
    std::cout << "Enter X coordinate (0-500): ";
    std::cin >> x;
    std::cout << "Enter Y coordinate (0-500): ";
    std::cin >> y;
    
    std::cout << "Enter name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        std::cout << "Coordinates must be between 0 and 500!\n";
        return;
    }
    
    if (type < 1 || type > 3) {
        std::cout << "Invalid type!\n";
        return;
    }
    
    auto npc = Factory::CreateNPC(static_cast<NpcType>(type), x, y, name);
    if (npc) {
        npcs.push_back(npc);
        
        // Записываем в лог создание NPC
        std::ofstream logfile("log.txt", std::ios::app);
        auto now = std::time(nullptr);
        logfile << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S] ");
        logfile << "Created NPC: ";
        
        switch (type) {
            case 1: logfile << "Squirrel"; break;
            case 2: logfile << "Slaver"; break;
            case 3: logfile << "Knight"; break;
        }
        logfile << " \"" << name << "\" at (" << x << ", " << y << ")\n";
        logfile.close();
        
        std::cout << "NPC added successfully!\n";
    } else {
        std::cout << "Failed to create NPC!\n";
    }
}

void showNPCs(const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::cout << "\n--- All NPCs ---\n";
    if (npcs.empty()) {
        std::cout << "No NPCs found.\n";
        return;
    }
    
    int squirrel_count = 0, slaver_count = 0, knight_count = 0;
    
    for (const auto& npc : npcs) {
        npc->print();
        switch (npc->get_type()) {
            case SquirrelType: squirrel_count++; break;
            case SlaverType: slaver_count++; break;
            case KnightType: knight_count++; break;
        }
    }
    
    std::cout << "\nTotal: " << npcs.size() << " NPCs\n";
    std::cout << "Squirrels: " << squirrel_count << "\n";
    std::cout << "Slavers: " << slaver_count << "\n";
    std::cout << "Knights: " << knight_count << "\n";
}

void saveToFile(const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::string filename;
    std::cout << "Enter filename to save: ";
    std::cin >> filename;
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Cannot open file!\n";
        return;
    }
    
    for (const auto& npc : npcs) {
        npc->save(file);
    }
    
    file.close();
    
    // Записываем в лог сохранение
    std::ofstream logfile("log.txt", std::ios::app);
    auto now = std::time(nullptr);
    logfile << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S] ");
    logfile << "Saved " << npcs.size() << " NPCs to file: " << filename << "\n";
    logfile.close();
    
    std::cout << "Saved " << npcs.size() << " NPCs to " << filename << "\n";
}

void loadFromFile(std::vector<std::shared_ptr<NPC>>& npcs) {
    std::string filename;
    std::cout << "Enter filename to load: ";
    std::cin >> filename;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Cannot open file!\n";
        return;
    }
    
    int loaded = 0;
    npcs.clear();
    
    while (file.peek() != EOF) {
        auto npc = Factory::CreateNPC(file);
        if (npc) {
            npcs.push_back(npc);
            loaded++;
        } else {
            std::cout << "Warning: Failed to load some NPCs\n";
            break;
        }
    }
    
    file.close();
    
    // Записываем в лог загрузку
    std::ofstream logfile("log.txt", std::ios::app);
    auto now = std::time(nullptr);
    logfile << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S] ");
    logfile << "Loaded " << loaded << " NPCs from file: " << filename << "\n";
    logfile.close();
    
    std::cout << "Loaded " << loaded << " NPCs from " << filename << "\n";
}

void startBattle(std::vector<std::shared_ptr<NPC>>& npcs) {
    if (npcs.size() < 2) {
        std::cout << "Need at least 2 NPCs for battle!\n";
        return;
    }
    
    int distance;
    std::cout << "Enter battle distance (meters): ";
    std::cin >> distance;
    
    std::cout << "\n=== BATTLE MODE ===\n";
    std::cout << "Starting battle with " << npcs.size() << " NPCs\n";
    std::cout << "Battle distance: " << distance << " meters\n";
    
    // Записываем в лог начало битвы
    std::ofstream logfile("log.txt", std::ios::app);
    auto now = std::time(nullptr);
    logfile << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S] ");
    logfile << "=== BATTLE STARTED ===\n";
    logfile << "NPCs: " << npcs.size() << ", Distance: " << distance << "m\n";
    logfile.close();
    
    int round = 1;
    bool battle_occurred = false;
    
    // Создаем копию для безопасной итерации
    auto npcs_copy = npcs;
    auto it = npcs_copy.begin();
    
    while (it != npcs_copy.end()) {
        auto attacker = *it;
        auto jt = npcs_copy.begin();
        while (jt != npcs_copy.end()) {
            if (it != jt) {
                auto defender = *jt;
                if (attacker->is_close(defender, distance)) {
                    std::cout << "\nRound " << round++ << ": ";
                    std::cout << attacker->get_name() << " attacks " << defender->get_name() << "\n";
                    
                    auto visitor = std::make_shared<BattleVisitor>(attacker);
                    if (defender->accept(visitor)) {
                        // Defender killed
                        std::cout << "  -> " << attacker->get_name() << " killed " 
                                  << defender->get_name() << "!\n";
                        
                        battle_occurred = true;
                        
                        // Удаляем из основного вектора
                        auto pos = std::find(npcs.begin(), npcs.end(), defender);
                        if (pos != npcs.end()) {
                            npcs.erase(pos);
                        }
                        
                        // Удаляем из копии
                        jt = npcs_copy.erase(jt);
                        continue;
                    } else {
                        std::cout << "  -> " << attacker->get_name() << " failed to kill " 
                                  << defender->get_name() << "\n";
                    }
                }
            }
            ++jt;
        }
        ++it;
    }
    
    // Записываем в лог окончание битвы
    logfile.open("log.txt", std::ios::app);
    now = std::time(nullptr);
    logfile << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S] ");
    logfile << "Battle finished. Remaining NPCs: " << npcs.size() << "\n";
    logfile << "=== BATTLE ENDED ===\n\n";
    logfile.close();
    
    if (!battle_occurred) {
        std::cout << "\nNo battles occurred (NPCs were too far apart)\n";
    }
    
    std::cout << "\nBattle finished!\n";
    std::cout << "Remaining NPCs: " << npcs.size() << "\n";
}

void showBattleLog() {
    std::cout << "\n--- Battle Log (log.txt) ---\n";
    
    std::ifstream logfile("log.txt");
    if (!logfile.is_open()) {
        std::cout << "Log file not found or empty.\n";
        return;
    }
    
    std::string line;
    bool hasContent = false;
    
    while (std::getline(logfile, line)) {
        std::cout << line << "\n";
        hasContent = true;
    }
    
    logfile.close();
    
    if (!hasContent) {
        std::cout << "Log file is empty.\n";
    }
}

void clearBattleLog() {
    std::ofstream logfile("log.txt", std::ios::trunc);
    logfile.close();
    
    std::cout << "Battle log cleared!\n";
    
    // Записываем само событие очистки
    logfile.open("log.txt", std::ios::app);
    auto now = std::time(nullptr);
    logfile << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S] ");
    logfile << "Log file cleared by user\n";
    logfile.close();
}

void printBattleRules() {
    std::cout << "\n=== Battle Rules ===\n";
    std::cout << "1. Slaver kills Squirrel\n";
    std::cout << "2. Knight kills Slaver\n";
    std::cout << "3. Squirrel kills Squirrel\n";
    std::cout << "4. All other combinations: no kill\n";
    std::cout << "====================\n";
}

int main() {
    std::vector<std::shared_ptr<NPC>> npcs;
    int choice = 0;
    
    std::cout << "========================================\n";
    std::cout << "   Welcome to Balagur Fate 3\n";
    std::cout << "       Dungeon Editor\n";
    std::cout << "========================================\n";
    
    // Создаем начальный лог файл
    std::ofstream logfile("log.txt", std::ios::app);
    auto now = std::time(nullptr);
    logfile << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S] ");
    logfile << "=== Program Started ===\n";
    logfile.close();
    
    // Пример начальных NPC
    npcs.push_back(Factory::CreateNPC(SlaverType, 100, 100, "Slaver1"));
    npcs.push_back(Factory::CreateNPC(KnightType, 200, 200, "Knight1"));
    npcs.push_back(Factory::CreateNPC(SquirrelType, 150, 150, "Squirrel1"));
    npcs.push_back(Factory::CreateNPC(SquirrelType, 160, 160, "Squirrel2"));
    
    std::cout << "\nCreated 4 example NPCs.\n";
    std::cout << "Battle events will be logged to 'log.txt'\n";
    
    printBattleRules();
    
    while (choice != 8) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input!\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                addNPC(npcs);
                break;
            case 2:
                showNPCs(npcs);
                break;
            case 3:
                saveToFile(npcs);
                break;
            case 4:
                loadFromFile(npcs);
                break;
            case 5:
                startBattle(npcs);
                break;
            case 6:
                showBattleLog();
                break;
            case 7:
                clearBattleLog();
                break;
            case 8:
                {
                    std::cout << "\nGoodbye!\n";
                    std::ofstream logfile("log.txt", std::ios::app);
                    auto now = std::time(nullptr);
                    logfile << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S] ");
                    logfile << "=== Program Exited ===\n\n";
                    logfile.close();
                }
                break;
            default:
                std::cout << "Invalid option! Please choose 1-8.\n";
        }
    }
    
    return 0;
}
