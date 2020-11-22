#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void bucketSort(double arr[], int n)
{
    vector<double> b[n];

    int i;
    #pragma omp parallel for private(i)
    for (i = 0; i < n; i++)
    {
        int bi = n * arr[i];
        b[bi].push_back(arr[i]);
    }

    #pragma omp parallel for private(i)
    for (i = 0; i < n; i++)
        sort(b[i].begin(), b[i].end());

    int index = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < b[i].size(); j++)
            arr[index++] = b[i][j];
}

int main(int argc, char *argv[])
{
    int n = 1000;
    double array[n];

    double x;
    for (int i = 0; i < n; i++)
    {
        x = (double)rand() / RAND_MAX;
        array[i] = x;
    }

    bucketSort(array, n);

    cout << "Sorted array is \n";
    for (int i = 0; i < n; i++)
        cout << array[i] << " ";
    return 0;
}