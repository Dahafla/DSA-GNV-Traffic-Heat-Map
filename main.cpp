#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "bryHashMap.h"
#include "RBTree.cpp"
#include <matplot/matplot.h>
using namespace std;
using namespace matplot;

//struct to hold different attributes of the data
struct crashData{
    int caseNumber;
    double latitude;
    double longitude;
    int totalVehicle;
    string crashDay;
};

//split the data
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
void parseCSV(const string& filename, vector<crashData>& crashdata, bHash& crashDataMap, RBTree& RbtreeData) {
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
                data.totalVehicle = stoi(tokens[15]);
                data.crashDay = tokens[5];

                crashdata.push_back(data);  // Add data to the vector
                crashDataMap.AddItem(data.caseNumber, data.latitude, data.longitude, data.totalVehicle, data.crashDay);  // Add data to the hashmap
                RbtreeData.insert(data.caseNumber, data.latitude, data.longitude, data.totalVehicle);
            } catch (const exception& e) {
                continue;  // Skip to the next line if there's an error
            }
        }
    }
}

void createGraphHash(bHash& crashDataMap){
   vector<double> latitudeVec;
   vector<double> longitudeVec;
   vector<double> totalVehiclesVec;

    for(int i = 0; i < crashDataMap.tableSize; i++){
        auto* ptr = crashDataMap.hashTable[i];
        while(ptr != nullptr && ptr->caseNumber != 0){
            int caseNumber = ptr->caseNumber;
            double latitude = ptr->latitude;
            double longtitude = ptr->longitude;
            int totalVehicle = ptr->totalVehicle;
            latitudeVec.push_back(latitude);
            longitudeVec.push_back(longtitude);
            totalVehiclesVec.push_back(totalVehicle);
            ptr = ptr->next;
        }
    }

    vector<double> log_totalVehiclesVec;
    transform(totalVehiclesVec.begin(), totalVehiclesVec.end(), std::back_inserter(log_totalVehiclesVec),
              [](double x) { return log(x + 2); });

    vector<double> scaledLogTotalVehiclesVec;
    transform(log_totalVehiclesVec.begin(), log_totalVehiclesVec.end(), back_inserter(scaledLogTotalVehiclesVec),
              [](double x) { return x * 0.01; });  // Adjust the scaling factor as needed


    xlim({-20, -100});
    ylim({10, 60});
    title("Scatter Plot of Number of Vehicles Involved In Traffic Crashes");
    xlabel("Longitude");
    ylabel("Latitude");
    zlabel("Total Vehicles");

    //scatter(longitudeVec, latitudeVec, totalVehiclesVec);
    geobubble(latitudeVec, longitudeVec, scaledLogTotalVehiclesVec);
    save("Images/MapHash.Jpeg", "Jpeg");
    show();
}

void createGraphRB(RBTree& RbtreeData) {
    vector<double> latitudeVec;
    vector<double> longitudeVec;
    vector<double> totalVehiclesVec;

    //retrieve the data through inorder traversal, and add it to the vector,
    RbtreeData.inorder(latitudeVec, longitudeVec, totalVehiclesVec);

    vector<double> log_totalVehiclesVec;
    transform(totalVehiclesVec.begin(), totalVehiclesVec.end(), std::back_inserter(log_totalVehiclesVec),
              [](double x) { return log(x + 2); });

    vector<double> scaledLogTotalVehiclesVec;
    transform(log_totalVehiclesVec.begin(), log_totalVehiclesVec.end(), back_inserter(scaledLogTotalVehiclesVec),
              [](double x) { return x * 0.01; });  // Adjust the scaling factor as needed

    //set parameters
    xlim({-20, -100});
    ylim({10, 60});
    title("Scatter Plot of Number of Vehicles Involved In Traffic Crashes");
    xlabel("Longitude");
    ylabel("Latitude");
    zlabel("Total Vehicles");

    //scatter(longitudeVec, latitudeVec, totalVehiclesVec);
    geobubble(latitudeVec, longitudeVec, scaledLogTotalVehiclesVec);
    save("Images/MapR&B.Jpeg", "Jpeg");
    show();
}


void menu(bHash& crashDataMap, RBTree& RbtreeData){
    cout << "Choose an option: " << endl;
    cout << "Option 1: Hash Map Data Structure" << endl;
    cout << "Option 2: Red and Black Tree Structure" << endl;
    int option;
    cout << "Enter option (1 or 2): " << endl;
    cin >> option;
    if(option == 1) {
        cout << "Generating Map Using Hash Map..." << endl;
        createGraphHash(crashDataMap);
    }
    else if(option == 2) {
        std::cout << "Generating Map Using Red and Black Tree..." << endl;
        createGraphRB(RbtreeData);
    }
    else{
        cout << "Invalid choice. Please enter 1 or 2." << endl;
        menu(crashDataMap, RbtreeData);
    }
}

int main() {
    string filename = "Traffic_Crashes.csv";
    vector<crashData> crashdata;
    bHash crashDataMap;
    RBTree RbtreeData;
    parseCSV(filename, crashdata, crashDataMap, RbtreeData);
    //menu(crashDataMap, RbtreeData);
    cout << "Choose an option: " << endl;
    cout << "Option 1: Hash Map Data Structure" << endl;
    cout << "Option 2: Red and Black Tree Structure" << endl;
    int option;
    cout << "Enter option (1 or 2): " << endl;
    cin >> option;
    if(option == 1) {
        cout << "Generating Map Using Hash Map..." << endl;
        createGraphHash(crashDataMap);
    }
    if(option == 2) {
        cout << "Generating Map Using Red and Black Tree..." << endl;
        createGraphRB(RbtreeData);
    }
    else{
        cout << "Invalid choice. Please enter 1 or 2." << endl;
        menu(crashDataMap, RbtreeData);
    }





//     Print the data from the vector (retaining original functionality)
//    for (const auto& data : crashdata) {
//        cout << "Case Number: " << data.caseNumber
//             << ", Latitude: " << data.latitude
//             << ", Longitude: " << data.longitude
//             << ", Total People: " << data.totalPeople
//             << ", Crash Day: " << data.crashDay << endl;
//    }
//     Print the size of the hashmap
//    cout << "Total items in the hashmap: " << crashDataMap.GetTotalItems() << endl;
//    crashDataMap.FindLatLon(216018195);
//    crashDataMap.FindLatLon(220006661);
//    crashDataMap.FindLatLon(220012025);
//    NodePtr resultNode = RbtreeData.searchTree(216019195);
//    cout << get<0>(resultNode->values) << " " << get<1>(resultNode->values) << endl;
//
//     Optionally, you can interact with the crashDataMap hashmap as needed
//     Example: crashDataMap.PrintTable();
//
    return 0;
}
