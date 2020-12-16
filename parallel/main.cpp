// multi thread implementation
#include <bits/stdc++.h>
#include <iomanip>
#include <pthread.h>

#define NUMBER_OF_THREADS 6
#define MAXVALUE 1e9

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
    minMaxStruct mms;
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

void* findTotalMinMax(void* args)
{
    thData *data = (thData *)args;

    data->dataset = ReadCSV(data->datasetFilepath);

    data->mms = getMinMaxColumn(data->dataset);

    pthread_exit(NULL);
}

void* calculator(void* args)
{
    thData* data = (thData*) args;

    csv Dataset;
    Dataset = data->dataset;

    csv weightDataset;
    weightDataset = ReadCSV(data->weightedFilepath);

    minMaxStruct minmax;
    minmax = data->mms;

    csv nomalizedDataset;
    nomalizedDataset = getNormalizedDateset(Dataset, minmax);

    vector<bool> rowCorrectness;
    rowCorrectness = isCorrectPrediction(nomalizedDataset, weightDataset);

    double numofcorrect = count(rowCorrectness.begin(), rowCorrectness.end(), true);
    
    data->numOfCorrect = numofcorrect;
    data->total = rowCorrectness.size();

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t threads[NUMBER_OF_THREADS];
    pthread_t threads2[NUMBER_OF_THREADS];
    int return_code, return_code2;

    double totalcorrect = 0;
    double totalentries = 0;

    // initializing
    for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
    {
        thread_data_array[tid].datasetFilepath = string(argv[1]) + "train_" + to_string((int)tid) + ".csv";
        thread_data_array[tid].weightedFilepath = string(argv[1]) + "weights.csv";
        thread_data_array[tid].numOfCorrect = 0;
        thread_data_array[tid].total = 0;
    }
    
    // create threads
    for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
    {
        return_code = pthread_create(&threads[tid], NULL, findTotalMinMax, (void *)&thread_data_array[tid]);

        if (return_code)
        {
            printf("ERROR; return code from pthread_create() is %d\n", return_code);
            exit(-1);
        }
    }

    // wait for threads to done their jobs[get MinMaxstruct]
    for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
    {
        return_code = pthread_join(threads[tid], NULL);
        if (return_code)
        {
            printf("ERROR; return code from pthread_join() is %d\n", return_code);
            exit(-1);
        }
    }

    vector<string> header;
    header = thread_data_array[0].dataset.headerFile;
    minMaxStruct minmaxfinal;
    // convert all mms to final minMax
    for (long i = 0; i < header.size()-1; i++)
    {
        double maxVal = 0;
        double minVal = MAXVALUE;
        for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
        {
            if (thread_data_array[tid].mms[header[i]][1] < minVal)
                minVal = thread_data_array[tid].mms[header[i]][1];

            if (thread_data_array[tid].mms[header[i]][0] > maxVal)
                maxVal = thread_data_array[tid].mms[header[i]][0];
        }
        minmaxfinal[header[i]] = {maxVal, minVal};
    }

    // initializing final minmaxmatrix
    for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
    {
        thread_data_array[tid].mms = minmaxfinal;
    }

    // create threads
    for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
    {
        return_code2 = pthread_create(&threads2[tid], NULL, calculator, (void *)&thread_data_array[tid]);

        if (return_code2)
        {
            printf("ERROR; return code from pthread_create() is %d\n", return_code2);
            exit(-1);
        }
    }

    // wait for threads to done their jobs[get final res]
    for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
    {
        return_code2 = pthread_join(threads2[tid], NULL);
        if (return_code2)
        {
            printf("ERROR; return code from pthread_join() is %d\n", return_code2);
            exit(-1);
        }
    }

    // get result value from each thread and calculate final result
    for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
    {
        totalcorrect += thread_data_array[tid].numOfCorrect;
        totalentries += thread_data_array[tid].total;
    }

    double answer = totalcorrect / totalentries;

    cout << fixed << setprecision(2) << answer * 100 << "%" << endl;

    pthread_exit(NULL);
}