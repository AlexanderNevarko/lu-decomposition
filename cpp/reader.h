#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::vector<double> splitIntoDoubles(const std::string& s) {
    std::vector<double> matrix_row;

    std::istringstream stream(s);

    double d;
    while (stream >> d) {
        matrix_row.push_back(d);
    }

    return matrix_row;
}

std::vector<std::vector<double>> readMatrix(const std::string& filename) {
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "can't open file" << std::endl;
        exit(1);
    }

    std::vector<std::vector<double>> matrix;

    std::string line;
    while (std::getline(infile, line)) {
        matrix.emplace_back(splitIntoDoubles(line));
    }
    infile.close();

    return matrix;
}
