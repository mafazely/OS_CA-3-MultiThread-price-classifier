// multi thread implementation
#include <bits/stdc++.h>
#include <iomanip>
#include <pthread.h>
#include <limits.h>

#define NUMBER_OF_THREADS 4
#define MAXVALUE 1000000000

using namespace std;

struct csvFile
{
    vector<vector<double>> numericMatrix;
    vector<string> headerFile;
};
typedef struct csvFile csv;
typedef map<string, vector<double>> minMaxStruct;

struct threadData
{
    csv dataset;
    double numOfCorrect;
    double total = dataset.numericMatrix.size();
};
typedef struct threadData thData;
