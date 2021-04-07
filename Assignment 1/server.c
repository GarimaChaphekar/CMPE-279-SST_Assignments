// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 80

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";
    

    printf("execve=0x%p\n", execve);
//---------------------------------------------PART1 :SETTING UP THE SOCKET------------------------------------------//
    // 1.1 :Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 1.2 : Attaching socket to port 80 (setting up the socket options)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

//To connect to a remote server on a certain port number, we need 2 things, ip address and a port number to connect to.
//The sockaddr_in has a member called sin_addr of type in_addr which has a s_addr (It contains the IP address in long format)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // 1.3 : Forcefully attaching socket to the port 80. (Binding a socket to a particular "address and port" combination)

    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

// 1.4 : Putting the socket in listining mode to listen for connections.
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
//1.5 : Accepting new connections.
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
// At this point, connection is established between client and server, and they are ready to transfer data.

//---------------------------------------------PART 2 :PROCESSING THE DATA ------------------------------------------//

int pid = fork();
if (pid == -1)
{
printf("ERROR !! COULD NOT CREATE CHILD PROCESS");
exit(EXIT_FAILURE);
}
else if (pid == 0)
{
// Fork() return 0 for newly created child process.
printf("INSIDE THE CHILD PROCESS \n");
int setuidvalue = setuid(1001); // 1001 is the user id of noroot user (having no privileages)
if (setuidvalue < 0)
{
printf("ERROR !! COULD NOT CHANGE THE USER ID");
exit(EXIT_FAILURE);
}
else
{
valread = read(new_socket, buffer, 1024);
printf("%s\n", buffer);
send(new_socket, hello, strlen(hello), 0);
printf("Hello message sent\n");
}
}
else if (pid > 0)
{
// Fork() returns a positive value of process ID of newly created child process.
printf("INSIDE THE PARENT PROCESS.WAITING FOR THE CHILD\n");
wait(NULL);
printf("PARENT COMPLETE");

}

   
    return 0;
}

