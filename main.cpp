#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "bryHashMap.h"

using namespace std;

//struct to hold different attributes of the data
struct crashData{
    int caseNumber;
    double latitude;
    double longitude;
    int totalPeople;
    string crashDay;
};

vector<string> split(const string& line, char delim){
    vector<string> tokens;
    istringstream tokenStream(line);
    string token;
    while(getline(tokenStream, token, delim)){
        tokens.push_back(token);
    }
    return tokens;
}

//parse the CSV and hold the data in a vector
void parseCSV(const string& filename, vector<crashData>& crashdata, bHash& crashDataMap) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "file can't open: error" << endl;
        return;
    }
    string line;
    getline(file, line); // Skip header line

    while (getline(file, line)) {
        vector<string> tokens = split(line, ',');
        if (tokens.size() >= 24) {  // Ensure enough tokens are present
            crashData data;
            try {
                data.caseNumber = stoi(tokens[0]);
                data.latitude = stod(tokens[23]);
                data.longitude = stod(tokens[22]);
                data.totalPeople = stoi(tokens[12]);
                data.crashDay = tokens[5];

                crashdata.push_back(data);  // Add data to the vector
                crashDataMap.AddItem(data.caseNumber, data.latitude, data.longitude, data.totalPeople, data.crashDay);  // Add data to the hashmap
            } catch (const exception& e) {
                continue;  // Skip to the next line if there's an error
            }
        }
    }
}

int main() {
    string filename = "Traffic_Crashes.csv";
    vector<crashData> crashdata;
    bHash crashDataMap;

    parseCSV(filename, crashdata, crashDataMap);

    // Print the data from the vector (retaining original functionality)
    for (const auto& data : crashdata) {
        cout << "Case Number: " << data.caseNumber
             << ", Latitude: " << data.latitude
             << ", Longitude: " << data.longitude
             << ", Total People: " << data.totalPeople
             << ", Crash Day: " << data.crashDay << endl;
    }

    // Print the size of the hashmap
    cout << "Total items in the hashmap: " << crashDataMap.GetTotalItems() << endl;
    crashDataMap.FindLatLon(216018195);
    crashDataMap.FindLatLon(220006661);
    crashDataMap.FindLatLon(220012025);

    // Optionally, you can interact with the crashDataMap hashmap as needed
    // Example: crashDataMap.PrintTable();

    return 0;
}
