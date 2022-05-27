mpirun -np 1  ompi-server --no-daemonize -r + &

mpiexec -report-pid - -n 1 --ompi-server file:openmpi_address python server.py

mpirun --ompi-server file:openmpi_address -np 1 python3 client.py
