#pragma once

#include <vector>

namespace sequential {

void decompose(const std::vector<std::vector<double>>& matrix,
               std::vector<std::vector<double>>& l,
               std::vector<std::vector<double>>& u) {

	int i = 0, j = 0, k = 0;
	for(j = 0; j < matrix.size() - 1; j++) {
        for(i = j + 1; i < matrix.size(); i++) {

            double factor = matrix[i][j] / matrix[j][j];

            for(k = 0; k < matrix.size(); k++) {
            	u[i][k]= matrix[i][k] - (matrix[j][k] * factor);
            }
            l[i][j] = factor;
        }
    }
}

} // namespace sequential
