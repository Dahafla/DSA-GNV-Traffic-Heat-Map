#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

//struct to hold different attributes of the data
struct crashData{
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
void parseCSV(const string& filename, vector<crashData>& crashdata){
    ifstream file(filename);
    //check if file is open or not
    if(!file.is_open()){
        cout << "file can't open: error" << endl;
        return;
    }
    string line;
    getline(file, line); // skip header line

    while(getline(file, line)){
        vector<string> tokens = split(line, ',');
        if(tokens.size() >= 23){
            crashData data;
            if(!tokens[22].empty() && !tokens[23].empty()) {
                try {
                    data.longitude = stod(tokens[22]);
                    data.latitude = stod(tokens[23]);
                } catch (const exception& e){
                    continue;
                }
            }
            else{
                continue;
            }
            try {
                data.totalPeople = stoi(tokens[12]);
            }
            catch (const exception& e){
                continue;
            }
            data.crashDay = tokens[5];

            crashdata.push_back(data);
        }
    }
}

int main() {
    string filename;
    filename = "Traffic_Crashes.csv";
    vector<crashData> crashdata;
    parseCSV(filename, crashdata);


    for (const auto& data : crashdata) {
        cout << "Latitude: " << data.latitude << ", Longitude: " << data.longitude
             << ", Total People: " << data.totalPeople << ", Crash Day: " << data.crashDay << endl;
    }

    return 0;
}
