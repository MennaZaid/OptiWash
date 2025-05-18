//
// Created by ASUS on 5/16/2025.
//
#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include "Item.h"
#include <vector>

using namespace std;

class Backtracking {
private:
    vector<Item> items;
    int maxLoadSize;
    bool separateWhites;
    bool relaxedMode;

    vector<vector<Item>> bestSolution;

    void backtrack(int index, vector<vector<Item>>& currentLoads);

public:
    Backtracking(const vector<Item>& items, int maxLoadSize, bool separateWhites, bool relaxedMode = false);

    vector<vector<Item>> pack();
};

#endif // BACKTRACKING_H
