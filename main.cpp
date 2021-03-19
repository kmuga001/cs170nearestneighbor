#include <iostream>

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
#include <float.h>
#include <math.h>

using namespace std;

void search_dataset(vector< vector<double> > data);
bool checkFeatureInVector(vector<double> v, int val);
double leave_one_out_cross_validation(vector< vector<double> > data, vector<double> currentFeatures, int feature_add);


int main() {
    fstream ifs;
    vector< vector<double> > data;
    string str;
    string line;
    float tempD;
    
    ifs.open("test95.txt");
    //ifs.open("CS170_SMALLtestdata__42.txt");
    //ifs.open("CS170_small_special_testdata__95.txt");
    
    //char delimiterSpace; --> caused the issue and took out the negative signs, don't use!!!
    while (getline(ifs, line)) { //get row
        stringstream iss;
        iss << line;
        
        vector<double> inner;
        while (iss >> tempD) { //get data value at column--> changing negative to positive --> fix
            cout << "TEMPD: " << tempD << endl;
            inner.push_back(tempD);
        }
        data.push_back(inner);
    }
    cout << data[0][0] << " " << data[0][1] << " " << data[0][2] << endl;

   
    //cout << "# of Columns: " << data[0].size() << endl;
    
    search_dataset(data);


    return 0;
}


void search_dataset(vector< vector<double> > data) { //remember that features start on column 1 (not 0)
    vector<double> currentFeatures;
    double bestFinalAccuracy = 0;
    vector<double> bestlist;
    double inital_accuracy = leave_one_out_cross_validation(data, currentFeatures, 0);
    for(int i = 1; i < data[0].size(); i++){ //data[0] because we want number of columns as those are the features' data
        cout << "I am on the " << i << "th level" << endl;
        double feature_added = 0;
        double bestAccuracy = 0;

        for(int j = 1; j < data[0].size(); j++) {
            if(checkFeatureInVector(currentFeatures, j) == false){ //check if the feature is in the currentFeatures set: false = no
                cout << "Considering to add feature #" << j << endl;
                double accuracy = leave_one_out_cross_validation(data, currentFeatures, j);

                if(accuracy > bestAccuracy){
                    bestAccuracy = accuracy;
                    feature_added = j;
                    cout << "HI: " << bestAccuracy << endl;
                }
            }
            
        }
        //things are different here, need to see which features are being added when
        
        if(bestAccuracy > bestFinalAccuracy){
            bestFinalAccuracy = bestAccuracy;
            //bestlist = currentFeatures; //might not be good - push_back(feature_added)
            bestlist.push_back(feature_added);
            cout << "ULTIMATE: " << bestAccuracy << endl;
        }
        currentFeatures.push_back(feature_added);
        cout << "I added feature #" << feature_added << " to the set." << endl;
        //print feature set
        cout << "final feature set: {";
        for(int i = 0; i < currentFeatures.size(); i++){
            cout << currentFeatures.at(i) << ", ";
        }
        cout << "}" << endl;
    }
    cout << "The final best accuracy percentage is: " << bestFinalAccuracy << endl;
    cout << "final feature set: {";
    for(int i = 0; i < bestlist.size(); i++){
        cout << bestlist.at(i) + 1 << ", ";
    }
    cout << "}" << endl;
}

bool checkFeatureInVector(vector<double> v, int val) {
    for(int i = 0; i < v.size(); i++){
        if(v.at(i) == val){
            return true;
        }
    }

    return false;
}

double leave_one_out_cross_validation(vector< vector<double> > data, vector<double> currentFeatures, int feature_add) {
    
    vector<double> object_to_classify;
    double num_correct = 0;
    double label_object_to_classify = 0;
    for(int i = 0; i < data.size(); i++) { //for each row/object: each row represents an object (classification: feature data)
        object_to_classify = data[i];
        object_to_classify.erase(object_to_classify.begin()); //pop front, so that we can get just the feature data part and not label
        label_object_to_classify = data[i][0]; //first column of data

        double nn_distance = DBL_MAX;
        double nn_location = DBL_MAX;
        double nn_label = 0;
        //cout << "looping over i, at the " << i+1 << " location." << endl;
        //cout << "the " << i+1 << "th object is in class " << label_object_to_classify << endl;
        for(int k = 0; k < data.size(); k++){
            if(k != i){ //don't compare to yourself
                //calculate distance
                double distance = 0;
                vector<double> currData_features = data[k];
                currData_features.erase(currData_features.begin());
                double currData_label = data[k][0];

                double sum = 0; //euclidean distance code here, need to loop to get sum of differences in data points
                for(int j = 0; j < object_to_classify.size(); j++){
                    //the checkFeatureinVector function runs through the features each time --> causes slower runtime
                    if((checkFeatureInVector(currentFeatures, j) == true) || (feature_add == j)){ //only check features in currentfeatures and feature_add, rest are ignored
                        double diff = object_to_classify.at(j) - currData_features.at(j);
                        sum += pow(diff, 2);
                    }
                }
                distance = sqrt(sum);

                if(distance < nn_distance){
                    nn_distance = distance;
                    nn_location = k;
                    nn_label = data[nn_location][0];
                }


            }
        }
        if(label_object_to_classify == nn_label){ //check if the label was correct
            num_correct += 1;
        }

    }
    double accuracy = num_correct/(data.size());
    //cout << "VALIDATION ACCURACY: " << accuracy << endl;
    return accuracy; //returned accuracy

}