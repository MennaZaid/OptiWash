//
// Created by ASUS on 5/16/2025.
//

#include "Greedy.h"

Greedy::Greedy(const vector<Item>& items, int maxLoadSize, bool separateWhites, bool relaxedMode)
    : items(items), maxLoadSize(maxLoadSize), separateWhites(separateWhites), relaxedMode(relaxedMode) {}

vector<vector<Item>> Greedy::pack() {
    // Step 1: Sort priority items first
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.isPriority() && !b.isPriority();
    });

    // Step 2: Final output: list of laundry loads
    vector<vector<Item>> loads;

    // Step 3: Go through items one by one
    for (const Item& item : items) {
        bool placed = false;

        // Try to place item in an existing load
        for (auto& load : loads) {
            if (Item::isCompatibleWithLoad(item, load, maxLoadSize, separateWhites, relaxedMode)) {
                load.push_back(item);
                placed = true;
                break;
            }

        }

        // If not placed, create a new load for it
        if (!placed) {
            loads.push_back({item});
        }
    }

    return loads;
}
