FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    build-essential \
    libsdl2-dev \
    libsdl2-image-dev \
    libpng-dev \
    libjpeg-dev \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /kaleidoscope

COPY src/ ./src/
COPY Makefile .
COPY img/image ./img/image

RUN make

ENV DISPLAY=:0

CMD "./kaleidoscope"

