#include <zephyr.h>
#include <device.h>
#include <drivers/can.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define CAN_DEV_NAME DT_LABEL(DT_CHOSEN(zephyr_can_primary))

static void can_rx_callback(const struct device *dev, struct zcan_frame *frame, void *arg) {
    ARG_UNUSED(dev);
    ARG_UNUSED(arg);

    // Check for 'b', 'u', 'g' in the CAN data
    bool found_bug = false;
    std::string data(reinterpret_cast<char*>(frame->data), frame->dlc);
    if (data.find("bug") != std::string::npos) {
        found_bug = true;
    }

    // If 'b', 'u', 'g' is found, abort
    if (found_bug) {
        std::cerr << "Found 'bug' in the CAN data. Aborting." << std::endl;
        exit(-1);
    }
}

void send_initial_data_to_can_bus(const struct device *can_dev) {
    std::ifstream inFile("in.bin", std::ios::binary | std::ios::ate);
    if (!inFile) {
        std::cerr << "Error opening in.bin" << std::endl;
        return;
    }

    std::streamsize size = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!inFile.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "Error reading in.bin" << std::endl;
        return;
    }

    // Construct a CAN frame from the buffer
    struct zcan_frame frame = {0};
    frame.id_type = CAN_STANDARD_IDENTIFIER;
    frame.rtr = CAN_DATAFRAME;
    frame.dlc = size;
    memcpy(frame.data, buffer.data(), size);

    // Send the frame
    if (can_send(can_dev, &frame, K_FOREVER, NULL, NULL) != 0) {
        std::cerr << "Failed to send frame" << std::endl;
    }

    inFile.close();
}

void main(void) {
    // Initialize CAN device
    const struct device *can_dev = device_get_binding(CAN_DEV_NAME);
    if (!can_dev) {
        std::cout << "CAN: Device driver not found." << std::endl;
        return;
    }

    // Send initial data to CAN bus
    send_initial_data_to_can_bus(can_dev);

    // Set up CAN receiver
    struct zcan_filter filter = {0};
    filter.id_type = CAN_STANDARD_IDENTIFIER;
    filter.rtr = CAN_DATAFRAME;
    filter.rtr_mask = 1U;
    filter.id_mask = CAN_STD_ID_MASK;

    can_attach_isr(can_dev, can_rx_callback, NULL, &filter);

    while (1) {
        // Main loop. The callback handles the CAN messages.
        k_sleep(K_MSEC(1000));
    }
}

