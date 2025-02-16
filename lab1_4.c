#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
    int n = 1024;
    MPI_Init(&argc, &argv);
    int commsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char ib[commsize][n];
    char ob[n];
    MPI_Request reqv[commsize * 2 - 1];
    MPI_Status stats[commsize * 2 - 1];
    for (int j = 0; j < n; j++)
    {
        ob[n] = rand() % 128;
    }
    double start_t = MPI_Wtime ();
    for (int k = 0; k < commsize; k++)
    {
        if (k == rank)
        {
            MPI_Isend (&ob, n, MPI_CHAR, k, 0, MPI_COMM_WORLD, &reqv[k]);
            MPI_Irecv (&ib[k], n, MPI_CHAR, k, 0, MPI_COMM_WORLD, &reqv[k+commsize]);
        }
        MPI_Isend (&ob, n, MPI_CHAR, k, 0, MPI_COMM_WORLD, &reqv[k]);
        MPI_Irecv (&ib[k], n, MPI_CHAR, k, 0, MPI_COMM_WORLD, &reqv[k+commsize]);
    }
    MPI_Waitall (commsize * 2 - 1, reqv, stats);
/*     for (int f = rank - 1; f >= 0; f--)
    {
        MPI_Isend (&ob, n, MPI_CHAR, f, 0, MPI_COMM_WORLD, &reqv[f]);
        MPI_Irecv (&ib[f], n, MPI_CHAR, f, 0, MPI_COMM_WORLD, &reqv[f+commsize]);
    }
    MPI_Waitall(commsize * 2 - 1, reqv, stats);
    for (int g = rank + 1; g < commsize; g++)
    {
        MPI_Isend (&ob, n, MPI_CHAR, g, 0, MPI_COMM_WORLD, &reqv[g-1]);
        MPI_Irecv (&ib[g], n, MPI_CHAR, g, 0, MPI_COMM_WORLD, &reqv[g+commsize-1]);
    }
    MPI_Waitall (commsize * 2 - 1, reqv, stats); */
    double end_t = MPI_Wtime ();
    if (rank == 0)
        printf ("start time = %f, end time = %f, delta time = %f \n", start_t, end_t, end_t - start_t);
    MPI_Finalize ();
    return 0;
}