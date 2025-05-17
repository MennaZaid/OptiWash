//
// Created by ASUS on 5/16/2025.
//

#include <gtest/gtest.h>
#include "Item.h"
#include "Greedy.h"
#include "Backtracking.h"
#include "LaundryProgramDatabase.h"

using namespace std;

// Helper to create mock items
Item makeItem(const string& name, const string& color, const string& fabric,
              bool delicate, const string& temp, bool drySafe, bool priority) {
    return Item(name, color, fabric, delicate, temp, drySafe, priority);
}

// Test: Packing more than 20 items
TEST(LaundryTest, HandlesMoreThan20Items) {
    vector<Item> items;
    for (int i = 0; i < 25; ++i) {
        items.push_back(makeItem("T-Shirt", "dark", "cotton", false, "warm", true, i % 2 == 0));
    }

    Greedy greedy(items, 5, true);
    auto loads = greedy.pack();

    EXPECT_GT(loads.size(), 0);
    int totalItems = 0;
    for (const auto& load : loads) totalItems += load.size();
    EXPECT_EQ(totalItems, 25);
}

// Test: Items with same name
TEST(LaundryTest, HandlesDuplicateItemNames) {
    vector<Item> items = {
        makeItem("Jeans", "dark", "denim", false, "warm", true, false),
        makeItem("Jeans", "dark", "denim", false, "warm", true, false)
    };

    Backtracking backtrack(items, 2, false);
    auto loads = backtrack.pack();

    EXPECT_EQ(loads.size(), 1);  // Compatible, so grouped
    EXPECT_EQ(loads[0].size(), 2);
}

// Test: Color separation works
TEST(LaundryTest, SeparatesWhiteItems) {
    vector<Item> items = {
        makeItem("White Shirt", "white", "cotton", false, "hot", true, false),
        makeItem("Tee", "dark", "cotton", false, "warm", true, false)
    };

    Greedy greedy(items, 5, true);
    auto loads = greedy.pack();

    EXPECT_EQ(loads.size(), 2);
}

// Test: Program assignment logic
TEST(LaundryTest, AssignsCorrectPrograms) {
    vector<Item> load = {
        makeItem("Silk Blouse", "light", "silk", true, "cold", false, false),
        makeItem("Tee", "dark", "cotton", false, "warm", true, false)
    };

    auto wash = LaundryProgramDatabase::selectWashingProgram(load);
    auto dry = LaundryProgramDatabase::selectDryingProgram(load);

    EXPECT_EQ(wash, "Easy Care");
    EXPECT_EQ(dry, "Air Dry");
}
// Test: Backtracking produces fewer loads than Greedy in mixed scenario
TEST(LaundryTest, BacktrackingIsOptimal) {
    vector<Item> items = {
        makeItem("Shirt1", "white", "cotton", false, "hot", true, false),
        makeItem("Shirt2", "white", "cotton", false, "hot", true, false),
        makeItem("Towel", "white", "cotton", false, "hot", true, false),
        makeItem("Jeans", "dark", "denim", false, "warm", true, false),
        makeItem("Shorts", "dark", "denim", false, "warm", true, false)
    };

    Greedy greedy(items, 2, true);
    Backtracking backtrack(items, 2, true);

    auto greedyLoads = greedy.pack();
    auto btLoads = backtrack.pack();

    EXPECT_GE(greedyLoads.size(), btLoads.size());  // backtracking should be same or better
}

// Test: Max load size respected
TEST(LaundryTest, RespectsMaxLoadSize) {
    vector<Item> items = {
        makeItem("A", "dark", "cotton", false, "warm", true, false),
        makeItem("B", "dark", "cotton", false, "warm", true, false),
        makeItem("C", "dark", "cotton", false, "warm", true, false),
        makeItem("D", "dark", "cotton", false, "warm", true, false)
    };

    Greedy greedy(items, 2, false);
    auto loads = greedy.pack();

    for (const auto& load : loads) {
        EXPECT_LE(load.size(), 2);
    }
}

// Test: Incompatible fabrics are separated
TEST(LaundryTest, IncompatibleFabricsSeparated) {
    vector<Item> items = {
        makeItem("Delicate Blouse", "light", "silk", true, "cold", false, false),
        makeItem("Jeans", "light", "denim", false, "warm", true, false)
    };

    Greedy greedy(items, 2, false);
    auto loads = greedy.pack();

    EXPECT_EQ(loads.size(), 2);  // incompatible due to delicate flag
}

// Test: Dry-safe and non-dry-safe items are separated
TEST(LaundryTest, DrySafeSeparated) {
    vector<Item> items = {
        makeItem("Cotton Tee", "dark", "cotton", false, "warm", true, false),
        makeItem("Silk Scarf", "dark", "silk", true, "cold", false, false)
    };

    Greedy greedy(items, 2, false);
    auto loads = greedy.pack();

    EXPECT_EQ(loads.size(), 2);  // one is not dry-safe
}

// Test: Time estimation is correct for same program
TEST(LaundryTest, EstimateTimeSingleMachine) {
    vector<string> wash = {"Normal Care", "Normal Care"};
    vector<string> dry = {"Cotton", "Cotton"};
    LaundryProgramDatabase db;

    int totalTime = calculateLaundryTime(wash, dry, 1, 1, db);

    // Each program = 58 (wash), 120 (dry), 2 sequentially = 2 * 58 + 2 * 120
    EXPECT_EQ(totalTime, 356);
}

// Test: Time estimation parallel machines
TEST(LaundryTest, EstimateTimeWithParallelMachines) {
    vector<string> wash = {"Normal Care", "Stain Care", "Allergy Care"};
    vector<string> dry = {"Cotton", "Cotton", "Wool"};
    LaundryProgramDatabase db;

    int totalTime = calculateLaundryTime(wash, dry, 2, 1, db);

    // Washers: batches = 2 → [Normal, Stain] = max(58,137)=137 + [Allergy]=257 → 394
    // Dryers: batches = 3 → 120 + 120 + 29 = 269
    // Total = 394 + 269
    EXPECT_EQ(totalTime, 663);
}
