//
// Created by ASUS on 5/16/2025.
//

#ifndef GREEDY_H
#define GREEDY_H
#include"Item.h"
#include<vector>
#include<algorithm>
using namespace std;
class Greedy {
private:
    vector<Item> items;
    int maxLoadSize;
    bool separateWhites;
    bool relaxedMode;

public:
    Greedy(const vector<Item>& items, int maxLoadSize, bool separateWhites, bool relaxedMode = false);

    vector<vector<Item>> pack();
};




#endif //GREEDY_H
