#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
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

struct uniqueCoords {
    int caseNumber;
    double latitude;
    double longitude;
    int totalVehicleSum;
   uniqueCoords(int caseNum, double lat, double lon, int sum) : caseNumber(caseNum), latitude(lat), longitude(lon), totalVehicleSum(sum) {}

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

    vector<uniqueCoords> uniqueC;

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

                crashdata.push_back(data);
                //iterate through uniqueC vector and check if there's any duplicates
                auto it = find_if(uniqueC.begin(), uniqueC.end(), [&](const uniqueCoords& uc){
                    return uc.latitude == data.latitude && uc.longitude == data.longitude;
                });

                if(it != uniqueC.end()){
                    it->totalVehicleSum += data.totalVehicle;
                }
                else{
                    uniqueC.emplace_back(data.caseNumber, data.latitude, data.longitude, data.totalVehicle);
                }
            } catch (const exception& e) {
                continue;  // Skip to the next line if there's an error
            }
        }
    }

    for(auto a: uniqueC){
        crashDataMap.AddItem(a.caseNumber,a.latitude, a.longitude, a.totalVehicleSum);  // Add data to the hashmap
        RbtreeData.insert(a.caseNumber, a.latitude, a.longitude, a.totalVehicleSum);
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
            double longitude = ptr->longitude;
            int totalVehicle = ptr->totalVehicle;
            latitudeVec.push_back(latitude);
            longitudeVec.push_back(longitude);
            totalVehiclesVec.push_back(totalVehicle);
            ptr = ptr->next;
        }
    }

    vector<double> log_totalVehiclesVec;
    transform(totalVehiclesVec.begin(), totalVehiclesVec.end(), std::back_inserter(log_totalVehiclesVec),
              [](double x) { return log(x + 2); });

    vector<double> scaledLogTotalVehiclesVec;
    transform(log_totalVehiclesVec.begin(), log_totalVehiclesVec.end(), back_inserter(scaledLogTotalVehiclesVec),
              [](double x) { return x * 0.3; });  // Adjust the scaling factor as needed

    auto c = linspace(1, 10000, totalVehiclesVec.size());

    xlim({-82.1, -82.5});
    ylim({29.5734, 29.75});
    title("Scatter Plot of Number of Vehicles Involved In Traffic Crashes");
    xlabel("Longitude");
    ylabel("Latitude");
    zlabel("Total Vehicles");
    colorbar();

    scatter(longitudeVec, latitudeVec, scaledLogTotalVehiclesVec, c);
    //save("Images/MapHash.png", "png");
    show();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
              [](double x) { return x * 0.3; });  // Adjust the scaling factor as needed

    //set parameters
    xlim({-82.1, -82.5});
    ylim({29.5734, 29.75});
    title("Scatter Plot of Number of Vehicles Involved In Traffic Crashes");
    xlabel("Longitude");
    ylabel("Latitude");
    zlabel("Total Vehicles");
    scatter(longitudeVec, latitudeVec, scaledLogTotalVehiclesVec);
    save("Images/MapR&B.png", "png");
    show();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


void menu(bHash& crashDataMap, RBTree& RbtreeData){
    int option;

    while (true) {
        cout << "Choose an option: " << endl;
        cout << "Option 1: Hash Map Data Structure" << endl;
        cout << "Option 2: Red and Black Tree Structure" << endl;
        cout << "Enter option (1 or 2): " << endl;

        if (!(cin >> option) || (option != 1 && option != 2)) {
            cout << "Invalid input. Please enter 1 or 2." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }

    if (option == 1) {
        cout << "Generating Map Using Hash Map..." << endl;
        createGraphHash(crashDataMap);
    } else if (option == 2) {
        cout << "Generating Map Using Red and Black Tree..." << endl;
        createGraphRB(RbtreeData);
    }
}

int main() {
    string filename = "Traffic_Crashes.csv";
    vector<crashData> crashdata;
    bHash crashDataMap;
    RBTree RbtreeData;
    parseCSV(filename, crashdata, crashDataMap, RbtreeData);
    menu(crashDataMap, RbtreeData);
    //createGraphHash(crashDataMap);
    //createGraphRB(RbtreeData);

//    cout << "Choose an option: " << endl;
//    cout << "Option 1: Hash Map Data Structure" << endl;
//    cout << "Option 2: Red and Black Tree Structure" << endl;
//    int option;
//    cout << "Enter option (1 or 2): " << endl;
//    cin >> option;
//    if(option == 1) {
//        cout << "Generating Map Using Hash Map..." << endl;
//        createGraphHash(crashDataMap);
//    }
//    if(option == 2) {
//        cout << "Generating Map Using Red and Black Tree..." << endl;
//        createGraphRB(RbtreeData);
//    }
//    else{
//        cout << "Invalid choice. Please enter 1 or 2." << endl;
//        menu(crashDataMap, RbtreeData);
//    }

//     Print the data from the vector (retaining original functionality)
//    for (const auto& data : crashdata) {
//        cout << "Case Number: " << data.caseNumber
//             << ", Latitude: " << data.latitude
//             << ", Longitude: " << data.longitude
//             << ", Total People: " << data.totalPeople
//             << ", Crash Day: " << data.crashDay << endl;
//    }
//     Print the size of the hashmap
    //cout << "Total items in the hashmap: " << crashDataMap.GetTotalItems() << endl;
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
