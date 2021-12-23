#pragma once

#include <vector>
#include <omp.h>

namespace lu_openmp {

void decompose(const std::vector<std::vector<double>>& matrix,
               std::vector<std::vector<double>>& l,
               std::vector<std::vector<double>>& u) {

    int n = matrix.size();

#pragma omp parallel shared(matrix,l,u)
	for(int j = 0; j < n; j++) {

        l[j][j] = 1;
#pragma omp for
        for (int i = 0; i <= j; ++i) {
            int sum = 0;
            for (int k = 0; k < i; ++k) {
                sum += u[k][j] * l[i][k];
            }
            u[i][j] = matrix[i][j] - sum;

        }
#pragma omp for
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


} // namespace lu_ penmp
