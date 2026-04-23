FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y \
    cmake g++ git libssl-dev libyaml-cpp-dev libfmt-dev \
    libboost-all-dev pkg-config && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .
RUN mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make -j$(nproc)

FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    libssl3 libyaml-cpp0.7 libfmt9 libboost-system1.74.0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/build/fitness-api ./
COPY configs ./configs
EXPOSE 8080 8081
CMD ["./fitness-api", "--config", "configs/config.yaml"]