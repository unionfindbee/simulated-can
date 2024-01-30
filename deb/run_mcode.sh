#!/bin/bash

# Description: This script will compile the mayhemit example and test it using Mayhem For Code
# Load the environment variables
source .env
NAME="simulated-can"
VERSION_UUID="$(uuidgen)"
MAYHEMFILE="Mayhemfile.yml"
ARCH="x86_64"

# Install Mayhem
mkdir -p ~/bin
export PATH=${PATH}:~/bin 
curl --no-progress-meter -Lo ~/bin/mayhem-${ARCH} ${MAYHEM_URL}/cli/Linux/mayhem  && chmod +x ~/bin/mayhem-${ARCH}
MAYHEM_PROMPT=1 mayhem-${ARCH} login --url ${MAYHEM_URL} --token ${MAYHEM_TOKEN}

# Build Dockerfile
REGISTRY=$(mayhem-${ARCH} docker-registry)
IMAGE="${REGISTRY}/${NAME}:${VERSION_UUID}"
docker build --platform=linux/amd64 -f Dockerfile -t ${IMAGE} .
echo "${MAYHEM_TOKEN}" | docker login -u ${MAYHEM_USERNAME} --password-stdin ${REGISTRY}
docker push ${IMAGE}

# Fuzz the code
mayhem-${ARCH} --verbosity info run . --project $NAME --owner ${MAYHEM_USERNAME} --image ${IMAGE} --file ${MAYHEMFILE} --duration 90
