#pragma once

#include <vector>

namespace sequential {

void decompose(const std::vector<std::vector<double>>& matrix,
               std::vector<std::vector<double>>& l,
               std::vector<std::vector<double>>& u) {

    int n = matrix.size();

	for(int j = 0; j < n; j++) {

        l[j][j] = 1;
        for (int i = 0; i <= j; ++i) {
            int sum = 0;
            for (int k = 0; k < i; ++k) {
                sum += u[k][j] * l[i][k];
            }
            u[i][j] = matrix[i][j] - sum;

        }
        for (int i = j; i < n; ++i) {
            int sum = 0;
            for (int k = 0; k < j; ++k) {
                sum += u[k][j] * l[i][k];
            }
            if (u[j][i] == 0) u[j][i] = 0.0001;
            l[i][j] = (matrix[i][j] - sum) / u[j][j];

        }
    }
}

} // namespace sequential
