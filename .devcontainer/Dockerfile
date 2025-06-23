FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

ENV LANG=ja_JP.utf8
ENV LANGUAGE=ja_JP:ja
ENV LC_ALL=ja_JP.utf8
ENV TZ=Asia/Tokyo
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        locales \
        tzdata && \
    ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    echo $TZ > /etc/timezone && \
    rm -rf /var/lib/apt/lists/*

RUN sed -i -E 's/^#\s*(ja_JP.utf8\s+UTF-8)/\1/' /etc/locale.gen && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    locale-gen ja_JP.utf8

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        gnupg \
        lsb-release \
        software-properties-common \
        build-essential \
        clang \
        clang-format \
        less \
        libbsd-dev \
        libreadline-dev \
        openssh-client \
        python3-pip \
        valgrind \
        wget \
        man-db \
        manpages \
        manpages-dev && \
    rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
    install -m 0755 -d /etc/apt/keyrings && \
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc && \
    chmod a+r /etc/apt/keyrings/docker.asc && \
    echo \
      "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
      $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
      tee /etc/apt/sources.list.d/docker.list > /dev/null && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
        docker-ce-cli \
        docker-compose-plugin && \
    rm -rf /var/lib/apt/lists/*

CMD ["bash"]