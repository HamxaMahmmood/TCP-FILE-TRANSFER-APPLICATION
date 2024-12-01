#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in server_addr;
    FILE *video_file;
    char buffer[BUFFER_SIZE];
    char file_name[100];
    printf("Enter the name of the video file to send: ");
    scanf("%s", file_name);  // Get the file name from the user
    // Open video file
    video_file = fopen(file_name, "rb");
    if (video_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error \n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    // Convert IPv4 addresses from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to receiver
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Read and send file contents
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, video_file)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    // Signal end of file (EOF marker or socket close)
    close(sock);
    fclose(video_file);
    
    printf("File sent successfully\n");
    return 0;
}
