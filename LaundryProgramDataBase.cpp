//
// Created by ASUS on 5/16/2025.
//

#include "LaundryProgramDatabase.h"

unordered_map<string, int> LaundryProgramDatabase::washingPrograms = {
    {"Normal Care", 58},
    {"Stain Care", 137},      // 2:17 hrs = 137 minutes
    {"Allergy Care", 257},    // 4:17 hrs = 257 minutes
    {"Quick Wash", 23}
};

unordered_map<string, int> LaundryProgramDatabase::dryingPrograms = {
    {"Cotton", 120},
    {"Speed 30", 30},
    {"Easy Care", 80},
    {"Wool", 29},
    {"Sports Wear", 50},
    {"Allergy Care", 180},
    {"Duvet", 175},
    {"Bedding Refresh", 50},
    {"Down Jacket Refresh", 25},
    {"Warm Air", 60},
    {"Cool Air", 60},
    {"Rack Dry", 180}
};


int calculateLaundryTime(const vector<string>& washingPrograms,
                         const vector<string>& dryingPrograms,
                         int numWashers, int numDryers,
                         const LaundryProgramDatabase& db) {
    int totalLoads = (int)washingPrograms.size();

    int washBatches = (totalLoads + numWashers - 1) / numWashers;
    int dryBatches = (totalLoads + numDryers - 1) / numDryers;

    int totalWashTime = 0;
    for (int batch = 0; batch < washBatches; ++batch) {
        int startIdx = batch * numWashers;
        int endIdx = min(startIdx + numWashers, totalLoads);
        int maxBatchTime = 0;
        for (int i = startIdx; i < endIdx; ++i) {
            int t = db.getWashingDuration(washingPrograms[i]);
            if (t > maxBatchTime) maxBatchTime = t;
        }
        totalWashTime += maxBatchTime;
    }

    int totalDryTime = 0;
    for (int batch = 0; batch < dryBatches; ++batch) {
        int startIdx = batch * numDryers;
        int endIdx = min(startIdx + numDryers, totalLoads);
        int maxBatchTime = 0;
        for (int i = startIdx; i < endIdx; ++i) {
            int t = db.getDryingDuration(dryingPrograms[i]);
            if (t > maxBatchTime) maxBatchTime = t;
        }
        totalDryTime += maxBatchTime;
    }

    return totalWashTime + totalDryTime;
}
int LaundryProgramDatabase::getWashingDuration(const string& program) {
    if (washingPrograms.count(program))
        return washingPrograms[program];
    return -1; // unknown program
}

int LaundryProgramDatabase::getDryingDuration(const string& program) {
    if (dryingPrograms.count(program))
        return dryingPrograms[program];
    return -1; // unknown program
}

int estimateTotalLaundryTime(const vector<vector<Item>>& loads,
                             int washers, int dryers,
                             const LaundryProgramDatabase& db) {
    int totalLoads = (int)loads.size();

    // For demo, assume all use "Normal Care" washing and "Cotton" drying
    int washTime = db.getWashingDuration("Normal Care");
    int dryTime = db.getDryingDuration("Cotton");

    int washBatches = (totalLoads + washers - 1) / washers;
    int dryBatches = (totalLoads + dryers - 1) / dryers;

    return washBatches * washTime + dryBatches * dryTime;
}
// Determine washing program for a load
string LaundryProgramDatabase::selectWashingProgram(const vector<Item>& load) {
    bool hasDelicate = false;
    bool hasAllergyCare = false;
    bool allHot = true;

    for (const auto& item : load) {
        if (item.isDelicate()) hasDelicate = true;
        // Assume you add a flag or tag for allergy care, here simulated:
        // if (item.hasTag("allergy")) hasAllergyCare = true;
        if (item.getWashTemp() != "hot") allHot = false;
    }

    if (hasAllergyCare) return "Allergy Care";
    if (hasDelicate) return "Easy Care";
    if (allHot) return "Stain Care";
    return "Normal Care";
}

// Determine drying program for a load
string LaundryProgramDatabase::selectDryingProgram(const vector<Item>& load) {
    bool anyNotDrySafe = false;

    for (const auto& item : load) {
        if (!item.isDrySafe()) {
            anyNotDrySafe = true;
            break;
        }
    }

    if (anyNotDrySafe) return "Air Dry";

    // Simplify: pick drying program based on dominant fabric, for example
    // Here just return default "Cotton"
    return "Cotton";
}


