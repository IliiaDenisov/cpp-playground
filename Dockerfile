# Using a base image
FROM gcc:14

# Installing additional libs
RUN apt-get update && apt-get install -y \
    cmake \
    gdb \
    valgrind \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Setting working directory inside the container
WORKDIR /cpp-app

# Using default command
CMD ["bash"]