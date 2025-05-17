//
// Created by ASUS on 5/16/2025.
//

// Item.cpp
#include "Item.h"


Item::Item(string name, string color, string fabric, bool delicate,
           string washTemp, bool drySafe, bool priority)
    : name(name), color(color), fabric(fabric), delicate(delicate),
      washTemp(washTemp), drySafe(drySafe), priority(priority) {}


string Item::getName() const { return name; }
string Item::getColor() const { return color; }
string Item::getFabric() const { return fabric; }
bool Item::isDelicate() const { return delicate; }
string Item::getWashTemp() const { return washTemp; }
bool Item::isDrySafe() const { return drySafe; }
bool Item::isPriority() const { return priority; }


void Item::setName(const string& newName) { name = newName; }
void Item::setColor(const string& newColor) { color = newColor; }
void Item::setFabric(const string& newFabric) { fabric = newFabric; }
void Item::setDelicate(bool value) { delicate = value; }
void Item::setWashTemp(const string& temp) { washTemp = temp; }
void Item::setDrySafe(bool value) { drySafe = value; }
void Item::setPriority(bool value) { priority = value; }


#include <set>
#include "Item.h"

string Item::getColorCategory(const string& color) {
    static set<string> whiteColors = {"white"};
    static set<string> lightColors = {"beige", "cream", "light gray", "pastel pink", "sky blue" , "blue" , "pink" , "mauve" , "green" , "light"};
    static set<string> darkColors = {"black", "navy", "dark gray", "maroon", "forest green", "deep purple" , "dark" , "red"};

    if (whiteColors.count(color)) return "white";
    if (lightColors.count(color)) return "light";
    if (darkColors.count(color)) return "dark";
    return "unknown";
}

bool Item::isCompatibleWithLoad(const Item& item,
                                 const vector<Item>& load,
                                 int maxLoadSize,
                                 bool separateWhites) {
    if (load.size() >= maxLoadSize) return false;

    string itemColorCat = getColorCategory(item.getColor());

    for (const Item& existing : load) {
        string existingColorCat = getColorCategory(existing.getColor());

        // Constraint: Color separation
        if (separateWhites) {
            if (itemColorCat == "white" && existingColorCat != "white") return false;
            if (itemColorCat != "white" && existingColorCat == "white") return false;
        } else {
            if (itemColorCat != existingColorCat) return false;
        }

        // Constraint: Wash temperature match
        if (item.getWashTemp() != existing.getWashTemp()) return false;

        // Constraint: Delicate items separated
        if (item.isDelicate() != existing.isDelicate()) return false;

        // Optional: Dry-safe conflict
        if (item.isDrySafe() != existing.isDrySafe()) return false;
    }

    return true;
}

