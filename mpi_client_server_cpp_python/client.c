#include "mpi.h"
#include "stdio.h"
#include "string.h"

#define MAX_DATA 50

#define CODE_DISCONNECT_SERVER 0
#define CODE_DISCONNECT_CLIENT 1
#define CODE_SERVER_RECEIVES_CLIENTS_DATA 2

int main(int argc, char **argv) {

    MPI_Comm server;
    char portNameOmpiServer[MPI_MAX_PORT_NAME];

    int numberDataToSend = 3;

    int buffer=9;

    MPI_Init(&argc, &argv);
    // strcpy(portNameOmpiServer, argv[1] ); /* assume server's name is cmd-line arg */

    fprintf(stderr, "Check on server...\n");
    MPI_Lookup_name("compute", MPI_INFO_NULL, portNameOmpiServer);
    MPI_Comm_connect(portNameOmpiServer, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &server);

    //MPI_Bcast(&buffer, 1, MPI_INT, 0, server);
    char c='6';

    MPI_Send(&c, 1, MPI_CHAR, 0, 0, server);

    MPI_Comm_disconnect(&server);

    MPI_Finalize();

    return 0;
}