    /**
 * @file parsplit.cpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief The implementation of the parallel split using OpenMPI (PRL - project 1)
 * @date 2023-03-30
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <numeric>
#include <mpi.h>


#define ROOT 0

using namespace std;


/**
 * @brief Loads numbers (in the form of bytes) from a file.
 *
 * @param file_name Input file with the numbers.
 * @param numbers_out A pointer to the output array with loaded numbers.
 * @return int The count of loaded numbers.
 */
int load_numbers(const char *file_name, unsigned char *numbers_out[]) {
    ifstream input(file_name, ios::binary);

    if (!input.good()) {
        cerr << "Input file not found." << endl;
    }

    vector<unsigned char> numbers((istreambuf_iterator<char>(input)),
                                  (istreambuf_iterator<char>()));

    unsigned char *new_numbers = new unsigned char[numbers.size()];
    for (int i = 0; i < numbers.size(); ++i) {
        new_numbers[i] = numbers[i];
    }
    *numbers_out = new_numbers;

    return (int) numbers.size();
}

/**
 * @brief Prints array to the stdout.
 *
 * @param arr An array being printed.
 * @param size The array size.
 */
void print_array(const unsigned char arr[], const int size) {
    cout << "[";
    if (size != 0) {
        for (int i = 0; i < size; ++i) {
            printf("%u,", arr[i]);
        }
        cout << "\b";
    }
    cout << "]";
}

/**
 * @brief Prints results of a parallel split.
 *
 * @param lt The array with numbers that are less than eq.
 * @param lt_size The size of the lt array.
 * @param eq The array with equal numbers.
 * @param eq_size The size of the eq array.
 * @param gt The array with numbers that are grater than eq.
 * @param gt_size The size of the gt array.
 */
void print_results(const unsigned char lt[], const int lt_size,
                   const unsigned char eq[], const int eq_size,
                   const unsigned char gt[], const int gt_size) {

    cout << "LT = ";
    print_array(lt, lt_size);
    cout << endl;

    cout << "EQ = ";
    print_array(eq, eq_size);
    cout << endl;

    cout << "GT = ";
    print_array(gt, gt_size);
    cout << endl;
}

/**
 * @brief Performs sequential split.
 *
 * @param numbers An array of numbers being splitted.
 * @param numbers_cnt The size of array numbers.
 * @param midpoint A middle number in the array.
 * @param lt The array for numbers that are less than then midpoint.
 * @param lt_cnt The count of the numbers in the lt.
 * @param eq The array for numbers that are equal to the midpoint.
 * @param eq_cnt The count of the numbers in the eq.
 * @param gt The array for numbers that are grater than the midpoint.
 * @param gt_cnt The count of the numbers int the gt.
 */
void seqsplit(unsigned char numbers[],
              int numbers_cnt,
              unsigned char midpoint,
              unsigned char lt[], int &lt_cnt,
              unsigned char eq[], int &eq_cnt,
              unsigned char gt[], int &gt_cnt) {

    lt_cnt = eq_cnt = gt_cnt = 0;

    for (int i = 0; i < numbers_cnt; ++i) {
        unsigned char n = numbers[i];
        if (n < midpoint) {
            lt[lt_cnt++] = n;
        } else if (n == midpoint) {
            eq[eq_cnt++] = n;
        } else {
            gt[gt_cnt++] = n;
        }
    }
}

/**
 * @brief Performs prescan on a given array.
 *
 * @param arr Input array.
 * @param size Input array size.
 * @return int* Array of prescan results.
 */
int *prescan(int arr[], int size) {
    int *out = new int[size]();
    for (int i = 1; i < size; ++i) {
        out[i] = out[i-1] + arr[i-1];
    }
    return out;
}

// *********************
// *       MAIN        *
// *********************

