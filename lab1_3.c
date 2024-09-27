#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
    MPI_Init (&argc, &argv);
    int rank, commsize;
    MPI_Comm_size (MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    int n = 1024;
    for(int i = 0; i <= 1; i++)
    {
        char rbuf[n];
        for(int b = 0; b < n; b++)
        {
            rbuf[b] = rand() % 128;
        }
        char sbuf[n*commsize];
        double start_t = MPI_Wtime ();
        if(rank == 0)
        for(int i = commsize; i > 0; i--)
        {
            MPI_Recv (&sbuf[n*i], n, MPI_CHAR, i, 0, MPI_COMM_WORLD, 0);
        }
        else
        {
            MPI_Send (&rbuf, n, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
        double end_t = MPI_Wtime ();
        if (rank == 0)
            printf ("n = %d, start time = %f, end time = %f, delta time = %f \n", n, start_t, end_t, end_t - start_t);
        n*= 1024;
    }
    MPI_Finalize();
    return 0;
}