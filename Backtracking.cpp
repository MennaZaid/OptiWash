#include "Backtracking.h"
#include "Item.h"
#include <limits>

using namespace std;

Backtracking::Backtracking(const vector<Item>& items, int maxLoadSize, bool separateWhites)
    : items(items), maxLoadSize(maxLoadSize), separateWhites(separateWhites) {}

vector<vector<Item>> Backtracking::pack() {
    bestSolution.clear();

    vector<vector<Item>> currentLoads;

    backtrack(0, currentLoads);

    return bestSolution;
}

void Backtracking::backtrack(int index, vector<vector<Item>>& currentLoads) {
    if (index == items.size()) {
        // All items assigned, check if better than bestSolution
        if (bestSolution.empty() || currentLoads.size() < bestSolution.size()) {
            bestSolution = currentLoads;
        }
        return;
    }

    const Item& item = items[index];

    // Try adding to existing loads
    for (int i = 0; i < (int)currentLoads.size(); ++i) {
        if (Item::isCompatibleWithLoad(item, currentLoads[i], maxLoadSize, separateWhites)) {
            currentLoads[i].push_back(item);
            backtrack(index + 1, currentLoads);
            currentLoads[i].pop_back();
        }
    }

    // Try creating a new load
    currentLoads.push_back({item});
    backtrack(index + 1, currentLoads);
    currentLoads.pop_back();
}
