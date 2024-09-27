#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
    int n = 1024;
    MPI_Init(&argc, &argv);
    int commsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char ib[n*commsize];
    char ob[n];
    double start_t = MPI_Wtime ();
    for (int f = rank + 1; f > 0; f--)
    {
        MPI_Isend (&ob, n, MPI_CHAR, f, 0, MPI_COMM_WORLD, 0);
        MPI_Irecv (&ib[n*f], n, MPI_CHAR, f, 0, MPI_COMM_WORLD, 0);
    }
    for (int g = rank + 1; g < commsize; g++)
    {
        MPI_Isend (&ob, n, MPI_CHAR, g, 0, MPI_COMM_WORLD, 0);
        MPI_Irecv (&ib[n*g], n, MPI_CHAR, g, 0, MPI_COMM_WORLD, 0);
    }
    double end_t = MPI_Wtime ();
    printf ("start time = %f, end time = %f, delta time = %f \n", start_t, end_t, end_t - start_t);
    MPI_Finalize ();
    return 0;
}