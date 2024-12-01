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

  

  
    char temp2[KILOBYTE];

    char videofile2[KILOBYTE];

    // Read from file descriptor 3 (which is redirected from the second pipe)
    fgets(videofile2, KILOBYTE, fdopen(3, "r"));
    // Read from file descriptor 5 (which is redirected from the folder2 pipe)
    fgets(temp2, KILOBYTE, fdopen(5, "r"));


    // videofile2
  
    // videofile4
    size_t length4 = strlen(temp2) + strlen(videofile2) + 1;
    char result4[length4];
    strcpy(result4, temp2);
    strcat(result4, videofile2);
  
    
    printf("Hello from client %s",result4);







    int sock = 0;
    struct sockaddr_in server_addr;
    FILE *video_file;
    char buffer[BUFFER_SIZE];
   
    video_file = fopen(result4, "rb");
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
