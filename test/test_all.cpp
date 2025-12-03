#include <gtest/gtest.h>
#include "../include/npc.h"
#include "../include/slaver.h" 
#include "../include/knight.h" 
#include "../include/squirrel.h"
#include <sstream>
#include <stdexcept>

class MockObserver : public IFightObserver {
public:
    bool fight_observed = false;
    std::shared_ptr<NPC> last_attacker;
    std::shared_ptr<NPC> last_defender;
    bool last_win = false;

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override {
        fight_observed = true;
        last_attacker = attacker;
        last_defender = defender;
        last_win = win;
    }
};

class NPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        observer = std::make_shared<MockObserver>();
    }

    std::shared_ptr<MockObserver> observer;
};

// Тест 1: Создание NPC
TEST_F(NPCTest, NPCCreation) {
    auto slaver = std::make_shared<Slaver>(100, 100, "TestSlaver");
    EXPECT_EQ(slaver->get_type(), SlaverType);
    EXPECT_EQ(slaver->get_x(), 100);
    EXPECT_EQ(slaver->get_y(), 100);
    EXPECT_EQ(slaver->get_name(), "TestSlaver");
}

// Тест 2: Валидация координат
TEST_F(NPCTest, CoordinatesValidation) {
    EXPECT_THROW(std::make_shared<Slaver>(-1, 100, "BadSlaver"), std::runtime_error);
    EXPECT_THROW(std::make_shared<Slaver>(100, 600, "BadSlaver"), std::runtime_error);
}

// Тест 3: Расчет расстояния
TEST_F(NPCTest, DistanceCalculation) {
    auto npc1 = std::make_shared<Slaver>(100, 100, "Slaver1");
    auto npc2 = std::make_shared<Knight>(103, 104, "Knight1");

    EXPECT_TRUE(npc1->is_close(npc2, 5));
    EXPECT_FALSE(npc1->is_close(npc2, 4));
}

// Тест 4: Логика боя работорговца как защитника
TEST_F(NPCTest, SlaverFightLogic) {
    auto slaver = std::make_shared<Slaver>(100, 100, "Slaver1");
    auto knight = std::make_shared<Knight>(100, 100, "Knight1");
    auto squirrel = std::make_shared<Squirrel>(100, 100, "Squirrel1");
    auto slaver2 = std::make_shared<Slaver>(100, 100, "Slaver2");
    
    slaver->subscribe(observer);

    // Рыцарь убивает работорговца
    EXPECT_TRUE(slaver->accept(knight));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_TRUE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), KnightType);
    EXPECT_EQ(observer->last_defender->get_type(), SlaverType);
    observer->fight_observed = false;

    // Белка не убивает работорговца
    EXPECT_FALSE(slaver->accept(squirrel));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SquirrelType);
    EXPECT_EQ(observer->last_defender->get_type(), SlaverType);
    observer->fight_observed = false;

    // Работорговец не убивает работорговца
    EXPECT_FALSE(slaver->accept(slaver2));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SlaverType);
    EXPECT_EQ(observer->last_defender->get_type(), SlaverType);
}

// Тест 5: Логика боя рыцаря как защитника
TEST_F(NPCTest, KnightFightLogic) {
    auto knight = std::make_shared<Knight>(100, 100, "Knight1");
    auto squirrel = std::make_shared<Squirrel>(100, 100, "Squirrel1");
    auto slaver = std::make_shared<Slaver>(100, 100, "Slaver1");
    
    knight->subscribe(observer);

    // Белка не убивает рыцаря
    EXPECT_FALSE(knight->accept(squirrel));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SquirrelType);
    EXPECT_EQ(observer->last_defender->get_type(), KnightType);
    observer->fight_observed = false;

    // Работорговец не убивает рыцаря
    EXPECT_FALSE(knight->accept(slaver));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SlaverType);
    EXPECT_EQ(observer->last_defender->get_type(), KnightType);
}

// Тест 6: Логика боя белки как защитника
TEST_F(NPCTest, SquirrelFightLogic) {
    auto squirrel = std::make_shared<Squirrel>(100, 100, "Squirrel1");
    auto slaver = std::make_shared<Slaver>(100, 100, "Slaver1");
    auto knight = std::make_shared<Knight>(100, 100, "Knight1");
    auto squirrel2 = std::make_shared<Squirrel>(100, 100, "Squirrel2");
    
    squirrel->subscribe(observer);

    // Работорговец убивает белку
    EXPECT_TRUE(squirrel->accept(slaver));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_TRUE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SlaverType);
    EXPECT_EQ(observer->last_defender->get_type(), SquirrelType);
    observer->fight_observed = false;

    // Рыцарь не убивает белку
    EXPECT_FALSE(squirrel->accept(knight));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), KnightType);
    EXPECT_EQ(observer->last_defender->get_type(), SquirrelType);
    observer->fight_observed = false;

    // Белка убивает белку
    EXPECT_TRUE(squirrel->accept(squirrel2));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_TRUE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SquirrelType);
    EXPECT_EQ(observer->last_defender->get_type(), SquirrelType);
}

// Тест 7: Сохранение и загрузка
TEST_F(NPCTest, SaveAndLoad) {
    auto original_slaver = std::make_shared<Slaver>(123, 456, "TestSlaver");
    auto original_knight = std::make_shared<Knight>(321, 254, "TestKnight");

    std::stringstream ss;
    original_slaver->save(ss);
    original_knight->save(ss);

    ss.seekg(0);

    // Загрузка работорговца
    int type;
    ss >> type;
    auto loaded_slaver = std::make_shared<Slaver>(ss);

    // Загрузка рыцаря
    ss >> type;
    auto loaded_knight = std::make_shared<Knight>(ss);
    
    EXPECT_EQ(loaded_slaver->get_type(), SlaverType);
    EXPECT_EQ(loaded_slaver->get_x(), 123);
    EXPECT_EQ(loaded_slaver->get_y(), 456);
    EXPECT_EQ(loaded_slaver->get_name(), "TestSlaver");
    EXPECT_EQ(loaded_knight->get_type(), KnightType);
    EXPECT_EQ(loaded_knight->get_x(), 321);
    EXPECT_EQ(loaded_knight->get_y(), 254);
    EXPECT_EQ(loaded_knight->get_name(), "TestKnight");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
