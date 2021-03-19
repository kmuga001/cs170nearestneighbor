#include <iostream>
#include <chrono>
#include <time.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>
#include <float.h>
#include <math.h>
//#include <bits/stdc++.h> 

using namespace std;

void search_dataset(vector< vector<double> > data, int alg);
bool checkFeatureInVector(vector<double> v, int val);
double leave_one_out_cross_validation(int alg, vector< vector<double> > data, vector<double> currentFeatures, int feature_add);


int main() {
    cout << "Welcome to Kinjal Mugatwala's Feature Selection Algorithm. " << endl;
    fstream ifs;
    vector< vector<double> > data;
    string str;
    string line;
    string filename;
    float tempD;
    cout << "Please input a text file to test: " << endl;
    cin >> filename;
    ifs.open(filename);
    
    //ifs.open("CS170_SMALLtestdata__42.txt");
    //ifs.open("CS170_small_special_testdata__95.txt");
    //ifs.open("CS170_largetestdata__20.txt")
    //char delimiterSpace; --> caused the issue and took out the negative signs, don't use!!!
    while (getline(ifs, line)) { //get row
        stringstream iss;
        iss << line;
        
        vector<double> inner;
        while (iss >> tempD) { //get data value at column--> changing negative to positive --> fix
            
            inner.push_back(tempD);
        }
        data.push_back(inner);
    }
    
    
    cout << "Type the number of the algorithm you want to run (1 = forward selection, 2 = backwards elimination)" << endl;
    int alg = 0;
    cin >> alg;
    cout << "This dataset has " << data[0].size() - 1 << " features and " << data.size() << " instances." << endl;

    auto start = chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(false);
    search_dataset(data, alg);
    auto end = chrono::high_resolution_clock::now(); 
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9; 
    
    cout << "FINAL TIME TAKEN: " << fixed << time_taken << setprecision(9) << " seconds" << endl;
    
    return 0;
}


void search_dataset(vector< vector<double> > data, int alg) { //remember that features start on column 1 (not 0)
    /*cout << "Type the number of the algorithm you want to run (1 = forward selection, 2 = backwards elimination)" << endl;
    int alg = 0;
    cin >> alg;*/
    
    cout << "Beginning search." << endl;
    vector<double> currentFeatures;
    double bestFinalAccuracy = 0;
    vector<double> bestlist;

    if(alg == 2){ //BE starts with all features
        for(int i = 1; i < data[0].size(); i++){
            currentFeatures.push_back(i);
            bestlist.push_back(i);
        }
    }



    double allfour_accuracy = leave_one_out_cross_validation(1, data, currentFeatures, 0);
    cout << "Running nearest neighbor with all 4 features, using “leaving-one-out” evaluation, I get an accuracy of " << allfour_accuracy << endl;
    for(int i = 1; i < data[0].size(); i++){ //data[0] because we want number of columns as those are the features' data
        
        double feature_added = 0;
        double bestAccuracy = 0;

        for(int j = 1; j < data[0].size(); j++) {
            if(alg == 1){ //FS
                if(checkFeatureInVector(currentFeatures, j) == false){ //check if the feature is in the currentFeatures set: false = no
                
                    double accuracy = leave_one_out_cross_validation(1, data, currentFeatures, j);

                    cout << "      Using feature(s) { ";
                    for (int z = 0; z < currentFeatures.size(); z++) {
                        cout << currentFeatures[z] << ", ";
                    }
                    cout << j; //feature we are on right now
                    cout << "} accuracy is " << accuracy << endl;


                    if(accuracy > bestAccuracy){
                        bestAccuracy = accuracy;
                        feature_added = j;
                        //cout << "HI: " << bestAccuracy << endl;
                        cout << "Feature set { ";
                        for (int z = 0; z < currentFeatures.size(); z++) {
                            cout << currentFeatures.at(z) << ", ";
                        }
                        cout << j; //feature added that has best accuracy
                        cout << "} was best, accuracy is " << bestAccuracy << endl << endl;
                    }
                }
                
            } else { //BE
                if(checkFeatureInVector(currentFeatures, j) == false){ //check if the feature is in the currentFeatures set
                
                    double accuracy = leave_one_out_cross_validation(2, data, currentFeatures, j);

                    cout << "      Using feature(s) { ";
                    for (int z = 0; z < currentFeatures.size(); z++) {
                        cout << currentFeatures.at(z) << ", ";
                    }
                    cout << j; //feature we are on right now
                    cout << "} accuracy is " << accuracy << endl;


                    if(accuracy > bestAccuracy){
                        bestAccuracy = accuracy;
                        feature_added = j; //need to remove this feature as this helped improve accuracy
                        
                        cout << "Feature set { ";
                        for (int z = 0; z < currentFeatures.size(); z++) {
                            cout << currentFeatures.at(z) << ", ";
                        }
                        cout << j; //feature added that has best accuracy
                        cout << "} was best, accuracy is " << bestAccuracy << endl << endl;
                    }
                }

            
            }
        } 

        if(bestAccuracy > bestFinalAccuracy){
            bestFinalAccuracy = bestAccuracy;
            //bestlist = currentFeatures; //might not be good - push_back(feature_added)
            if(alg == 1){
                bestlist.push_back(feature_added);
            } else {
                if(bestlist.size() > 1){
                    bestlist.erase(bestlist.begin() + (feature_added - 1));
                } else {
                    bestlist.erase(bestlist.begin());
                }
                    
                
                
            }
            
            
        }

        //update currentFeatures
        if(alg == 1){
            currentFeatures.push_back(feature_added);
        } else {
            if(currentFeatures.size() > 1){
                currentFeatures.erase(currentFeatures.begin() + (feature_added - 1));
                
            } else {
                currentFeatures.erase(currentFeatures.begin());
            }
            
        }
        
            
        if(bestAccuracy < bestFinalAccuracy){
            cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
        }
    }


    //best accuracy printing results
    
    //cout << "The final best accuracy percentage is: " << bestFinalAccuracy << endl;
    cout << "Finished search!!! Best Feature Set: {";
    for(int i = 0; i < bestlist.size(); i++){
        cout << bestlist.at(i) << ", ";
    }
    cout << "} accuracy is " << bestFinalAccuracy << endl << endl;
}

