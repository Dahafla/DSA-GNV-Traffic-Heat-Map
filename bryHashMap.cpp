#include <iostream>
#include <string>
#include "bryHashMap.h"

using namespace std;

// constructor for the hashmap
bHash::bHash() {

    for(int i = 0; i < tableSize; i++)
    {
        hashTable[i] = new crashData;
        hashTable[i]->caseNumber = 0;
        hashTable[i]->latitude = 0;
        hashTable[i]->longitude = 0;
        hashTable[i]->totalVehicle = 0;
        hashTable[i]->crashDay = "empty";
        hashTable[i]->next = nullptr;
    }
}

// Prints the items within the buckets
void bHash::PrintItemsInBucket(int bucket) {
    crashData* ptr = hashTable[bucket];

    if(ptr->caseNumber == 0)
    {
        cout << "Bucket = " << bucket << " is empty";
    }
    else
    {
        cout << "Bucket = " << bucket << " contains the following items\n";

        while(ptr != nullptr)
        {
            cout << "----------------------\n";
            cout << ptr->caseNumber << endl;
            cout << ptr->latitude << endl;
            cout << ptr->longitude << endl;
            cout << ptr->totalVehicle << endl;
            cout << ptr->crashDay << endl;
            cout << "----------------------\n";
            ptr = ptr->next;
        }
    }
}

// Shows number of items in a bucket
int bHash::NumberOfItemsInBucket(int bucket) {
    int count = 0;

    if(hashTable[bucket]->caseNumber == 0)
    {
        return count;
    }
    else
    {
        count++;
        crashData* ptr = hashTable[bucket];
        while(ptr->next != nullptr)
        {
            count++;
            ptr = ptr->next;
        }
    }
    return count;
}

// Show total number of elements in hashMap
int bHash::GetTotalItems() {
    int totalItems = 0;
    for (int i = 0; i < tableSize; i++) {
        crashData* ptr = hashTable[i];
        while (ptr != nullptr && ptr->caseNumber != 0) {
            totalItems++;
            ptr = ptr->next;
        }
    }
    return totalItems;
}

// Function to search in the hashmap for latitude and longitude by case number
void bHash::FindLatLon(int caseNumber) {
    int bucket = Hash(caseNumber);
    bool foundCase = false;
    double latitude, longitude;

    crashData* ptr = hashTable[bucket];
    while (ptr != nullptr) {
        if (ptr->caseNumber == caseNumber) {
            foundCase = true;
            latitude = ptr->latitude;
            longitude = ptr->longitude;
            break;  // Break the loop once the case is found
        }
        ptr = ptr->next;
    }

    if (foundCase) {
        cout << "Latitude: " << latitude << ", Longitude: " << longitude << endl;
    } else {
        cout << "Case number " << caseNumber << "'s info was not found in the HashTable\n";
    }
}

// Adds item into the hashmap
void bHash::AddItem(int caseNumber, double latitude, double longitude, int totalVehicle, string crashDay) {

    int bucket = Hash(caseNumber);

    if(hashTable[bucket]->caseNumber == 0)
    {
        hashTable[bucket]->caseNumber = caseNumber;
        hashTable[bucket]->latitude = latitude;
        hashTable[bucket]->longitude = longitude;
        hashTable[bucket]->totalVehicle = totalVehicle;
        hashTable[bucket]->crashDay = crashDay;
    }
    else
    {
        crashData* ptr = hashTable[bucket];
        crashData* n = new crashData;
        n->caseNumber = caseNumber;
        n->latitude = latitude;
        n->longitude = longitude;
        n->totalVehicle = totalVehicle;
        n->crashDay = crashDay;
        n->next = nullptr;
        while(ptr->next != nullptr)
        {
            ptr = ptr->next;

        }
        ptr->next = n;
    }
}

// Function to print table and values
void bHash::PrintTable() {

    // number of elements in bucket, and prints table
    int number;
    for (int i = 0; i < tableSize; i++)
    {
        number = NumberOfItemsInBucket(i);
        cout << "------------------------" << endl;
        cout << "Bucket =  " << i << endl;
        cout << hashTable[i]->caseNumber << endl;
        cout << hashTable[i]->latitude << endl;
        cout << hashTable[i]->longitude << endl;
        cout << hashTable[i]->totalVehicle << endl;
        cout << hashTable[i]->crashDay << endl;
        cout << "# of items = " << number << endl;
        cout << "------------------------" << endl;
    }
}

// Function for hashing
int bHash::Hash(int key) {
    // Hash function using modulo operation
    return key % 210000000 % tableSize;
}


