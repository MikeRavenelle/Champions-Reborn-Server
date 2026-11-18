FROM debian:trixie-slim AS build
RUN apt-get update && apt-get install -y --no-install-recommends g++ cmake make && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY ["CMakeLists.txt", "Champions Reborn Server.vcxproj", "./"]
COPY Dependency Dependency
COPY Include Include
COPY Source Source
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j"$(nproc)"

FROM debian:trixie-slim
RUN useradd --system --create-home --home-dir /data champions
COPY --from=build /src/build/champions-server /usr/local/bin/champions-server
COPY bin/config.ini /opt/config.default.ini
COPY docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
WORKDIR /data
USER champions
VOLUME /data
EXPOSE 10101/udp 40900-40910/tcp 40810-40830/tcp
ENTRYPOINT ["docker-entrypoint.sh"]
