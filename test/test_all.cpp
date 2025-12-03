#include <gtest/gtest.h>
#include "../include/npc.h"
#include "../include/slaver.h" 
#include "../include/knight.h" 
#include "../include/squirrel.h"
#include "../include/factory.h"
#include "../include/visitor.h"
#include "../include/observer.h"
#include <sstream>
#include <stdexcept>
#include <memory>

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

// Тест 1: Создание NPC через фабрику
TEST_F(NPCTest, FactoryNPCCreation) {
    auto slaver = Factory::CreateNPC(SlaverType, 100, 100, "TestSlaver");
    auto knight = Factory::CreateNPC(KnightType, 200, 200, "TestKnight");
    auto squirrel = Factory::CreateNPC(SquirrelType, 300, 300, "TestSquirrel");
    
    EXPECT_NE(slaver, nullptr);
    EXPECT_NE(knight, nullptr);
    EXPECT_NE(squirrel, nullptr);
    
    EXPECT_EQ(slaver->get_type(), SlaverType);
    EXPECT_EQ(knight->get_type(), KnightType);
    EXPECT_EQ(squirrel->get_type(), SquirrelType);
}

// Тест 2: Создание NPC напрямую
TEST_F(NPCTest, DirectNPCCreation) {
    auto slaver = std::make_shared<Slaver>(100, 100, "TestSlaver");
    EXPECT_EQ(slaver->get_type(), SlaverType);
    EXPECT_EQ(slaver->get_x(), 100);
    EXPECT_EQ(slaver->get_y(), 100);
    EXPECT_EQ(slaver->get_name(), "TestSlaver");
}

// Тест 3: Расчет расстояния
TEST_F(NPCTest, DistanceCalculation) {
    auto npc1 = std::make_shared<Slaver>(100, 100, "Slaver1");
    auto npc2 = std::make_shared<Knight>(103, 104, "Knight1");

    EXPECT_TRUE(npc1->is_close(npc2, 5));
    EXPECT_FALSE(npc1->is_close(npc2, 4));
}

