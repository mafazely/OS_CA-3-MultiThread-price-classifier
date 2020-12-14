#include <bits/stdc++.h>

using namespace std;

struct csvFile
{
    vector<vector<double>> numericMatrix;
    vector<string> headerFile;
};
typedef struct csvFile csv;

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

map<string, double> getMinMaxColumn(csv dataset)
{
}

int main(int argc, char const *argv[])
{
    csv Dataset;
    Dataset = ReadCSV(argv[1]);

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

    cout << Dataset.headerFile.size() << " " << Dataset.numericMatrix.size() << endl;
    return 0;
}