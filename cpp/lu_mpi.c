#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdbool.h>
#include <string.h>

#define matrix(i,j) matrix[i * SIZE + j]
#define U(i,j) U[SIZE * i + j]
#define L(i,j) L[SIZE * i +j]

#define U2(i,j) U2[SIZE * i + j]
#define L2(i,j) L2[SIZE * i +j]


#define MASTER 0

// SIZE * SIZE
double *matrix;
// // SIZE * SIZE
double *L;
// // SIZE * SIZE
double *U;

int ntasks = 1;
// MPI_Status status;
int taskid;

int SIZE = 10;


void printmat(double * arr_2d, int rownum ,int colnum){
    for(int i = 0; i < rownum; i++){
        for(int j = 0; j < colnum; j++){
            printf("%lf ",arr_2d[i * colnum + j]);
        }
        printf("\n");
    }
}

void get_lu(){

    int rows_per_thread = SIZE / ntasks;

    double* matrix2 = (double*)malloc(SIZE * SIZE * sizeof(double));

    double* L2 = (double*)calloc(SIZE*SIZE, sizeof(double));
    double* U2 = (double*)calloc(SIZE*SIZE, sizeof(double));

    double* Ut = (double*)calloc(SIZE*SIZE, sizeof(double));

    double* L3 = (double*)calloc(SIZE*rows_per_thread, sizeof(double));
    double* U3 = (double*)calloc(SIZE*rows_per_thread, sizeof(double));

    // if (taskid == 0) {
    //     matrix2 = matrix;
    // }

    MPI_Bcast(matrix, SIZE*SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int n = SIZE;
    for (int i = 0; i < n; i++) {
        for (int j = taskid*rows_per_thread; j < (taskid + 1)*rows_per_thread; j++) {
            if (j < i) {
                L2(j, i) = 0;
            } else {
                L2(j, i) = matrix(j, i);
                for (int k = 0; k < i; k++) {
                    L2(j, i) = L2(j, i) - L2(j, k) * U2(k, i);
                }
            }
        }
        MPI_Gather(L3, SIZE*rows_per_thread, MPI_DOUBLE, L2,SIZE *rows_per_thread, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        MPI_Bcast(L2, SIZE*SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        for (int j = taskid*rows_per_thread; j < (taskid + 1)*rows_per_thread; j++) {
            if (j < i) {
                U2(i, j) = 0;
            } else if (j == i) {
                U2(i, j) = 1;
            } else {
                if (L2(i, j) == 0) L2(i, j) = 0.00001;
                U2(i, j) = matrix(i, j) / L2(i, j);
                for (int k = 0; k < i; k++) {
                    U2(i, j) = U2(i, j) - ((L2(i, k) * U2(k, j)) / L2(i, i));
                }
            }
        }
        MPI_Gather(U3, SIZE*rows_per_thread, MPI_DOUBLE, Ut, SIZE*rows_per_thread, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(U2, SIZE*SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    printf("fin");
}

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&ntasks);

    matrix = (double*)malloc(SIZE * SIZE * sizeof(double));
    // matrix[0] = 2;
    // matrix[1] = 7;
    // matrix[2] = 1;
    // matrix[3] = 3;
    // matrix[4] = -2;
    // matrix[5] = 0;
    // matrix[6] = 1;
    // matrix[7] = 5;
    // matrix[8] = 3;
    // if (taskid == MASTER)
    // printmat(matrix, SIZE, SIZE);

            // L and U should be initialized as zero
    L = (double*)calloc(SIZE*SIZE, sizeof(double));
    U = (double*)calloc(SIZE*SIZE, sizeof(double));

    double val;
    int val_num = 0;
    int i;
    if (taskid == MASTER) {

        FILE *inp;
        inp = fopen(argv[1], "r");
        i = fscanf(inp, "%lf", &val);
        while (i != EOF) {
            matrix[val_num] = val;
            val_num += 1;
            i = fscanf(inp, "%lf", &val);
        }

        fclose(inp);
    }

    double run_time;
    if (taskid == MASTER) {
        run_time = MPI_Wtime();
    }

    if (taskid == MASTER) {
        get_lu();
    }

	if (taskid == MASTER) {
		run_time = MPI_Wtime() - run_time;
        printf("time: %lf\n", run_time);
	}

   MPI_Finalize();
}
