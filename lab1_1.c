#include <stdio.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
    int n = 1;
    int msg_tg;
    MPI_Init(&argc, &argv);
    int commsize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int tags[16] = { 0 };
    for (int i = 0; i <= 2;i++)
    {
        char msg[n];
        for(int j = 0; j < n; j++)
        {
            msg[j] = rand() % 128;
        }
        msg_tg = rank;
        MPI_Status stat;
        double start_t = MPI_Wtime ();
        for (;tags[rank] != 1;)
        {
        if (rank == commsize-1)
        {
            MPI_Sendrecv(&msg, n, MPI_CHAR, 0, msg_tg, &msg, n, MPI_CHAR, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            tags[stat.MPI_TAG] = 1;
            msg_tg = stat.MPI_TAG;
        }
        else
        {
            MPI_Sendrecv(&msg, n, MPI_CHAR, rank+1, msg_tg, &msg, n, MPI_CHAR, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            tags[stat.MPI_TAG] = 1;
            msg_tg = stat.MPI_TAG;
        }
        }
        double end_t = MPI_Wtime();
        if (rank == 0)
            printf ("n = %d , start time = %f , end time = %f , delta time = %f \n", n, start_t, end_t, end_t - start_t);
        n = n * 1024;
        for(int i = 0; i < 16; i++)
        {
            tags[i] = 0;
        }
    }
    MPI_Finalize();
    return 0;
}