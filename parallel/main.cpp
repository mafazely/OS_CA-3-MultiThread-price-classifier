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
    //csv dataset;
    string datasetFilepath;
    string weightedFilepath;
    double numOfCorrect;
    double total;
};
typedef struct threadData thData;

thData thread_data_array[NUMBER_OF_THREADS];

csv ReadCSV(string file_path)
{
    csv csvfile;
    ifstream infile(file_path);
    string line;
    vector<vector<double>> arguments;
    vector<string> header;
    int header_flag = 0;
    while (getline(infile, line))
    {
        vector<double> temp;
        stringstream ss(line);
        if (!header_flag)
        {
            while (ss.good())
            {
                string substr;
                getline(ss, substr, ',');
                header.push_back(substr);
            }
            header_flag = 1;
        }
        else
        {
            while (ss.good())
            {
                string substr;
                getline(ss, substr, ',');
                temp.push_back(atof(substr.c_str()));
            }
            arguments.push_back(temp);
        }
    }
    csvfile.headerFile = header;
    csvfile.numericMatrix = arguments;
    return csvfile;
}

minMaxStruct getMinMaxColumn(csv dataset)
{
    minMaxStruct minMaxValue;
    for (size_t i = 0; i < dataset.numericMatrix[0].size() - 1; i++)
    {
        vector<double> temp;
        double maxVal = 0;
        double minVal = MAXVALUE;
        string headerName = dataset.headerFile[i];

        for (size_t j = 0; j < dataset.numericMatrix.size(); j++)
        {
            if (dataset.numericMatrix[j][i] < minVal)
                minVal = dataset.numericMatrix[j][i];

            if (dataset.numericMatrix[j][i] > maxVal)
                maxVal = dataset.numericMatrix[j][i];
        }
        temp.push_back(maxVal);
        temp.push_back(minVal);
        minMaxValue.insert({headerName, temp}); // .f.g { headerName, {maxValue, minValue} }
    }
    return minMaxValue;
}

csv getNormalizedDateset(csv dataset, minMaxStruct mmstruct)
{
    csv csvfile;
    csvfile.numericMatrix = dataset.numericMatrix;
    //vector<vector<double>> elements;
    dataset.headerFile.pop_back(); // for price column
    csvfile.headerFile = dataset.headerFile;
    for (size_t i = 0; i < dataset.numericMatrix[0].size() - 1; i++)
    {
        double maxVal = mmstruct[dataset.headerFile[i]][0];
        double minVal = mmstruct[dataset.headerFile[i]][1];
        double diff = maxVal - minVal;
        vector<double> temp;
        for (size_t j = 0; j < dataset.numericMatrix.size(); j++)
        {
            //temp.push_back((dataset.numericMatrix[j][i] - minVal) / diff);
            csvfile.numericMatrix[j][i] = (dataset.numericMatrix[j][i] - minVal) / diff;
        }
        //csvfile.numericMatrix.push_back(temp);
    }
    return csvfile;
}

vector<bool> isCorrectPrediction(csv normalizedDataset, csv weightedDataset)
{
    vector<bool> rowState;
    for (size_t i = 0; i < normalizedDataset.numericMatrix.size(); i++)
    {
        vector<double> AllWeightRow;
        for (size_t j = 0; j < weightedDataset.numericMatrix.size(); j++)
        {
            // calcute Dot product
            double rowSum = 0;
            for (size_t k = 0; k < normalizedDataset.numericMatrix[0].size() - 1; k++)
            {
                rowSum += normalizedDataset.numericMatrix[i][k] * weightedDataset.numericMatrix[j][k];
            }
            rowSum += weightedDataset.numericMatrix[j][weightedDataset.numericMatrix[0].size() - 1]; // add bias

            AllWeightRow.push_back(rowSum);
        }

        double maxWeight = *max_element(AllWeightRow.begin(), AllWeightRow.end());
        ptrdiff_t pos = distance(AllWeightRow.begin(), find(AllWeightRow.begin(), AllWeightRow.end(), maxWeight));

        if (pos == normalizedDataset.numericMatrix[i][normalizedDataset.numericMatrix[0].size() - 1])
        {
            rowState.push_back(true);
        }
        else
        {
            rowState.push_back(false);
        }
    }
    return rowState;
}

void* calculator(void* args)
{
    thData* data = (thData*) args;

    csv Dataset;
    Dataset = ReadCSV(data->datasetFilepath);

    csv weightDataset;
    weightDataset = ReadCSV(data->weightedFilepath);

    minMaxStruct minmax;
    minmax = getMinMaxColumn(Dataset);

    csv nomalizedDataset;
    nomalizedDataset = getNormalizedDateset(Dataset, minmax);

    vector<bool> rowCorrectness;
    rowCorrectness = isCorrectPrediction(nomalizedDataset, weightDataset);

    double numofcorrect = count(rowCorrectness.begin(), rowCorrectness.end(), true);
    
    data->numOfCorrect = numofcorrect;
    data->total = rowCorrectness.size();

    pthread_exit(NULL);
}   