#pragma once

#include <vector>

using matrix = std::vector<std::vector<double>>;

void printMatrix(const matrix& mat) {
    for (const auto& row : mat) {
        for (const double val : row) {
            std::cout << val << "\t";
        }
        std::cout << std::endl;
    }
}

void pivotize(matrix& mat) {
    matrix pivot(mat.size(), std::vector<double> (mat[0].size()));
    matrix res(mat.size(), std::vector<double> (mat[0].size(), 0));

    for (int i = 0; i != mat.size(); ++i) {
        for (int j = 0; j != mat[0].size(); ++j) {
            if (i == j) {
                pivot[i][j] = 1;
            } else {
                pivot[i][j] = 0;
            }
        }
    }

    double max_val;
    int row;
    for (int j = 0; j != mat.size(); ++j) {
        max_val = mat[j][j];
        row = j;

        for (int i = j; i != mat[0].size(); ++i) {
            if (mat[i][j] > max_val) {
                max_val = mat[i][j];
                row = i;
            }

        }

        if (j != row) {
            std::vector<double> tmp = pivot[j];
            pivot[j] = pivot[row];
            pivot[row] = tmp;
        }
    }

    for (int i = 0; i != mat.size(); ++i) {
        for (int j = 0; j != mat[0].size(); ++j) {
            for (int k = 0; k != mat.size(); ++k) {
                res[i][j] += pivot[i][k] * mat[k][j];

            }
        }
    }
    mat = res;
}
