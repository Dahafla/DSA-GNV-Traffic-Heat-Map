#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

struct CrashData {
    string crashDate;
    int totalPeople;
    double longitude;
    double latitude;
};

void parseCSVAndInsertIntoMap(const string& filename, unordered_map<int, CrashData>& crashDataMap) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Failed to open file " << filename << endl;
        return;
    }

    string line;
    getline(inputFile, line); // Skip header line

    while (getline(inputFile, line)) {
        stringstream inputString(line);
        CrashData data;
        string tempString;
        int caseNumber;

        try {
            getline(inputString, tempString, ',');
            caseNumber = stoi(tempString) % 211000000; // Apply modulo operation
            getline(inputString, data.crashDate, ',');
            getline(inputString, tempString, ',');
            data.totalPeople = stoi(tempString);
            getline(inputString, tempString, ',');
            data.longitude = stod(tempString);
            getline(inputString, tempString, ',');
            data.latitude = stod(tempString);
        } catch (const exception& e) {
            cerr << "Warning: Skipping line due to exception: " << line << endl;
            continue;
        }

        crashDataMap[caseNumber] = data;
    }

    inputFile.close();
}

int main(int argc, char** argv) {
    unordered_map<int, CrashData> crashDataMap;

    parseCSVAndInsertIntoMap("heatmap_data.csv", crashDataMap);

    // Print the number of elements in the hashmap
    cout << "Number of elements in the hashmap: " << crashDataMap.size() << endl;

    return 0;
}
