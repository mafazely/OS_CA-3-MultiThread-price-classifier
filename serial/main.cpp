#include <bits/stdc++.h>

#define MAXVALUE 1000000000


using namespace std;

struct csvFile
{
    vector<vector<double>> numericMatrix;
    vector<string> headerFile;
};
typedef struct csvFile csv;
typedef map<string, vector<double>> minMaxStruct;

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
    for (size_t i = 0; i < dataset.numericMatrix[0].size()-1; i++)
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



int main(int argc, char const *argv[])
{
    csv Dataset;
    Dataset = ReadCSV(argv[1]);

    minMaxStruct minmax;
    minmax = getMinMaxColumn(Dataset);

    csv nomalizedDataset;
    nomalizedDataset = getNormalizedDateset(Dataset, minmax);

    for (size_t i = 0; i < Dataset.headerFile.size(); i++)
    {
        cout << Dataset.headerFile[i] << " ";
    }
    cout << endl;
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < Dataset.numericMatrix[0].size(); j++)
        {
            cout << Dataset.numericMatrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for (size_t i = 0; i < Dataset.headerFile.size()-1; i++)
    {
        cout << "column : " << Dataset.headerFile[i] << "\t # max :" <<minmax[Dataset.headerFile[i]][0]
        << "\t # min :" << minmax[Dataset.headerFile[i]][1] << endl;
    }
    
    cout << Dataset.headerFile.size() << " " << Dataset.numericMatrix.size() << endl;

    cout << endl;
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < nomalizedDataset.numericMatrix[0].size(); j++)
        {
            cout << nomalizedDataset.numericMatrix[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}