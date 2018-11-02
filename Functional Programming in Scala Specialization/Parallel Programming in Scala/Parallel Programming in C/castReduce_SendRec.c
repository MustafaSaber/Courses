#include <stdio.h>
#include <malloc.h>
#include "mpi.h"


int GetVal(int* arr , int* vec , int col , int row)
{
    int value = -1;
    int iterate = 0;
    while(iterate<(col*row))
    {
        int z = 0;
        for(;z<col; z++)
        {
            int j = 0;
            int currVal =0;
            for( j = 0 ; j < row; j++)
            {
                currVal+=arr[iterate]*vec[j];
                iterate++;
            }
            if(value < currVal)
                value = currVal;
        }
    }
    return value;
}


int main(int argc , char* argv[])
{
    int myrank;
    int ProcessNumber;
    int source;
    int dest;
    int tag = 0;
    MPI_Status status;
    MPI_Init(&argc ,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD , &myrank);
    MPI_Comm_size(MPI_COMM_WORLD , &ProcessNumber);

    int *vec;
    int row , col;
    int res=-1 , tres=-1;

    if(myrank == 0)
    {
        printf("Enter the row number and col number:\n");
        scanf("%d %d",&col , &row);

        int *allarr;

        printf("Enter the matrix:\n");

        allarr = malloc(sizeof(int) * row *col);
        vec = malloc( sizeof(int)* row );
        int i;
        for(i = 0 ; i < row*col ; i++)
            scanf("%d", &allarr[i]);

        int portion = col/(ProcessNumber-1);


        for( dest = 1; dest< ProcessNumber ; dest++)
        {
            MPI_Send( &row, 1,
                      MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send( &portion, 1,
                      MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send( allarr + ((dest-1) * portion * row), portion * row,
                      MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
    }

    MPI_Bcast(&row , 1 , MPI_INT, 0 , MPI_COMM_WORLD);

    vec = malloc(sizeof(int) * row);

    if (myrank == 0)
    {
        printf("Enter the vector:\n");
        int i;
        for(i = 0 ; i < row; i++)
            scanf("%d",&vec[i]);
    }




    MPI_Bcast(vec , row, MPI_INT, 0 , MPI_COMM_WORLD);


    int minValue = -1;

    if(myrank!=0)
    {
        int portion ;
        MPI_Recv(&row, 1, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        MPI_Recv(&portion, 1, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        
        int* myPortion = (int*)malloc ( row * portion * sizeof(int));

        MPI_Recv(myPortion, (portion*row), MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        int i;
        int iterate = 0;
        minValue = GetVal(myPortion , vec , portion , row);
        

    }

    MPI_Reduce(&minValue , &tres , 1 , MPI_INT , MPI_MAX , 0 , MPI_COMM_WORLD);

    if(myrank == 0) printf("Max = %d \n",tres);

    MPI_Finalize();

    return 0;
}