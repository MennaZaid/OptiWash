//
// Created by ASUS on 5/16/2025.
//

#include "Greedy.h"

Greedy::Greedy(const vector<Item>& items, int maxLoadSize, bool separateWhites, bool relaxedMode)
    : items(items), maxLoadSize(maxLoadSize), separateWhites(separateWhites), relaxedMode(relaxedMode) {}

vector<vector<Item>> Greedy::pack() {

    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        return a.isPriority() && !b.isPriority();
    });


    vector<vector<Item>> loads;


    for (const Item& item : items) {
        bool placed = false;


        for (auto& load : loads) {
            if (Item::isCompatibleWithLoad(item, load, maxLoadSize, separateWhites, relaxedMode)) {
                load.push_back(item);
                placed = true;
                break;
            }

        }


        if (!placed) {
            loads.push_back({item});
        }
    }

    return loads;
}
