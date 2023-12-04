#pragma once
#include <iostream>
#include <string>

using namespace std;

class bHash {
public:
    static const int tableSize = 40;

    struct crashData {
        int caseNumber;
        double longitude;
        double latitude;
        int totalVehicle;
        string crashDay;
        crashData* next;
    };

    crashData* hashTable[tableSize];
    bHash();
    int Hash(int key);
    void AddItem(int caseNumber, double latitude, double longitude, int totalPeople, string crashDay);
    void RemoveItem(int caseNumber);
    int NumberOfItemsInBucket(int bucket);
    void PrintTable();
    void PrintItemsInBucket(int index);
    void FindLatLon(int caseNumber);
    int GetTotalItems();
};


