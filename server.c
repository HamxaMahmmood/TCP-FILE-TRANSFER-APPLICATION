#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#define PORT 8080
#define BUFFER_SIZE 1024
#define KILOBYTE 1024
int main() {
    char temp1[KILOBYTE];
    char temp2[KILOBYTE];

    char videofile1[KILOBYTE];
    char videofile2[KILOBYTE];

    // Read from stdin (which is redirected from the first pipe)
    fgets(videofile1, KILOBYTE, stdin);
    fgets(temp1, KILOBYTE, fdopen(4, "r"));
    // Read from file descriptor 5 (which is redirected from the folder2 pipe)

    // videofile1
    size_t length1 = strlen(temp1) + strlen(videofile1) + 1;
    char result1[length1];
    strcpy(result1, temp1);
    strcat(result1, videofile1);

 


    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    FILE *output_file;
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Hello from C %s\n",result1);
    printf("Waiting for connections...\n");

    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Open new file to write received video
    output_file = fopen(result1, "wb");
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Receive data and write to file
    size_t bytes_received;
    while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, output_file);
    }

    // Close file and socket
    fclose(output_file);
    close(new_socket);
    close(server_fd);

    printf("File received successfully\n");
    return 0;
}
