#include <iostream>
#include <vector>
#include <string>

#include "profile.h"
#include "reader.h"
#include "matrix_operations.h"

#include "sequential.h"
#include "lu_openmp.h"


int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Expected filename and log filename as command line arguments" << std::endl;
        exit(1);
    }


    matrix mat = readMatrix(argv[1]);
    // need to avoid dividing by zero
    // pivotize(mat);
    matrix l(mat.size(), std::vector<double> (mat[0].size()));
    matrix u(mat.size(), std::vector<double> (mat[0].size()));


    std::string log_filename = argv[2];
    log_filename += "_";
    log_filename += std::to_string(mat.size());

    // sequential
    {
        LOG_DURATION(log_filename + "_sequential.txt");
        sequential::decompose(mat, l, u);

    }

    // openmp
    {
        LOG_DURATION(log_filename + "_openmp.txt");
        lu_openmp::decompose(mat, l, u);
    }
}
