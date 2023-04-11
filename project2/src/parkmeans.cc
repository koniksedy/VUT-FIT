/**
 * @file parkmeans.cc
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief The implementation of the parallel 4-Means clustering using OpenMPI (PRL - project 2)
 * @date 04.04.2023
 */


#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>

#define ROOT 0
#define CLUSTERS_CNT 4
#define MIN_NUMBERS 4
#define MAX_NUMBERS 32


using namespace std;


/**
 * @brief Loads numbers (in the form of bytes) from a file.
 *
 * @param file_name Input file with the numbers.
 * @return vector The vector of loaded numbers.
 */
vector<unsigned char> load_numbers(const char *file_name)
{
    ifstream input(file_name, ios::binary);

    if (!input.good())
    {
        cerr << "Input file not found." << endl;
    }

    vector<unsigned char> numbers((istreambuf_iterator<char>(input)),
                         (istreambuf_iterator<char>()));

    return numbers;
}

/**
 * @brief Returns the index of the closest cluster.
 *
 * @param point A point being examined.
 * @param means An array of all cluster means.
 * @return unsigned char The index of the closer cluster
 */
unsigned char get_best_cluster(const unsigned char point, const float means[])
{
    unsigned char idx = CLUSTERS_CNT;
    float min_distance = 255;
    for (int i = 0; i < CLUSTERS_CNT; ++i)
    {
        float distance = abs(means[i] - point);
        if (distance < min_distance)
        {
            min_distance = distance;
            idx = i;
        }
    }

    return idx;
}

/**
 * @brief Prints the result of K-means clustering for CLUSTERS_CNT (4) clusters.
 *
 * @param means An array of cluster means.
 * @param clusters An array of vectors of clusters and its points.
 * @param flags An array of vectors of flags. If one, then the point belongs to that cluster.
 */
void print_results(float means[], vector<unsigned char> clusters[], vector<unsigned char> flags[])
{
    for (int i = 0; i < CLUSTERS_CNT; ++i)
    {
        // Sometimes the count of uniq numbers can be less than number of clusters.
        if (!isnan(means[i]))
        {
            printf("[%.1f] ", means[i]);
            for (int j = 0; j < clusters[i].size(); ++j)
            {
                if (flags[i][j] == 1)
                {
                    printf("%d ", clusters[i][j]);
                }
            }
            printf("\n");
        }
    }
}

// *********************
// *       MAIN        *
// *********************
int main(int argc, char *argv[])
{
    // Initializing MPI
    int rank;
    int size;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Input variable
    vector<unsigned char> numbers;

    // Working variables
    bool changed_glob = true;
    bool changed_loc = true;
    unsigned char number_loc;
    float means[4];                                 // The array of cluster means.
    unsigned char cluster_idx = CLUSTERS_CNT;       // The index of the cluster, the point is part of.
    int clusters_loc[4] = {0,};                     // The point is assigned to its cluster segment.
    unsigned char cluster_affiliation[4] = {0,};    // 1 indicates point in the cluster.

    // Reduction variables for new means
    int clusters_sum[4] = {0,};
    unsigned char clusters_size[4] = {0,};

    // Output variables
    vector<unsigned char> clusters[4];
    vector<unsigned char> clusters_flags[4];


    if (rank == ROOT)
    {
        numbers = load_numbers("numbers");

        // Testing the correctness of the numbers count.
        if (numbers.size() < MIN_NUMBERS)
        {
            cerr << "Not enough numbers." << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
            MPI_Finalize();
        }

        // Slicing numbers if too many.
        if (numbers.size() > MAX_NUMBERS)
        {
            numbers.resize(MAX_NUMBERS);
        }

        // Testing number of processors.
        if (numbers.size() != size)
        {
            cerr << "The number of processors must be equal to the count of numbers." << endl;
            cerr << size << " " << numbers.size() << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
            MPI_Finalize();
        }

        // Init means by first CLUSTERS_CNT numbers.
        for (int i = 0; i < CLUSTERS_CNT; ++i)
        {
            means[i] = numbers[i];
            clusters[i].resize(numbers.size());
            clusters_flags[i].resize(numbers.size());
        }


    }
    // Broadcast means and distribute number to each processor.
    MPI_Bcast(means, CLUSTERS_CNT, MPI_FLOAT, ROOT, MPI_COMM_WORLD);
    MPI_Scatter(numbers.data(), 1, MPI_UNSIGNED_CHAR, &number_loc, 1, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);

    // Clustering process
    unsigned char new_cluster;
    while (changed_glob)
    {
        changed_loc = false;

        // Assigning point to the nearest cluster.
        new_cluster = get_best_cluster(number_loc, means);
        if (new_cluster != cluster_idx)
        {
            cluster_affiliation[cluster_idx] = 0;
            clusters_loc[cluster_idx] = 0;

            cluster_idx = new_cluster;
            cluster_affiliation[cluster_idx] = 1;
            clusters_loc[cluster_idx] = number_loc;
            changed_loc = true;
        }


        // Recalculate cluster means and broadcast them.
        MPI_Reduce(cluster_affiliation, clusters_size, CLUSTERS_CNT, MPI_UNSIGNED_CHAR, MPI_SUM, ROOT, MPI_COMM_WORLD);
        MPI_Reduce(clusters_loc, clusters_sum, CLUSTERS_CNT, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);
        if (rank == ROOT)
        {
            for (int i = 0; i < CLUSTERS_CNT; ++i)
            {
                // Zero division is done on a purpose. It marks a cluster as empty.
                means[i] = ((float) clusters_sum[i]) / clusters_size[i];
            }
        }
        MPI_Bcast(means, CLUSTERS_CNT, MPI_FLOAT, ROOT, MPI_COMM_WORLD);
        // Test if the convergence has been met.
        MPI_Allreduce(&changed_loc, &changed_glob, 1, MPI_CXX_BOOL, MPI_LOR, MPI_COMM_WORLD);

    }

    // Collecting results.
    for (int i = 0; i < CLUSTERS_CNT; ++i)
    {
        MPI_Gather(&clusters_loc[i], 1, MPI_UNSIGNED_CHAR, clusters[i].data(), 1, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);
        MPI_Gather(&cluster_affiliation[i], 1, MPI_UNSIGNED_CHAR, clusters_flags[i].data(), 1, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);
    }

    if (rank == ROOT)
    {
        print_results(means, clusters, clusters_flags);
    }

    MPI_Finalize();

    return 0;
}
