#include <iostream>
#include <fstream>
#include <openmpi/mpi.h>

int compar(const void *a, const void *b)
{
    if (*(double *)a > *(double *)b)
    {
        return 1;
    }
    else if (*(double *)a < *(double *)b)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int find_bucket(double num, int p_num)
{
    int x;
    for (x = 1; x < p_num + 1; x++)
    {
        double bucket_range = (double)x / (double)p_num;
        if (num <= bucket_range)
        {
            return x - 1;
        }
    }
}

int main(int argc, char *argv[])
{
    int rank, size;
    for (long N = 1; N < 100000; N = N * 2)
    {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double array[N];
    double scatter_array[N];

    int counts[size];
    int offsets[size];
    int arr_idx[size];

    int local_count;

    double t1 = MPI_Wtime();

    if (rank == 0)
    {
        double x;
        for (int i = 0; i < N; i++)
        {
            x = (double)rand() / RAND_MAX;
            array[i] = x;

            int bucket;

            for (int j = 1; j < size + 1; j++)
            {
                double range = (double)j / size;
                if (x <= range)
                {
                    bucket = j - 1;
                }
            }

            counts[bucket]++;
        }
    }

    MPI_Scatter(counts, 1, MPI_INT, &local_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double local_array[local_count];

    if (rank == 0)
    {
        offsets[0] = 0;
        for (int i = 1; i < size; i++)
        {
            offsets[i] = counts[i - 1] + offsets[i - 1];
        }

        for (int i = 0; i < N; i++)
        {
            int bucket;
            for (int j = 1; j < size + 1; j++)
            {
                double range = (double)j / size;
                if (array[j] <= range)
                {
                    bucket = j - 1;
                }
            }
            scatter_array[offsets[bucket] + arr_idx[bucket]] = array[i];
            arr_idx[bucket]++;
        }
    }

    MPI_Scatterv(scatter_array, counts, offsets, MPI_DOUBLE, local_array, local_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    qsort(local_array, local_count, sizeof(double), compar);

    MPI_Gatherv(local_array, local_count, MPI_DOUBLE, scatter_array, counts, offsets, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double t2 = MPI_Wtime();

    std::ofstream myfile;
    myfile.open ("bsort_mpi.txt",std::ios::app);
    if (rank == 0)
    {
        myfile << t2 - t1<< std::endl;
    }
    myfile.close();
    MPI_Finalize();
    }
}