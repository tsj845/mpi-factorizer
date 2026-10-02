#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <mpi.h>

const int MAX_STRING = 100;

typedef unsigned long long u64;

int main(int argc, char** argv) {
    int comm_sz;    // number of processors
    int my_rank;    // process rank

    if (argc < 2) {
        printf("failed because no input\n");
    }

    u64 num = strtoull(argv[1], NULL, 10);

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    // zero is a special case that needs special handling
    if (my_rank != 0 && num == 0) return 0;

    u64 x = 1;
    int shifts = 0;
    while (num&~1) {
        num >>= 1;
        x <<= 1;
        shifts ++;
    }
    int factorsize = num / 6 + 1;
    int perproc = factorsize/(comm_sz-1);
    // factors[0] is how many factors were found
    u64 *factors = malloc(sizeof(u64) * (factorsize+1));

    if (my_rank == 0) {
        printf("%d\n", shifts);
        printf("%llu ", num);
        for (int i = 0; i < shifts; i ++) {
            printf("%llu ", 1ull << i);
        }
        for (int q = 1; q < comm_sz; q ++) {
            MPI_Recv(factors, factorsize+1, MPI_UNSIGNED_LONG_LONG, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < (int)factors[0]; i ++) {
                printf("%llu ", x*factors[i+1]);
            }
        }
        printf("\n");
    } else {
        u64 found = 0;
        u64 test = 3+perproc*(my_rank-1)*2;
        for (int fac = 0; fac <= perproc; fac ++) {
            if (num%test == 0) {
                factors[found+1] = num/test;
                factors[found+2] = test;
                found += 2;
            }
            test += 2;
        }
        factors[0] = found;
        MPI_Send(factors, factorsize+1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