// Тест 4: Логика боя работорговца с использованием Visitor
TEST_F(NPCTest, SlaverFightLogicWithVisitor) {
    auto slaver = std::make_shared<Slaver>(100, 100, "Slaver1");
    auto knight = std::make_shared<Knight>(100, 100, "Knight1");
    auto squirrel = std::make_shared<Squirrel>(100, 100, "Squirrel1");
    auto slaver2 = std::make_shared<Slaver>(100, 100, "Slaver2");
    
    slaver->subscribe(observer);

    // Рыцарь атакует работорговца (рыцарь убивает работорговца)
    auto knight_visitor = std::make_shared<BattleVisitor>(knight);
    EXPECT_TRUE(slaver->accept(knight_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_TRUE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), KnightType);
    EXPECT_EQ(observer->last_defender->get_type(), SlaverType);
    observer->fight_observed = false;

    // Белка атакует работорговца (белка не убивает работорговца)
    auto squirrel_visitor = std::make_shared<BattleVisitor>(squirrel);
    EXPECT_FALSE(slaver->accept(squirrel_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SquirrelType);
    EXPECT_EQ(observer->last_defender->get_type(), SlaverType);
    observer->fight_observed = false;

    // Работорговец атакует работорговца (работорговец не убивает работорговца)
    auto slaver_visitor = std::make_shared<BattleVisitor>(slaver2);
    EXPECT_FALSE(slaver->accept(slaver_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SlaverType);
    EXPECT_EQ(observer->last_defender->get_type(), SlaverType);
}

// Тест 5: Логика боя рыцаря с использованием Visitor
TEST_F(NPCTest, KnightFightLogicWithVisitor) {
    auto knight = std::make_shared<Knight>(100, 100, "Knight1");
    auto squirrel = std::make_shared<Squirrel>(100, 100, "Squirrel1");
    auto slaver = std::make_shared<Slaver>(100, 100, "Slaver1");
    auto knight2 = std::make_shared<Knight>(100, 100, "Knight2");
    
    knight->subscribe(observer);

    // Белка атакует рыцаря (белка не убивает рыцаря)
    auto squirrel_visitor = std::make_shared<BattleVisitor>(squirrel);
    EXPECT_FALSE(knight->accept(squirrel_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SquirrelType);
    EXPECT_EQ(observer->last_defender->get_type(), KnightType);
    observer->fight_observed = false;

    // Работорговец атакует рыцаря (работорговец не убивает рыцаря)
    auto slaver_visitor = std::make_shared<BattleVisitor>(slaver);
    EXPECT_FALSE(knight->accept(slaver_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SlaverType);
    EXPECT_EQ(observer->last_defender->get_type(), KnightType);
    observer->fight_observed = false;

    // Рыцарь атакует рыцаря (рыцарь не убивает рыцаря)
    auto knight_visitor = std::make_shared<BattleVisitor>(knight2);
    EXPECT_FALSE(knight->accept(knight_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), KnightType);
    EXPECT_EQ(observer->last_defender->get_type(), KnightType);
}

// Тест 6: Логика боя белки с использованием Visitor
TEST_F(NPCTest, SquirrelFightLogicWithVisitor) {
    auto squirrel = std::make_shared<Squirrel>(100, 100, "Squirrel1");
    auto slaver = std::make_shared<Slaver>(100, 100, "Slaver1");
    auto knight = std::make_shared<Knight>(100, 100, "Knight1");
    auto squirrel2 = std::make_shared<Squirrel>(100, 100, "Squirrel2");
    
    squirrel->subscribe(observer);

    // Работорговец атакует белку (работорговец убивает белку)
    auto slaver_visitor = std::make_shared<BattleVisitor>(slaver);
    EXPECT_TRUE(squirrel->accept(slaver_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_TRUE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SlaverType);
    EXPECT_EQ(observer->last_defender->get_type(), SquirrelType);
    observer->fight_observed = false;

    // Рыцарь атакует белку (рыцарь не убивает белку)
    auto knight_visitor = std::make_shared<BattleVisitor>(knight);
    EXPECT_FALSE(squirrel->accept(knight_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_FALSE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), KnightType);
    EXPECT_EQ(observer->last_defender->get_type(), SquirrelType);
    observer->fight_observed = false;

    // Белка атакует белку (белка убивает белку)
    auto squirrel_attacker_visitor = std::make_shared<BattleVisitor>(squirrel2);
    EXPECT_TRUE(squirrel->accept(squirrel_attacker_visitor));
    EXPECT_TRUE(observer->fight_observed);
    EXPECT_TRUE(observer->last_win);
    EXPECT_EQ(observer->last_attacker->get_type(), SquirrelType);
    EXPECT_EQ(observer->last_defender->get_type(), SquirrelType);
}

// Тест 7: Сохранение и загрузка через фабрику
TEST_F(NPCTest, SaveAndLoadWithFactory) {
    // Создаем оригинальные объекты
    auto original_slaver = Factory::CreateNPC(SlaverType, 123, 456, "TestSlaver");
    auto original_knight = Factory::CreateNPC(KnightType, 321, 254, "TestKnight");
    auto original_squirrel = Factory::CreateNPC(SquirrelType, 111, 222, "TestSquirrel");

    // Сохраняем в поток
    std::stringstream ss;
    original_slaver->save(ss);
    original_knight->save(ss);
    original_squirrel->save(ss);

    ss.seekg(0);

    // Загружаем через фабрику
    auto loaded_slaver = Factory::CreateNPC(ss);
    auto loaded_knight = Factory::CreateNPC(ss);
    auto loaded_squirrel = Factory::CreateNPC(ss);
    
    EXPECT_NE(loaded_slaver, nullptr);
    EXPECT_NE(loaded_knight, nullptr);
    EXPECT_NE(loaded_squirrel, nullptr);
    
    EXPECT_EQ(loaded_slaver->get_type(), SlaverType);
    EXPECT_EQ(loaded_slaver->get_x(), 123);
    EXPECT_EQ(loaded_slaver->get_y(), 456);
    EXPECT_EQ(loaded_slaver->get_name(), "TestSlaver");
    
    EXPECT_EQ(loaded_knight->get_type(), KnightType);
    EXPECT_EQ(loaded_knight->get_x(), 321);
    EXPECT_EQ(loaded_knight->get_y(), 254);
    EXPECT_EQ(loaded_knight->get_name(), "TestKnight");
    
    EXPECT_EQ(loaded_squirrel->get_type(), SquirrelType);
    EXPECT_EQ(loaded_squirrel->get_x(), 111);
    EXPECT_EQ(loaded_squirrel->get_y(), 222);
    EXPECT_EQ(loaded_squirrel->get_name(), "TestSquirrel");
}

// Тест 8: Наблюдатели через фабрику
TEST_F(NPCTest, FactoryObservers) {
    auto npc = Factory::CreateNPC(SlaverType, 100, 100, "TestNPC");
    
    // Добавляем дополнительного наблюдателя
    npc->subscribe(observer);
    
    // Создаем атакующего
    auto knight = std::make_shared<Knight>(100, 100, "Attacker");
    
    // Вызываем бой через Visitor
    auto visitor = std::make_shared<BattleVisitor>(knight);
    npc->accept(visitor);
    
    // Проверяем, что наблюдатель получил уведомление
    EXPECT_TRUE(observer->fight_observed);
}

// Тест 9: Неправильный тип при загрузке
TEST_F(NPCTest, InvalidTypeLoad) {
    std::stringstream ss;
    ss << "999 100 100 InvalidNPC" << std::endl; // Неправильный тип
    
    auto npc = Factory::CreateNPC(ss);
    EXPECT_EQ(npc, nullptr);
}

// Тест 10: Проверка оператора вывода
TEST_F(NPCTest, OutputOperator) {
    auto slaver = std::make_shared<Slaver>(100, 200, "TestOutput");
    std::stringstream ss;
    ss << *slaver;
    
    std::string output = ss.str();
    EXPECT_TRUE(output.find("Slaver:") != std::string::npos);
    EXPECT_TRUE(output.find("\"TestOutput\"") != std::string::npos);
    EXPECT_TRUE(output.find("x:100") != std::string::npos);
    EXPECT_TRUE(output.find("y:200") != std::string::npos);
}

// Тест 11: Проверка работы Visitor с разными типами атакующих
TEST_F(NPCTest, VisitorDifferentAttackers) {
    auto defender = std::make_shared<Squirrel>(100, 100, "Defender");
    defender->subscribe(observer);
    
    // Тестируем разных атакующих
    auto slaver = std::make_shared<Slaver>(100, 100, "SlaverAttacker");
    auto squirrel = std::make_shared<Squirrel>(100, 100, "SquirrelAttacker");
    auto knight = std::make_shared<Knight>(100, 100, "KnightAttacker");
    
    // Работорговец должен убить белку
    auto slaver_visitor = std::make_shared<BattleVisitor>(slaver);
    EXPECT_TRUE(defender->accept(slaver_visitor));
    EXPECT_TRUE(observer->last_win);
    observer->fight_observed = false;
    
    // Белка должна убить белку
    auto squirrel_visitor = std::make_shared<BattleVisitor>(squirrel);
    EXPECT_TRUE(defender->accept(squirrel_visitor));
    EXPECT_TRUE(observer->last_win);
    observer->fight_observed = false;
    
    // Рыцарь не должен убить белку
    auto knight_visitor = std::make_shared<BattleVisitor>(knight);
    EXPECT_FALSE(defender->accept(knight_visitor));
    EXPECT_FALSE(observer->last_win);
}

// Тест 12: Проверка метода print()
TEST_F(NPCTest, PrintMethod) {
    auto knight = std::make_shared<Knight>(150, 250, "PrintTest");
    
    // Перенаправляем вывод
    std::stringstream output;
    auto old_buf = std::cout.rdbuf(output.rdbuf());
    
    knight->print();
    
    std::cout.rdbuf(old_buf);
    
    std::string result = output.str();
    EXPECT_TRUE(result.find("Knight:") != std::string::npos);
    EXPECT_TRUE(result.find("\"PrintTest\"") != std::string::npos);
    EXPECT_TRUE(result.find("x:150") != std::string::npos);
    EXPECT_TRUE(result.find("y:250") != std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
