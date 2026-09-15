FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Base development tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    git \
    pkg-config \
    ca-certificates \
    gdb \
    clang \
    lldb \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y --no-install-recommends \
		#glm, openg, assimp
		libglm-dev \ 
		libgl1-mesa-dev \ 
		libglfw3-dev \ 
		libassimp-dev \
    && rm -rf /var/lib/apt/lists/*

# rp3d -> Physics dep
RUN git clone --depth 1 \
        https://github.com/DanielChappuis/reactphysics3d.git \
        /tmp/reactphysics3d \
    && cmake -S /tmp/reactphysics3d \
        -B /tmp/reactphysics3d/build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DRP3D_COMPILE_TESTS=OFF \
        -DRP3D_COMPILE_TESTBED=OFF \
    && cmake --build /tmp/reactphysics3d/build \
    && cmake --install /tmp/reactphysics3d/build \
    && rm -rf /tmp/reactphysics3d


##USER
ARG USERNAME=ape
ARG UID=1000
ARG GID=1000

RUN existing_user=$(getent passwd ${UID} | cut -d: -f1) && \
    if [ -n "$existing_user" ]; then \
        usermod -l ${USERNAME} "$existing_user" && \
        usermod -d /home/${USERNAME} -m ${USERNAME}; \
    else \
        if ! getent group ${GID} > /dev/null; then \
            groupadd --gid ${GID} ${USERNAME}; \
        fi && \
        useradd \
            --uid ${UID} \
            --gid ${GID} \
            --create-home \
            --shell /bin/bash \
            ${USERNAME}; \
    fi

USER ${USERNAME}

WORKDIR /Engine
CMD ["/bin/bash"]
