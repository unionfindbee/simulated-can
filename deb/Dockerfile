# Stage 1: Compile the application
FROM debian:12-slim AS compile-image

# Install necessary packages for building
RUN apt-get update && apt-get install -y build-essential

# Copy your C program into the Docker image
COPY . /usr/src/app
WORKDIR /usr/src/app

# Compile your C program
RUN gcc -o mayhemit mayhemit.c

# Stage 2: Create the final image
FROM debian:12-slim AS final-image

# Copy the compiled binary from the compile-image
COPY --from=compile-image /usr/src/app/mayhemit /mayhemit

