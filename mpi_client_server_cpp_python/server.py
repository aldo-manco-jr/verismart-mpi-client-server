import numpy as np
from mpi4py import MPI

comm = MPI.COMM_WORLD

service_name = 'compute'
# open a port
port_name = MPI.Open_port()
# bind the opened port to a service_name,
# client can connect to the port by looking-up this service_name
MPI.Publish_name(service_name, port_name)
# wait for client to connect
inter_comm = comm.Accept(port_name)

#recv_obj = np.zeros([1,3], dtype=int)
#recv_obj = np.zeros([1,3], dtype=np_float32)

# receive message from client
#inter_comm.Recv(buf=recv_obj, source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG)

import os

file_path = "/home/aldo/Desktop/program_analysis/learn/mpi/python/exercises/mpi_client_server_py2py/cases_auto_config.json"

# check if file is present
if os.path.isfile(file_path):
    # open text file in read mode
    text_file = open(file_path, "r")

    # read whole file to a string
    data = text_file.read()

    # close file
    text_file.close()

    print(data)

buf = bytearray(256)
inter_comm.Recv([buf,MPI.CHAR],source=MPI.ANY_SOURCE,tag=MPI.ANY_TAG)
# Decode the string to remove the empty characters from bytearray
print(buf.decode('utf-8'))

encoded_string = data.encode()
byte_array = bytearray(encoded_string)
inter_comm.Send([byte_array,MPI.CHAR],dest=0)

#print('Server receives from client ', recv_obj)
#send_obj = eval(recv_obj)
# reply the result to the client
#print 'Server sends %s to client.' % send_obj
#inter_comm.send(send_obj, dest=0, tag=1)

# unpublish the service_name, close the port and disconnect
MPI.Unpublish_name(service_name, port_name)
MPI.Close_port(port_name)
inter_comm.Disconnect()
