//
// Created by ASUS on 5/16/2025.
//


#ifndef LAUNDRY_PROGRAM_DATABASE_H
#define LAUNDRY_PROGRAM_DATABASE_H

#include <unordered_map>
#include <string>
#include <vector>
#include "Item.h"

using namespace std;

class LaundryProgramDatabase {
public:
    static unordered_map<string, int> washingPrograms;
    static unordered_map<string, int> dryingPrograms;

    static int getWashingDuration(const string& program);
    static int getDryingDuration(const string& program);

     int estimateTotalLaundryTime(const vector<vector<Item>>& loads,
                                 int washers, int dryers,
                                 const LaundryProgramDatabase& db);

    static string selectWashingProgram(const vector<Item>& load);
    static string selectDryingProgram(const vector<Item>& load);


};

int calculateLaundryTime(const vector<string>& washingPrograms,
                            const vector<string>& dryingPrograms,
                            int numWashers, int numDryers,
                            const LaundryProgramDatabase& db);



#endif //LAUNDRYPROGRAMDATABASE_H
