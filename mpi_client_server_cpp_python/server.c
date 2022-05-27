#include "mpi.h"
#include "stdio.h"
#include "string.h"

#define MAX_DATA 50

#define CODE_DISCONNECT_SERVER 0
#define CODE_DISCONNECT_CLIENT 1
#define CODE_SERVER_RECEIVES_CLIENTS_DATA 2

int main(int argc, char **argv) {

    MPI_Comm client;
    MPI_Status status;

    char portNameOmpiServer[MPI_MAX_PORT_NAME];
    double buffer[MAX_DATA];
    int size, again;
    double *pointerCurrentElement;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 1) {
        perror("There must be only one server.");
    }

    MPI_Open_port(MPI_INFO_NULL, portNameOmpiServer);
    printf("Server available at %s\n", portNameOmpiServer);

    MPI_Info info;
    MPI_Info_create(&info);
    MPI_Info_set(info, "ompi_global_scope", "true");

    MPI_Publish_name("ocean", info, portNameOmpiServer);

    int y=9;

    MPI_Recv(y, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, client, &status);

                    MPI_Unpublish_name("ocean", MPI_INFO_NULL, portNameOmpiServer);
                    MPI_Comm_free(&client);
                    MPI_Close_port(portNameOmpiServer);
                    MPI_Finalize();
                    return 0;

//    while (1) {
//
//        MPI_Comm_accept(portNameOmpiServer, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &client);
//        again = 1;
//
//        while (again) {
//
//            MPI_Recv(buffer, MAX_DATA, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, client, &status);
//
//            switch (status.MPI_TAG) {
//
//                case CODE_DISCONNECT_SERVER:
//                    MPI_Unpublish_name("ocean", MPI_INFO_NULL, portNameOmpiServer);
//                    MPI_Comm_free(&client);
//                    MPI_Close_port(portNameOmpiServer);
//                    MPI_Finalize();
//                    return 0;
//
//                case CODE_DISCONNECT_CLIENT:
//                    MPI_Comm_disconnect(&client);
//                    again = 0;
//                    break;
//
//                case CODE_SERVER_RECEIVES_CLIENTS_DATA:
//                    // do something
//                    pointerCurrentElement = (double *) buffer;
//                    fprintf(stderr, "Server got a client's data: %f\n", *pointerCurrentElement);
//                    break;
//
//                default:
//                    /* Unexpected message type */
//                    // radical original version aborted everything...
//                    ;//MPI_Abort( MPI_COMM_WORLD, 1 );
//            }
//        }
//    }
}