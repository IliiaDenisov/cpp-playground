# Using a base image
FROM gcc:16.1

# Installing additional libs
RUN apt-get update && apt-get install -y \
    cmake \
    gdb \
    valgrind \
    ninja-build \
    clang-tidy-18 \
    && rm -rf /var/lib/apt/lists/*

# Setting working directory inside the container
WORKDIR /app

# Using default command
CMD ["bash"]