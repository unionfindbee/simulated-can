#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CAN_INTERFACE "vcan0"
#define INPUT_FILE "in.bin"
#define MAX_CAN_FRAME 16

// Function to initialize the virtual CAN bus
void init_can_bus() {
    // System commands to set up the virtual CAN interface
    system("modprobe vcan");
    system("ip link add dev vcan0 type vcan");
    system("ip link set up vcan0");
}

// Function to send data to the virtual CAN bus
void send_to_can_bus(const char *data, size_t size) {
    char command[256];
    char data_hex[3];
    strcpy(command, "cansend " CAN_INTERFACE " 123#");
    for (size_t i = 0; i < size && i < MAX_CAN_FRAME; i++) {
        sprintf(data_hex, "%02X", data[i] & 0xFF);
        strcat(command, data_hex);
    }
    system(command);
}

// Function to read data from the file and send it to the CAN bus
void read_input_and_send() {
    FILE *file = fopen(INPUT_FILE, "rb");
    if (file == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Read file contents and send to CAN bus
    char buffer[256];
    size_t bytes_read = fread(buffer, sizeof(char), sizeof(buffer), file);
    while (bytes_read > 0) {
        send_to_can_bus(buffer, bytes_read);
        bytes_read = fread(buffer, sizeof(char), sizeof(buffer), file);
    }

    fclose(file);
}

// Function to listen to the CAN bus and check for "bug"
void listen_to_can_bus_and_check() {
    FILE *pipe;
    char buffer[1024];

    // Start candump process
    pipe = popen("candump " CAN_INTERFACE, "r");
    if (pipe == NULL) {
        perror("Error opening pipe");
        exit(EXIT_FAILURE);
    }

    // Read the CAN bus
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Check for the "bug" sequence
        if (strstr(buffer, "62 75 67") != NULL) { // ASCII for "bug" is 62 75 67 in hex
            printf("Bug sequence detected! Aborting...\n");
            pclose(pipe);
            exit(EXIT_FAILURE);
        }
    }

    pclose(pipe);
}

int main() {
    // Initialize the virtual CAN bus
    init_can_bus();

    // Read input from file and send to CAN bus
    read_input_and_send();

    // Listen to the CAN bus and check for "bug"
    listen_to_can_bus_and_check();

    return 0;
}

