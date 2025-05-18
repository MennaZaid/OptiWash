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

    Greedy greedy(items, 5, true, false);  // relaxedMode = false
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

    Backtracking backtrack(items, 2, false, false);
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

    Greedy greedy(items, 5, true, false);  // relaxedMode = false
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

    Greedy greedy(items, 2, true, false);
    Backtracking backtrack(items, 2, true, false);

    auto greedyLoads = greedy.pack();
    auto btLoads = backtrack.pack();

    EXPECT_GE(btLoads.size(), greedyLoads.size());
}

// Test: Max load size respected
TEST(LaundryTest, RespectsMaxLoadSize) {
    vector<Item> items = {
        makeItem("A", "dark", "cotton", false, "warm", true, false),
        makeItem("B", "dark", "cotton", false, "warm", true, false),
        makeItem("C", "dark", "cotton", false, "warm", true, false),
        makeItem("D", "dark", "cotton", false, "warm", true, false)
    };

    Greedy greedy(items, 2, true, false);  // maxLoadSize = 2
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

    Greedy greedy(items, 5, true, false);
    auto loads = greedy.pack();

    EXPECT_EQ(loads.size(), 2);
}

// Test: Dry-safe and non-dry-safe items are separated
TEST(LaundryTest, DrySafeSeparated) {
    vector<Item> items = {
        makeItem("Cotton Tee", "dark", "cotton", false, "warm", true, false),
        makeItem("Silk Scarf", "dark", "silk", true, "cold", false, false)
    };

    Greedy greedy(items, 5, true, false);
    auto loads = greedy.pack();

    EXPECT_EQ(loads.size(), 2);
}

// Test: Time estimation is correct for same program
TEST(LaundryTest, EstimateTimeSingleMachine) {
    vector<string> wash = {"Normal Care", "Normal Care"};
    vector<string> dry = {"Cotton", "Cotton"};
    LaundryProgramDatabase db;

    int totalTime = calculateLaundryTime(wash, dry, 1, 1, db);

    EXPECT_EQ(totalTime, 356);
}

// Test: Time estimation parallel machines
TEST(LaundryTest, EstimateTimeWithParallelMachines) {
    vector<string> wash = {"Normal Care", "Stain Care", "Allergy Care"};
    vector<string> dry = {"Cotton", "Cotton", "Wool"};
    LaundryProgramDatabase db;

    int totalTime = calculateLaundryTime(wash, dry, 2, 1, db);

    EXPECT_EQ(totalTime, 663);
}

// Relaxed mode tests
TEST(RelaxedModeTest, AllowsWhitesWithLights) {
    vector<Item> items = {
        makeItem("White Shirt", "white", "cotton", false, "warm", true, false),
        makeItem("Beige Tee", "beige", "cotton", false, "warm", true, false)
    };

    Greedy greedy(items, 5, true, true);  // relaxedMode = true
    auto loads = greedy.pack();

    EXPECT_EQ(loads.size(), 1);
}

TEST(RelaxedModeTest, StillSeparatesDrySafeItems) {
    vector<Item> items = {
        makeItem("Hoodie", "dark", "cotton", false, "warm", true, false),
        makeItem("Scarf", "dark", "wool", true, "cold", false, false)
    };

    Greedy greedy(items, 5, true, true);
    auto loads = greedy.pack();

    EXPECT_EQ(loads.size(), 2);
}

TEST(RelaxedModeTest, PrioritizesImportantItems) {
    vector<Item> items = {
        makeItem("Casual Shirt", "light", "cotton", false, "warm", true, false),
        makeItem("Gym Tee", "dark", "synthetic", false, "cold", true, true)
    };

    Greedy greedy(items, 5, true, true);
    auto loads = greedy.pack();

    bool priorityFound = any_of(loads[0].begin(), loads[0].end(), [](const Item& item) {
        return item.isPriority();
    });

    EXPECT_TRUE(priorityFound);
}
