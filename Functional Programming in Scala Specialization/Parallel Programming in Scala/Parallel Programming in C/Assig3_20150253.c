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
    double res = 1.0 , tres =  1.0;
    MPI_Status status;	/* return status for 	*/
    /* recieve		*/

    /* Start up MPI */
    MPI_Init( &argc , &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int num;

    if( my_rank == 0)
    {
        scanf("%d",&num);
    }
    else
    {

    }

    
    MPI_Bcast(&num , 1 , MPI_INT, 0 , MPI_COMM_WORLD);


    int lower, upper;

    if(my_rank == 0) lower = 1;
    else lower = my_rank * ( num / p ) + 1;
    if(my_rank == (p - 1)) upper = num;
    else upper = (my_rank+1) * (num/p);
    int  i;
    
    for(i = lower ; i <= upper; i++)  res *= (double)i; 

    printf("%lf\n", res );
    MPI_Reduce(&res , &tres , 1 , MPI_DOUBLE , MPI_PROD , 0 , MPI_COMM_WORLD);

    if(my_rank == 0) printf("facctorial = %lf \n",tres);

    /* shutdown MPI */
    MPI_Finalize();
    return 0;
}
