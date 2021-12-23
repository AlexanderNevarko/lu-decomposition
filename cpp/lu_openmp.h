#pragma once

#include <vector>
#include <omp.h>

namespace lu_openmp {

void decompose(const std::vector<std::vector<double>>& matrix,
               std::vector<std::vector<double>>& l,
               std::vector<std::vector<double>>& u,
               int nthread) {

    int n = matrix.size();

    omp_set_num_threads(nthread);

    #pragma omp parallel shared(matrix,l,u)
    for (int i = 0; i < n; i++) {

        #pragma omp for
        for (int j = 0; j < n; j++) {
            if (j < i) {
                l[j][i] = 0;
            } else {
                l[j][i] = matrix[j][i];
                for (int k = 0; k < i; k++) {
                    l[j][i] = l[j][i] - l[j][k] * u[k][i];
                }
            }
        }

        #pragma omp for
        for (int j = 0; j < n; j++) {
            if (j < i) {
                u[i][j] = 0;
            } else if (j == i) {
                u[i][j] = 1;
            } else {
                u[i][j] = matrix[i][j] / l[i][i];
                for (int k = 0; k < i; k++) {
                    u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i]);
                }
            }
        }
    }
}


} // namespace lu_openmp
