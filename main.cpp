#include <iostream>
#include <vector>
#include <string>
#include "Item.h"
#include "Greedy.h"
#include "Backtracking.h"
#include "LaundryProgramDatabase.h"
#include<set>
using namespace std;


void printLoads(const vector<vector<Item>>& loads,
                const vector<string>& washPrograms,
                const vector<string>& dryPrograms,
                const string& title) {
    cout << "==== " << title << " ====\n";
    int loadNum = 1;
    for (size_t i = 0; i < loads.size(); ++i) {
        cout << "Load " << loadNum++ << " (" << loads[i].size() << " items):\n";
        for (const auto& item : loads[i]) {
            cout << "  - " << item.getName() << " (" << item.getColor() << ", "
                 << item.getFabric() << ", "
                 << (item.isDelicate() ? "Delicate" : "Normal") << ", "
                 << item.getWashTemp() << " wash"
                 << ", Dry-safe: " << (item.isDrySafe() ? "Yes" : "No")
                 << ", Priority: " << (item.isPriority() ? "Yes" : "No") << ")\n";
        }
        cout << "  Washing Program: " << washPrograms[i] << "\n";
        cout << "  Drying Program: " << dryPrograms[i] << "\n\n";
    }
}

int main() {
    vector<Item> items;
    int n;

    cout << "Enter number of laundry items: ";
    cin >> n;
    cin.ignore(); // consume leftover newline
    string relaxedModeStr;
    cout << "Enable relaxed mode (safe compromises)? (yes/no): ";
    cin >> relaxedModeStr;
    bool relaxedMode = (relaxedModeStr == "yes");

    for (int i = 0; i < n; ++i) {
        cout << "\nEnter details for item " << (i + 1) << ":\n";

        string name, color, fabric, washTemp, priorityStr, delicateStr, drySafeStr;
        bool delicate, drySafe, priority;

        cout << "Name: ";
        getline(cin, name);

        set<string> whiteColors = {"white"};
        set<string> lightColors = {"beige", "cream", "light gray", "pastel pink", "sky blue", "blue", "pink", "mauve", "green", "light"};
        set<string> darkColors  = {"black", "navy", "dark gray", "maroon", "forest green", "deep purple", "dark", "red"};

        cout << "Color (choose from:\n";
        cout << "  White: ";
        for (const auto& c : whiteColors) cout << c << ", ";
        cout << "\n  Light: ";
        for (const auto& c : lightColors) cout << c << ", ";
        cout << "\n  Dark: ";
        for (const auto& c : darkColors) cout << c << ", ";
        cout << "\b\b)\n> ";
        getline(cin, color);

        cout << "Fabric (e.g., cotton, wool): ";
        getline(cin, fabric);

        cout << "Delicate? (yes/no): ";
        getline(cin, delicateStr);
        delicate = (delicateStr == "yes");

        cout << "Wash Temperature (cold, warm, hot): ";
        getline(cin, washTemp);

        cout << "Dry-safe? (yes/no): ";
        getline(cin, drySafeStr);
        drySafe = (drySafeStr == "yes");

        cout << "Priority item? (yes/no): ";
        getline(cin, priorityStr);
        priority = (priorityStr == "yes");

        items.emplace_back(name, color, fabric, delicate, washTemp, drySafe, priority);
    }

    int maxLoadSize;
    cout << "\nEnter maximum load size (number of items per load): ";
    cin >> maxLoadSize;

    bool separateWhites = true;  // default if strict

    if (!relaxedMode) {
        string separateWhitesStr;
        cout << "Separate whites into their own load? (yes/no): ";
        cin >> separateWhitesStr;
        separateWhites = (separateWhitesStr == "yes");
    } else {
        cout << "(Note: Whites may be mixed with lights in relaxed mode — skipping separation question.)\n";
        separateWhites = false;
    }

    int washers, dryers;
    cout << "Enter number of washing machines available: ";
    cin >> washers;

    cout << "Enter number of dryers available: ";
    cin >> dryers;


    Greedy greedyPacker(items, maxLoadSize, separateWhites, relaxedMode);
    vector<vector<Item>> greedyLoads = greedyPacker.pack();


    vector<string> greedyWashPrograms, greedyDryPrograms;
    for (const auto& load : greedyLoads) {
       greedyWashPrograms.push_back(LaundryProgramDatabase::selectWashingProgram(load));
 greedyDryPrograms.push_back(LaundryProgramDatabase::selectDryingProgram(load));
    }


    int greedyTotalTime = calculateLaundryTime(greedyWashPrograms, greedyDryPrograms, washers, dryers, LaundryProgramDatabase());

    printLoads(greedyLoads, greedyWashPrograms, greedyDryPrograms, "Greedy Packing Result");
    cout << "Estimated total laundry time (Greedy): " << greedyTotalTime << " minutes.\n\n";


    Backtracking backtrackingPacker(items, maxLoadSize, separateWhites, relaxedMode);
    vector<vector<Item>> backtrackingLoads = backtrackingPacker.pack();

    vector<string> backtrackingWashPrograms, backtrackingDryPrograms;
    for (const auto& load : backtrackingLoads) {
        backtrackingWashPrograms.push_back(LaundryProgramDatabase::selectWashingProgram(load));
        backtrackingDryPrograms.push_back(LaundryProgramDatabase::selectDryingProgram(load));
    }

    int backtrackingTotalTime = calculateLaundryTime(backtrackingWashPrograms, backtrackingDryPrograms, washers, dryers, LaundryProgramDatabase());

    printLoads(backtrackingLoads, backtrackingWashPrograms, backtrackingDryPrograms, "Backtracking Packing Result");
    cout << "Estimated total laundry time (Backtracking): " << backtrackingTotalTime << " minutes.\n";

    return 0;
}
