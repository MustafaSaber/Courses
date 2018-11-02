#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc , char * argv[])
{
    int my_rank;		/* rank of process	*/
    int p;			/* number of process	*/
    int source;		/* rank of sender	*/
    int dest;		/* rank of reciever	*/
    int tag = 0;		/* tag for messages	*/
    char message[100];	/* storage for message	*/
    MPI_Status status;	/* return status for 	*/
    /* recieve		*/

    /* Start up MPI */
    MPI_Init( &argc , &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if( my_rank != 0)
    {
        int portion;
        MPI_Recv(&portion, 1, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        int* myPortion = (int*)malloc (portion * sizeof(int));
        MPI_Recv(myPortion, portion, MPI_INT, 0,
                 tag, MPI_COMM_WORLD, &status );
        int i;
        int mymax , myind;
        mymax = myind = -10000000;
        for( i = 0; i< portion ; i++)
            if(mymax < myPortion[i]) { mymax = myPortion[i]; myind = i;}

        printf("My rank = %d , value = %d , ind = %d. \n", my_rank ,mymax , myind);

        MPI_Send( &mymax, 1,
                  MPI_INT, 0, tag, MPI_COMM_WORLD);
        MPI_Send( &myind, 1,
                  MPI_INT, 0, tag, MPI_COMM_WORLD);


    }
    else
    {
        int size;
        printf("Enter the array size: ");
        scanf("%d", &size);
        int* arr = (int*) malloc (size * sizeof(int));

        int ch = 0;

        if( (size % (p-1)) > 0) ch = 1;

        printf("Enter the array elements: ");

        int i;
        for(i = 0 ; i < size ; i++)
        {
            scanf("%d", &arr[i]);
        }
        int portion = size/(p-1);
        for( dest = 1; dest< p ; dest++)
        {
            MPI_Send( &portion, 1,
                      MPI_INT, dest, tag, MPI_COMM_WORLD);

            MPI_Send( arr + ((dest-1) * portion), portion,
                      MPI_INT, dest, tag, MPI_COMM_WORLD);
        }

        int maxVal, maxInd;
        maxVal = maxInd = -10000000;

        for( dest = 1; dest< p ; dest++)
        {
            int val , ind;
            MPI_Recv(&val, 1, MPI_INT, dest,
                     tag, MPI_COMM_WORLD, &status );

            MPI_Recv(&ind, 1, MPI_INT, dest,
                     tag, MPI_COMM_WORLD, &status );

            if(val > maxVal) { maxVal = val; maxInd = ind; }
        }

        if(ch == 1)
        {
            int* myArr = (int*)malloc ((size - (size/(p-1)*(p-1)))* sizeof(int));
            myArr = arr+((p-1) * portion);
            int iterate;
            for(iterate = 0; iterate < (size - (size/(p-1)*(p-1) )) ; iterate++)
            {
                if(myArr[iterate] > maxVal) { maxVal = myArr[iterate]; maxInd = iterate;}
            }
        }


        printf("value = %d , ind = %d. \n",maxVal , maxInd);
    }

    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
