FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
	make \
	valgrind \
	doxygen \ 
	graphviz \
	pkg-config \
	qtbase5-dev \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*



WORKDIR /app


COPY makefile ./
COPY src/ ./src/


RUN make

ENTRYPOINT ["/bin/bash", "-c"]
CMD ["/bin/bash"]