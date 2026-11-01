FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ make git pkg-config ca-certificates \
    qt5-qmake qtbase5-dev libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*



# Rarely-needed tools in their own layer so they don't slow the main one
RUN apt-get update && apt-get install -y --no-install-recommends \
    valgrind doxygen graphviz \
    && rm -rf /var/lib/apt/lists/*



WORKDIR /app


COPY GUI/ ./GUI/
COPY src/ ./src/
COPY ./TIVIDY.pro .




CMD ["/bin/bash"]