int main(int argc, char **argv) {

    // Initializing MPI
    int rank;
    int size;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Input variables
    unsigned char *numbers_in = nullptr;
    int numbers_in_size = 0;
    int numbers_per_proc = 0;

    // Output variables
    unsigned char *lt_out = nullptr;
    unsigned char *eq_out = nullptr;
    unsigned char *gt_out = nullptr;
    int lt_out_size = 0;
    int eq_out_size = 0;
    int gt_out_size = 0;

    // Auxiliary variables or MPI_Gatherv
    int *lt_displacement = nullptr;
    int *eq_displacement = nullptr;
    int *gt_displacement = nullptr;
    int *lt_sizes = nullptr;
    int *eq_sizes = nullptr;
    int *gt_sizes = nullptr;

    // Local variables
    unsigned char midpoint = 0;
    unsigned char *numbers_local = nullptr ;
    unsigned char *lt = nullptr;
    unsigned char *eq = nullptr;
    unsigned char *gt = nullptr;
    int lt_size = 0;
    int eq_size = 0;
    int gt_size = 0;

    // Loading numbers
    if (rank == 0) {
        numbers_in_size = load_numbers((argc == 1) ? "numbers" : argv[1], &numbers_in);
        if (numbers_in_size > 0) {
            if (numbers_in_size % 2 == 0) {
                midpoint = numbers_in[(numbers_in_size / 2) - 1];
            } else {
                midpoint = numbers_in[(numbers_in_size / 2)];
            }
            numbers_per_proc = numbers_in_size / size;
        }
    }

    // Sending task data
    MPI_Bcast(&numbers_per_proc, 1, MPI_UINT16_T, ROOT, MPI_COMM_WORLD);
    MPI_Bcast(&midpoint, 1, MPI_UINT16_T, ROOT, MPI_COMM_WORLD);
    numbers_local = new unsigned char[numbers_per_proc];
    MPI_Scatter(numbers_in, numbers_per_proc, MPI_UNSIGNED_CHAR, numbers_local, numbers_per_proc, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);

    // Spliting
    lt = new unsigned char[numbers_per_proc]();
    eq = new unsigned char[numbers_per_proc]();
    gt = new unsigned char[numbers_per_proc]();
    seqsplit(numbers_local, numbers_per_proc, midpoint, lt, lt_size, eq, eq_size, gt, gt_size);

    // Initializing sizes of the output arrays for MPI_Gatherv.
    if (rank == ROOT) {
        lt_sizes = new int[size]();
        eq_sizes = new int[size]();
        gt_sizes = new int[size]();
    }

    // Collecting sizes for the MPI_Gatherv.
    MPI_Gather(&lt_size, 1, MPI_INT, lt_sizes, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    MPI_Gather(&eq_size, 1, MPI_INT, eq_sizes, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    MPI_Gather(&gt_size, 1, MPI_INT, gt_sizes, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    // Calculating displacement for MPI_Gatherv.
    // Preparing output arrays.
    if (rank == ROOT) {
        lt_displacement = prescan(lt_sizes, size);
        eq_displacement = prescan(eq_sizes, size);
        gt_displacement = prescan(gt_sizes, size);
        lt_out_size = accumulate(lt_sizes, lt_sizes + size, lt_out_size);
        eq_out_size = accumulate(eq_sizes, eq_sizes + size, eq_out_size);
        gt_out_size = accumulate(gt_sizes, gt_sizes + size, gt_out_size);
        lt_out = new unsigned char[lt_out_size]();
        eq_out = new unsigned char[eq_out_size]();
        gt_out = new unsigned char[gt_out_size]();
    }

    // Collecting data from processors.
    MPI_Gatherv(lt, lt_size, MPI_UNSIGNED_CHAR, lt_out, lt_sizes, lt_displacement, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);
    MPI_Gatherv(eq, eq_size, MPI_UNSIGNED_CHAR, eq_out, eq_sizes, eq_displacement, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);
    MPI_Gatherv(gt, gt_size, MPI_UNSIGNED_CHAR, gt_out, gt_sizes, gt_displacement, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);

    // Printing results.
    if (rank == ROOT) {
        print_results(lt_out, lt_out_size, eq_out, eq_out_size, gt_out, gt_out_size);
    }

    // Finalizing.
    delete [] lt;
    delete [] eq;
    delete [] gt;
    if (rank == ROOT) {
        delete [] lt_sizes;
        delete [] eq_sizes;
        delete [] gt_sizes;
        delete [] lt_displacement;
        delete [] eq_displacement;
        delete [] gt_displacement;
        delete [] lt_out;
        delete [] eq_out;
        delete [] gt_out;
        delete [] numbers_in;
    }
    MPI_Finalize();

    return 0;
}
