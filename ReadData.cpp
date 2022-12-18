#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
using namespace std;

struct Array
{
    int rows;
    int cols;
    int **array;
};

struct Instance
{
    vector<vector<Array> > training;
    Array input;
    Array output;
    Array gt;
    Array initial;
};

// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
vector<string> split(string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

Instance read_file(string file_name)
{
    //std::cout << "File:" << file_name << std::endl;
    string line;
    ifstream myfile(file_name);

    Instance ins;

    if (myfile.is_open())
    {
        // Training size
        getline(myfile, line);

        int t_examples = stoi(line);
        for (int i = 0; i < t_examples; i++)
        {
            vector<Array> training_example;

            for (int j = 0; j < 2; j++)
            {
                getline(myfile, line);
                vector<string> s = split(line, " ");

                int rows = stoi(s.at(0));
                int cols = stoi(s.at(1));

                Array a;
                a.rows = rows;
                a.cols = cols;
                a.array = new int *[rows];

                for (int k = 0; k < rows; k++)
                {
                    a.array[k] = new int[cols];
                    getline(myfile, line);

                    vector<string> l = split(line, " ");

                    for (int x = 0; x < l.size(); x++)
                    {
                        a.array[k][x] = stoi(l.at(x));
                    }
                }

                training_example.push_back(a);
            }

            ins.training.push_back(training_example);
        }

        // Test input
        {
            getline(myfile, line);
            vector<string> s = split(line, " ");

            int rows = stoi(s.at(0));
            int cols = stoi(s.at(1));

            ins.input.rows = rows;
            ins.input.cols = cols;
            ins.input.array = new int *[rows];

            for (int k = 0; k < rows; k++)
            {
                ins.input.array[k] = new int[cols];

                getline(myfile, line);
                vector<string> l = split(line, " ");

                for (int x = 0; x < l.size(); x++)
                {
                    ins.input.array[k][x] = stoi(l.at(x));
                }
            }
        }

        // Test output
        {
            getline(myfile, line);
            vector<string> s = split(line, " ");

            int rows = stoi(s.at(0));
            int cols = stoi(s.at(1));

            ins.output.rows = rows;
            ins.output.cols = cols;
            ins.output.array = new int *[rows];

            ins.gt.rows = rows;
            ins.gt.cols = cols;
            ins.gt.array = new int *[rows];

            for (int k = 0; k < rows; k++)
            {
                ins.output.array[k] = new int[cols];
                ins.gt.array[k] = new int[cols];

                getline(myfile, line);
                vector<string> l = split(line, " ");

                for (int x = 0; x < l.size(); x++)
                {
                    ins.output.array[k][x] = stoi(l.at(x));
                    ins.gt.array[k][x] = stoi(l.at(x));
                }
            }
        }

        {
            getline(myfile, line);
            vector<string> s = split(line, " ");

            int rows = stoi(s.at(0));
            int cols = stoi(s.at(1));

            ins.initial.rows = rows;
            ins.initial.cols = cols;
            ins.initial.array = new int *[rows];

            for (int k = 0; k < rows; k++)
            {
                ins.initial.array[k] = new int[cols];

                getline(myfile, line);
                vector<string> l = split(line, " ");

                for (int x = 0; x < l.size(); x++)
                {
                    ins.initial.array[k][x] = stoi(l.at(x));
                }
            }
        }

        myfile.close();
    }
    else
        cout << "Unable to open file";

    return ins;
}

static Instance *read_dir(const char *path)
{
    Instance *output;

    vector<Instance> ins;

    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        return NULL;
    }

    string s = path;
    s += "/";

    while ((entry = readdir(dir)) != NULL)
        if (entry->d_name[0] != '.')
        {
            ins.push_back(read_file(s + entry->d_name));
        }

    closedir(dir);

    output = new Instance[ins.size()];

    for (int i = 0; i < ins.size(); i++)
    {
        output[i] = ins.at(i);
    }

    return output;
}

/*
int main()
{
    read_dir("./data");
}
*/
