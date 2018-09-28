#include <stdio.h>
#include <malloc.h>
#include "mpi.h"
#include <stdlib.h>

int* Scale(int* arr, int col , int row , int key)
{
    int iterate =0;
    for(iterate = 0 ; iterate < col*row ; iterate++)
        arr[iterate]*=key;
    return arr;
}


void print(int* arr , int col , int row , int scale , int check)
{
    int iterate =0 , i =0;
    for(iterate = 0 ; iterate < col*row ;)
    {
        for(i = 0 ; i < col ; i++)
        {
            int j;
            for(j = 0; j < row ; j++) {
                if(check==0)
                    printf("%d ", arr[iterate]/scale);
                else
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

    int row , col;
    int res=-1 , tres=-1;
    int portion;
    int *allarr , *receive , *gatheIn , *myPortion;
    int scale;

    if(myrank == 0)
    {

        printf("Enter the row number and col number:\n");
        scanf("%d %d",&col , &row);


        allarr = malloc(sizeof(int) * row *col);
        int min = 1 ,max =12;
        int i;
        for(i = 0 ; i < row*col ; i++)
            allarr[i] = min + (max-min)*(double)rand()/(double)RAND_MAX +0.5;

        int iterate = 0;
        for(iterate = 0 ; iterate < col*row ;)
        {
            for(i = 0 ; i < col ; i++)
            {
                int j;
                for(j = 0; j < row ; j++) {
                    printf("%d ", allarr[iterate]);
                    iterate++;
                }

                printf("\n");
            }
        }

        portion = col/(ProcessNumber-1);


        gatheIn = malloc(sizeof(int)*col*row);

        printf("Enter the scale:\n");
        scanf("%d",&scale);

        for( dest = 1; dest< ProcessNumber ; dest++)
        {
            MPI_Send( &row, 1,
                      MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send( &portion, 1,
                      MPI_INT, dest, tag, MPI_COMM_WORLD);
            MPI_Send( allarr + ((dest) * portion * row), portion * row,
                      MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
        ///We consider 0 is with us.

        //Scale(myPortion , portion , row , scale);

    }

    MPI_Bcast(&portion , 1 , MPI_INT, 0 , MPI_COMM_WORLD);
    MPI_Bcast(&scale , 1 , MPI_INT, 0 , MPI_COMM_WORLD);
    MPI_Bcast(&col , 1 , MPI_INT, 0 , MPI_COMM_WORLD);


    if(myrank!=0)
    {
        int portion ;
        MPI_Recv(&row, 1, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        MPI_Recv(&portion, 1, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );

        myPortion = (int*)malloc ( row * portion * sizeof(int));

        MPI_Recv(myPortion, (portion*row), MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );

        myPortion = Scale(myPortion, portion , row , scale);

    }
    if(myrank == 0)
    {
        int i =0;
        myPortion = (int*)malloc ( row * portion * sizeof(int));
        myPortion = allarr;
        Scale(myPortion , portion , row, scale);
    }

    MPI_Gather(myPortion , portion*row , MPI_INT , gatheIn , portion*row , MPI_INT , 0 , MPI_COMM_WORLD );

    if(myrank == 0) {
        printf("\nBefore\n");
        print(gatheIn , col , row , scale, 0);
        printf("\nAfter scale\n");
        print(gatheIn , col , row , scale, 1);
    }

    MPI_Finalize();

    return 0;
}