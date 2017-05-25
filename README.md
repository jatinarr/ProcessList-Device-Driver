# ProcessList-Device-Driver
This is the Miscellaneous Character Device Driver which gives the details of the running processes

The character device supports the read() operation. When the read() system call is invoked on your character device from a user space process (sample user program is checked in), the kernel module(device driver) returnsthe following information for all currently running processes:
1. process ID
2. parent process ID
3. the CPU on which the process is running 
4. its current state.

Instructions to execute:
1. make                           (This will compile the kernel module and user program both)
2. sudo insmod process_list.ko    (This will install the KERNEL MODULE. NOTE: Install using root user i.e. sudo user)
3. sudo ./userproc				  (This will run the user level program. NOTE: Run with root user i.e. sudo user)
4. make clean (optional)          (Optional: Delete the object and executable files)


Output of Program:
1. List of all process with PID, PPID, CPU and STATE

NOTE: User program is using GCC compiler to compile the code. Please have GCC installed on the machine.
