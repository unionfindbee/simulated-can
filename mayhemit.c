#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 1024

// Define a structure for CAN messages
struct CANMessage {
    unsigned int id; // CAN ID
    unsigned char data[8]; // CAN data field (max 8 bytes)
    unsigned char size; // Length of data field (0-8)
};

// Simulated CAN Bus structure
struct VirtualCANBus {
    struct CANMessage messages[MAX_SIZE];
    int messageCount;
};

// Write data to the simulated CAN Bus as CAN messages
void writeToCANBus(struct VirtualCANBus *bus, const struct CANMessage *msg) {
    if (bus->messageCount < MAX_SIZE) {
        bus->messages[bus->messageCount] = *msg;
        bus->messageCount++;
    } else {
        printf("Error: Bus is full\n");
    }
}

// Check for the "bug" sequence in CAN message data
bool checkForBug(const struct CANMessage *msg) {
    for (int i = 0; i < msg->size - 2; ++i) {
        if (msg->data[i] == 'b' && msg->data[i+1] == 'u' && msg->data[i+2] == 'g') {
            return true;
        }
    }
    return false;
}

// Read binary data and convert it into CAN messages
void processFileData(char *buffer, long filelen, struct VirtualCANBus *bus) {
    // Example: Process each 8 bytes of data as one CAN message (simplified)
    for (long i = 0; i < filelen; i += 8) {
        struct CANMessage msg;
        msg.id = i / 8; // Just an example ID assignment
        msg.size = (filelen - i < 8) ? filelen - i : 8;
        memcpy(msg.data, buffer + i, msg.size);
        
        if (checkForBug(&msg)) {
            printf("Aborting: 'bug' sequence detected in message ID %u\n", msg.id);
            free(buffer);
            exit(1);
        }

        writeToCANBus(bus, &msg);
    }
}

int main() {
    struct VirtualCANBus bus = {0};
    FILE *file;
    char *buffer;
    long filelen;

    file = fopen("in.bin", "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    filelen = ftell(file);
    rewind(file);

    buffer = (char *)malloc(filelen * sizeof(char));
    fread(buffer, filelen, 1, file);
    fclose(file);

    processFileData(buffer, filelen, &bus);

    // Print the CAN messages
    printf("CAN Bus Messages:\n");
    for (int i = 0; i < bus.messageCount; i++) {
        printf("ID: %u, Data: ", bus.messages[i].id);
        for (int j = 0; j < bus.messages[i].size; j++) {
            printf("%02X ", bus.messages[i].data[j]);
        }
        printf("\n");
    }

    free(buffer);
    return 0;
}

