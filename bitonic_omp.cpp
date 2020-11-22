#include <iostream>
#include <omp.h>
#include <cstdlib>

using namespace std;

void merge(long long *arr, long long n, long long m)
{
    long long step = n / 2, i, j, k, temp;
    while (step > 0)
    {
        for (i = 0; i < n; i += step * 2)
        {
            for (j = i, k = 0; k < step; j++, k++)
            {
                if (m == 1)
                {
                    if (arr[j] > arr[j + step])
                    {
                        // swap
                        temp = arr[j];
                        arr[j] = arr[j + step];
                        arr[j + step] = temp;
                    }
                }
                else
                {
                    if (arr[j] < arr[j + step])
                    {
                        // swap
                        temp = arr[j];
                        arr[j] = arr[j + step];
                        arr[j + step] = temp;
                    }
                }
            }
        }
        step /= 2;
    }
}

void printArray(long long *arr, long long n)
{
    long long i;
    cout << "[" << arr[0];
    for (i = 1; i < n; i++)
    {
        cout << "," << arr[i];
    }
    cout << "]\n";
}

int main(int argc, char *argv[])
{
    long long n, *arr, i, s;
    n = atoll(argv[1]);
    arr = (long long *)malloc(n * sizeof(long long));
    for (i = 0; i < n; i++)
    {
        arr[i] = (long long) rand();
    }
    // print array before
    printArray(arr, n);

    omp_set_num_threads(12);

    double t1 = omp_get_wtime();

    // do merges
    for (s = 2; s <= n; s *= 2)
    {
        #pragma omp parallel for private(i)
        for (i = 0; i < n; i += s * 2)
        {
            if ((i / s) % 2 == 0)
            {
                merge(arr + i, s, 1);
            }
            else
            {
                merge(arr + i + s, s, 0);
            }
        }
    }

    double t2 = omp_get_wtime();

    printArray(arr, n);
    cout << "Tiempo: " << t2 - t1 << endl;
}
