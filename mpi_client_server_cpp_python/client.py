import numpy as np
from mpi4py import MPI
import ctypes

comm = MPI.COMM_WORLD

service_name = 'compute'
# get the opened port of the server by looking-up a service_name
port_name = MPI.Lookup_name(service_name)
# connect to the server
inter_comm = comm.Connect(port_name)

# send message to the server
send_obj = 3
print('Client sends to server ', ctypes.c_int(send_obj))
inter_comm.send(send_obj, dest=0, tag=0)
# get results from the server
#recv_obj = inter_comm.recv(source=0, tag=1)
#print 'Client receives %s from server.' % recv_obj

# disconnect from the server
inter_comm.Disconnect()