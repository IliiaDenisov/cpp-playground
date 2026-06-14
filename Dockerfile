FROM gcc:16.1 as base
RUN apt-get update && apt-get install -y \
    cmake \
    ninja-build \
    clang-tidy-18 \
    && rm -rf /var/lib/apt/lists/*

FROM base as dev
RUN apt-get install -y \
        gdb \
        valgrind \
        && rm -rf /var/lib/apt/lists/*
WORKDIR /app
CMD ["bash"]

FROM base as builder
COPY . /app
WORKDIR /app
RUN pwd
RUN ls
RUN bash build.sh

FROM gcc:16.1 as runtime
COPY --from=builder /app/build/playground /usr/local/bin/
COPY --from=builder /app/build/libgeometry_helper.so /usr/local/lib/
RUN ldconfig
ENTRYPOINT ["playground"]