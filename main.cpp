#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void search_dataset(vector< vector<double> > data);
bool checkFeatureInVector(vector<int> v, int val);
int crossValidation();


int main() {
    ifstream ifs;
    vector< vector<double> > data;
    string str;
    string line;
    double tempDouble;
    ifs.open("test95.txt");
    
    char delimiterSpace;
    while (getline(ifs, line)) { //get row
        istringstream iss(line);
        vector<double> inner;
        while (iss >> tempDouble) { //get data value at column
            inner.push_back(tempDouble);
            iss >> delimiterSpace;
            //cout << inner.back() << endl;
        }
        data.push_back(inner);
    }

    ifs.close();
    
    cout << "# of Features: " << data[0].size() << endl;

    search_dataset(data);


    return 0;
}


void search_dataset(vector< vector<double> > data) { //remember that features start on column 1 (not 0)
    vector<int> currentFeatures;
    for(int i = 1; i < data[0].size(); i++){ //data[0] because we want number of columns as those are the features' data
        cout << "I am on the " << i << "th level" << endl;
        int feature_added = 0;
        double bestAccuracy = 0;

        for(int j = 1; j < data[0].size(); j++) {
            if(checkFeatureInVector(currentFeatures, j) == false){ //check if the feature is in the currentFeatures set: false = no
                cout << "Considering to add feature #" << j << endl;
                int accuracy = crossValidation();

                if(accuracy > bestAccuracy){
                    bestAccuracy = accuracy;
                    feature_added = j;
                }
            }
            
        }
        currentFeatures.push_back(feature_added);
        cout << "I added feature #" << feature_added << " to the set." << endl;
    }
}

bool checkFeatureInVector(vector<int> v, int val) {
    for(int i = 0; i < v.size(); i++){
        if(v.at(i) == val){
            return true;
        }
    }

    return false;
}

int crossValidation(){
    return 0;
}