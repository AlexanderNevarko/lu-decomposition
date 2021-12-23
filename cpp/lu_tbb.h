#pragma once

#include <vector>

#include <tbb/parallel_for.h>

namespace lu_tbb {

void decompose(const std::vector<std::vector<double>>& matrix,
               std::vector<std::vector<double>>& l,
               std::vector<std::vector<double>>& u) {

    int n = matrix.size();

	for(int j = 0; j < n; j++) {

        l[j][j] = 1;
        tbb::parallel_for( tbb::blocked_range<int>(0, j),
                               [&](tbb::blocked_range<int> r) {
                for (int i=r.begin(); i<r.end(); ++i) {
                    int sum = 0;
                    for (int k = 0; k < i; ++k) {
                        sum += u[k][j] * l[i][k];
                    }
                    u[i][j] = matrix[i][j] - sum;
                }
            });

        tbb::parallel_for( tbb::blocked_range<int>(j, n),
                               [&](tbb::blocked_range<int> r) {
                for (int i=r.begin(); i<r.end(); ++i) {
                    int sum = 0;
                    for (int k = 0; k < j; ++k) {
                        sum += u[k][j] * l[i][k];
                    }
                    if (u[j][i] == 0) u[j][i] = 0.0001;
                    l[i][j] = (matrix[i][j] - sum) / u[j][j];
                }
            });
    }
}


} // namespace lu_tbb
