# Performing Mayhem Analysis on a Simulated CAN Bus   

This repository contains a demonstration of how to perform fuzzing testing on a simulated CAN bus using Mayhem. Since accessing a real CAN bus within our Docker container is not feasible, we simulate the CAN bus as a buffer. This buffer is filled by reading from a file named `in.bin`. During Mayhem analysis, Mayhem injects test cases into this file.

## Project Structure

The project consists of the following main components:

- `mayhemit.c`: The C program that simulates the CAN bus and includes the fuzzing logic.
- `Mayhemfile.yml`: Configuration file for Mayhem, specifying how the fuzzing should be performed.
- `Dockerfile`: Instructions for building the Docker container for the fuzzing environment.
- `run_mcode.sh`: A script to automate the process of compiling, building, and running fuzzing tests with Mayhem.

### `mayhemit.c`

This is the core C program that simulates the CAN bus. The program features:

- A `CANMessage` structure to define CAN messages with an ID, data field, and size.
- A `VirtualCANBus` structure to simulate a CAN Bus, holding CAN messages and tracking the count of messages.
- The `writeToCANBus` function to write CAN messages to the virtual CAN bus, managing the message storage and ensuring the bus does not overflow.
- The `checkForBug` function to search for a specific "bug" sequence within the data field of a CAN message.
- The `processFileData` function to read binary data from a file, convert it into CAN messages, check each message for the "bug" sequence, and write the message to the simulated CAN bus. If the "bug" sequence is detected, the program aborts, overwrites the buffer with 'X' characters, frees the memory, and exits.
- The main routine which:
  - Initializes the virtual CAN bus.
  - Reads binary data from `in.bin`.
  - Converts the binary data into CAN messages.
  - Writes the CAN messages to the virtual CAN bus.
  - Checks each CAN message for the "bug" sequence.
  - Prints out all CAN messages stored in the virtual CAN bus.
  - Manages memory allocation and freeing for the read data.

### `Mayhemfile.yml`

This YAML file configures Mayhem for fuzzing. It specifies:

- The Docker image to use.
- The duration for the fuzzing process.
- The project and target names.
- Tasks for exploitability factors, regression testing, behavior testing, and coverage analysis.
- Commands to execute the compiled C program with the input file `in.bin`.

### `Dockerfile`

This multi-stage Dockerfile is used to:

1. Compile the `mayhemit.c` program in an environment with necessary dependencies.
2. Create a lightweight final image containing only the compiled binary.

### `run_mcode.sh`

This script automates the fuzzing process with Mayhem, including:

- Setting up environment variables.
- Installing Mayhem CLI tools.
- Building the Docker image.
- Pushing the image to the registry.
- Running the fuzzing process with Mayhem.

## Usage

1. **Prepare the environment**: Ensure Docker and necessary dependencies are installed.
2. **Build and Run**: Use the `run_mcode.sh` script to compile, build, and start the fuzzing process.

    ```sh
    chmod +x run_mcode.sh
    ./run_mcode.sh
    ```

## Important Notes

- Ensure that the `.env` file is correctly set up with necessary environment variables before running `run_mcode.sh`.
- The `in.bin` file is crucial as it is where Mayhem injects test cases. Make sure it is in the correct location.
- Monitor the fuzzing process and results through the Mayhem interface.


To set up the environment variables required for the shell script, you need to create a .env file in your project directory. This file should contain the following environment variables:

```
MAYHEM_TOKEN
MAYHEM_URL
MAYHEM_USERNAME
```

Replace the empty double quotes with your actual Mayhem for Code token, URL, and username.

For example:

```
MAYHEM_TOKEN="your_mayhem_token"
MAYHEM_URL="https://your_mayhem_url"
MAYHEM_USERNAME="your_mayhem_username"
```


---

For more details on Mayhem and fuzzing strategies, visit [Mayhem Documentation](https://app.mayhem.security/docs/code-testing/summary/).

