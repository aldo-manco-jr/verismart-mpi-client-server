#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {

    std::cout << "Hello, from Server!" << std::endl;

    MPI_Init(&argc, &argv);

    int initial_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &initial_rank);
    int initial_size;
    MPI_Comm_size(MPI_COMM_WORLD, &initial_size);

    char portNameOmpiServer[MPI_MAX_PORT_NAME];

    MPI_Comm interCommunicator;

    MPI_Status status;

    double buffer = 0;
    if (initial_rank == 0) {

        MPI_Info info;
        MPI_Info_create(&info);
        MPI_Info_set(info, "ompi_global_scope", "true");

        MPI_Open_port(info, portNameOmpiServer);

        MPI_Publish_name("ocean", info, portNameOmpiServer);
        std::cout << "Published port " << portNameOmpiServer << std::endl;

        std::cout << "Accepting comm (Rank: " << initial_rank << ")!" << std::endl;
        MPI_Comm_accept(portNameOmpiServer, info, 0, MPI_COMM_WORLD, &interCommunicator);
        std::cout << "Accepted comm (Rank: " << initial_rank << ")!" << std::endl;
        MPI_Recv(&buffer, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, interCommunicator, &status);
        std::cout << buffer << std::endl;

        std::cout << "Unpublishing port!" << std::endl;
        MPI_Unpublish_name("ocean", info, portNameOmpiServer);
    } else {
        std::cout << "Accepting comm (Rank: " << initial_rank << ")!" << std::endl;
        MPI_Comm_accept(portNameOmpiServer, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &interCommunicator);
        std::cout << "Accepted comm (Rank: " << initial_rank << ")!" << std::endl;
    }

    MPI_Comm newCommunicator;
    MPI_Intercomm_merge(interCommunicator, 0, &newCommunicator);

    int new_rank;
    MPI_Comm_rank(newCommunicator, &new_rank);
    int new_size;
    MPI_Comm_size(newCommunicator, &new_size);

    std::cout << "Old rank: " << initial_rank << ", new rank: " << new_rank << std::endl;
    std::cout << "Old size: " << initial_size << ", new size: " << new_size << std::endl;

    std::cout << "Disconnecting comm!" << std::endl;
    MPI_Comm_disconnect(&interCommunicator);

    if (initial_rank == 0) {
        std::cout << "Closing port!" << std::endl;
        MPI_Close_port(portNameOmpiServer);
        std::cout << "Port closed!" << std::endl;
    }

    MPI_Finalize();
    return 0;
}