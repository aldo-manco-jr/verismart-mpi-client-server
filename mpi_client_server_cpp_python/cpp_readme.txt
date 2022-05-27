
Client/Server Architecture between C++ Programs

# compile server and client and generate their executable:
mpicxx -o server_cpp server.cpp
mpicxx -o client_cpp client.cpp

# disable iptables with following commands:
sudo iptables -P INPUT ACCEPT
sudo iptables -P OUTPUT ACCEPT
sudo iptables -P FORWARD ACCEPT
sudo iptables -F

# run ompi server for communication between client and server, and run server and clients:
mpirun -np 1  ompi-server --no-daemonize -r + &
# save address of ompi server in a file called (address_ompi_server) with a new line at the end
mpirun -np 1 --ompi-server file:address_ompi_server ./server_cpp
mpirun -np 2 --ompi-server file:address_ompi_server ./client_cpp