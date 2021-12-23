#pragma once

#include <vector>

#include <tbb/parallel_for.h>
#include <tbb/global_control.h>

namespace lu_tbb {

void decompose(const std::vector<std::vector<double>>& matrix,
               std::vector<std::vector<double>>& l,
               std::vector<std::vector<double>>& u,
               int nthread) {

    tbb::global_control global_limit(tbb::global_control::max_allowed_parallelism, nthread);

    int n = matrix.size();

    for (int i = 0; i < n; i++) {

        tbb::parallel_for( tbb::blocked_range<int>(0, n),
                               [&](tbb::blocked_range<int> r) {
                for (int j=r.begin(); j<r.end(); ++j) {
                    if (j < i) {
                        l[j][i] = 0;
                    } else {
                        l[j][i] = matrix[j][i];
                        for (int k = 0; k < i; k++) {
                            l[j][i] = l[j][i] - l[j][k] * u[k][i];
                        }
                    }
                }
        });

        tbb::parallel_for( tbb::blocked_range<int>(0, n),
                               [&](tbb::blocked_range<int> r) {
                for (int j=r.begin(); j<r.end(); ++j) {
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
        });
    }
}

} // namespace lu_tbb
