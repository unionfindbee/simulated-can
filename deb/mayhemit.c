#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 1024

// Simulated CAN Bus structure
struct VirtualCANBus {
    char data[MAX_SIZE];
    int size;
};

// Write data to the simulated CAN Bus
void writeToCANBus(struct VirtualCANBus *bus, const char *data, int size) {
    memcpy(bus->data, data, size);
    bus->size = size;
}

// Check for the "bug" sequence
bool checkForBug(const char *data, int size) {
    for (int i = 0; i < size - 2; ++i) {
        if (data[i] == 'b' && data[i+1] == 'u' && data[i+2] == 'g') {
            return true;
        }
    }
    return false;
}

int main() {
    struct VirtualCANBus bus = {0};
    FILE *file;
    char *buffer;
    long filelen;

    // Open the file and read its contents
    file = fopen("in.bin", "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    filelen = ftell(file);
    rewind(file);

    buffer = (char *)malloc((filelen + 1) * sizeof(char));
    fread(buffer, filelen, 1, file);
    fclose(file);

    // Write the file contents to the virtual CAN bus
    writeToCANBus(&bus, buffer, filelen);

    // Check for the "bug" sequence
    if (checkForBug(bus.data, bus.size)) {
        printf("Aborting: 'bug' sequence detected\n");
	buffer[10000] = 'X'; // Invalid write
        // free(buffer);
        // return 1;
    } else {
        // Print the buffer if 'bug' is not found
        printf("Buffer contents: ");
        for (int i = 0; i < bus.size; i++) {
            printf("%c", bus.data[i]);
        }
        printf("\n");
    }

    // Your further processing goes here...

    free(buffer);
    return 0;
}

