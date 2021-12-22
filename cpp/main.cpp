#include <iostream>
#include <vector>
#include <string>

#include "profile.h"
#include "reader.h"
#include "sequential.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Expected filename and log filename as command line arguments" << std::endl;
        exit(1);
    }


    const std::vector<std::vector<double>>& matrix = readMatrix(argv[1]);
    std::vector<std::vector<double>> l(matrix.size(),
                                       std::vector<double> (matrix[0].size()));
    std::vector<std::vector<double>> u(matrix.size(),
                                       std::vector<double> (matrix[0].size()));
    // sequential
    {
        std::string log_filename = argv[2];
        log_filename += "_";
        log_filename += std::to_string(matrix.size());
        log_filename += "_sequential.txt";

        LOG_DURATION(log_filename);

        sequential::decompose(matrix, l, u);

    }
}