bool checkFeatureInVector(vector<double> v, int val) {
    for(int i = 0; i < v.size(); i++){
        if(v.at(i) == val){
            return true;
        }
    }

    return false;
}

double leave_one_out_cross_validation(int alg, vector< vector<double> > data, vector<double> currentFeatures, int feature_add) {
    vector< vector <double> > minidata;

    if(alg == 1){ //forward
        for(int i = 0; i < data.size(); i++){ //create a temporary structure so that we only need to look through certain features
            vector<double> objectfeatures;
            
            for(int j = 1; j < data[i].size(); j++){
                if((checkFeatureInVector(currentFeatures, j)) || (j == feature_add)) {
                    objectfeatures.push_back(data[i][j]);
                } else {
                    objectfeatures.push_back(0);
                }
            }
            minidata.push_back(objectfeatures);
        }
    } else { //backward
        for(int i = 0; i < data.size(); i++){ //create a temporary structure so that we only need to look through certain features
            vector<double> objectfeatures;
            
            for(int j = 1; j < data[i].size(); j++){
                if((checkFeatureInVector(currentFeatures, j)) == true) {
                    if(j == feature_add){
                        objectfeatures.push_back(0);
                    }
                    objectfeatures.push_back(data[i][j]);
                } else {
                    objectfeatures.push_back(0);
                }
            }
            minidata.push_back(objectfeatures);
        }
    }

    
    
    double num_correct = 0;
    double label_object_to_classify = 0;
    for(int i = 0; i < data.size(); i++) { //item 1 - for each row/object: each row represents an object (classification: feature data)
        
        label_object_to_classify = data[i][0]; //first column of data

        double nn_distance = DBL_MAX;
        double nn_location = DBL_MAX;
        double nn_label = 0;
        //cout << "looping over i, at the " << i+1 << " location." << endl;
        //cout << "the " << i+1 << "th object is in class " << label_object_to_classify << endl;
        for(int k = 0; k < data.size(); k++){ //item 2
            if(k != i){ //don't compare to yourself
                //calculate distance
                double distance = 0;
                double currData_label = data[k][0];

                double sum = 0; //euclidean distance code here, need to loop to get sum of differences in data points
                for(int j = 0; j < minidata[0].size(); j++){
                    //the checkFeatureinVector function runs through the features each time --> causes slower runtime
                    //if((checkFeatureInVector(currentFeatures, j) == true) || (feature_add == j)){ //only check features in currentfeatures and feature_add, rest are ignored
                    double diff = minidata[i].at(j) - minidata[k].at(j);
                    sum += pow(diff, 2);
                    //}
                }
                distance = sqrt(sum);

                if(distance < nn_distance){ //classify item based on smallest distance
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