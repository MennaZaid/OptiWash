//
// Created by ASUS on 5/16/2025.
//

#ifndef ITEM_H
#define ITEM_H
#include <string>
#include<vector>

using namespace std;

class Item {
private:
    string name;
    string color;
    string fabric;
    bool delicate;
    string washTemp;
    bool drySafe;
    bool priority;

public:

    Item(string name, string color, string fabric, bool delicate,
         string washTemp, bool drySafe, bool priority);


    string getName() const;
    string getColor() const;
    string getFabric() const;
    bool isDelicate() const;
    string getWashTemp() const;
    bool isDrySafe() const;
    bool isPriority() const;


    void setName(const string& newName);
    void setColor(const string& newColor);
    void setFabric(const string& newFabric);
    void setDelicate(bool value);
    void setWashTemp(const string& temp);
    void setDrySafe(bool value);
    void setPriority(bool value);


    static string getColorCategory(const string& color);

    static bool isCompatibleWithLoad(const Item& item, const std::vector<Item>& load, int maxLoadSize, bool separateWhites, bool relaxedMode = false);

};




#endif //ITEM_H
