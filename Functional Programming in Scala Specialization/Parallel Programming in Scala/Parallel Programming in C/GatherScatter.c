#include <stdio.h>
#include <malloc.h>
#include "mpi.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

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
    int portion;
    int *allarr , *receive , *gatheIn;

    if(myrank == 0)
    {

        gatheIn = malloc(sizeof(int)*ProcessNumber);

        printf("Enter the row number and col number:\n");
        scanf("%d %d",&col , &row);


        printf("Enter the matrix:\n");

        allarr = malloc(sizeof(int) * row *col);
        vec = malloc( sizeof(int)* row );
        int i;
        for(i = 0 ; i < row*col ; i++)
            scanf("%d", &allarr[i]);

        portion = col/(ProcessNumber);

        for(i = 1; i < ProcessNumber ; i++)
            MPI_Send( &portion, 1,
                      MPI_INT, i, tag, MPI_COMM_WORLD);

    }
    else
        MPI_Recv(&portion, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

    //can be used instead of send and receive for portion.
     // MPI_Bcast(&portion , 1 , MPI_INT, 0 , MPI_COMM_WORLD);
    MPI_Bcast(&row , 1 , MPI_INT, 0 , MPI_COMM_WORLD);

    vec = malloc(sizeof(int) * row);
    receive = malloc(sizeof(int)*portion*row);

    if (myrank == 0)
    {
        printf("Enter the vector:\n");
        int i;
        for(i = 0 ; i < row; i++)
            scanf("%d",&vec[i]);
    }



    MPI_Bcast(vec , row, MPI_INT, 0 , MPI_COMM_WORLD);

    MPI_Scatter(allarr , portion*row , MPI_INT, receive , portion*row , MPI_INT , 0 , MPI_COMM_WORLD );

    /*printf("My portion = %d , my rank = %d", portion , myrank);
    printf("What master received: \n");
    int i;
    for(i = 0; i < portion*row ; i++)
        printf("%d " ,receive[i]);
    printf("\n");*/

    int minValue = -1;
    minValue = GetVal(receive , vec , portion , row);


   // MPI_Reduce(&minValue , &tres , 1 , MPI_INT , MPI_MAX , 0 , MPI_COMM_WORLD);

    MPI_Gather(&minValue , 1 , MPI_INT , gatheIn , 1 , MPI_INT , 0 , MPI_COMM_WORLD );

    if(myrank == 0) {
        tres = gatheIn[0];
        int i;
        for(i = 1 ; i < ProcessNumber ; i++)
            tres = MAX(tres , gatheIn[i]);
        printf("Max = %d \n",tres);
    }

    MPI_Finalize();

    return 0;
}