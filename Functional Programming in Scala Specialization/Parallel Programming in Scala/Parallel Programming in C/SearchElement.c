#include <stdio.h>
#include <malloc.h>
#include "mpi.h"
#include <stdlib.h>

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

int* GetVal(int* arr , int* vec , int col , int row , int key)
{
    int iterate = 0;
    while(iterate<(col*row))
    {
        int z = 0;
        for(;z<col; z++)
        {
            int ch = 0;
            int j = 0;
            for( j = 0 ; j < row; j++)
            {
                if(arr[iterate]==key)
                {
                    ch =1;
                    vec[z] = j;
                }
                printf(" %d ", arr[iterate]);
                iterate++;
            }
            printf("\n");
            if(ch == 0)
                vec[z] = -1;
        }
    }

    return vec;
}


void print(int* arr , int col , int row)
{
    int i, iterate = 0;
    for(iterate = 0 ; iterate < col*row ;)
    {
        for(i = 0 ; i < col ; i++)
        {
            int j;
            printf("My rank = %d ,my results: \n" , i);
            for(j = 0; j < row ; j++) {
                printf("%d ", arr[iterate]);
                iterate++;
            }

            printf("\n");
        }
    }
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
    int key;

    if(myrank == 0)
    {

        printf("Enter the row number and col number:\n");
        scanf("%d %d",&col , &row);


        allarr = malloc(sizeof(int) * row *col);
        vec = malloc( sizeof(int)* row );
        int min = 1 ,max =12;
        int i;
        for(i = 0 ; i < row*col ; i++)
            allarr[i] = min + (max-min)*(double)rand()/(double)RAND_MAX +0.5;

        portion = col/(ProcessNumber);

        gatheIn = malloc(sizeof(int)*ProcessNumber*portion);

        printf("Enter the key:\n");
        scanf("%d",&key);
    }

    MPI_Bcast(&portion , 1 , MPI_INT, 0 , MPI_COMM_WORLD);
    MPI_Bcast(&key , 1 , MPI_INT, 0 , MPI_COMM_WORLD);
    MPI_Bcast(&row , 1 , MPI_INT, 0 , MPI_COMM_WORLD);

    vec = malloc(sizeof(int) * portion);
    receive = malloc(sizeof(int)*portion*row);

    MPI_Scatter(allarr , portion*row , MPI_INT, receive , portion*row , MPI_INT , 0 , MPI_COMM_WORLD );

    vec = GetVal(receive , vec , portion , row , key);


    MPI_Gather(vec , portion , MPI_INT , gatheIn , portion , MPI_INT , 0 , MPI_COMM_WORLD );

    if(myrank == 0) {
        print(gatheIn , ProcessNumber , portion);
    }

    MPI_Finalize();

    return 0;
}