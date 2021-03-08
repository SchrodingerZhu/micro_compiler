FROM archlinux
COPY . /project/
WORKDIR /project
RUN echo 'Server = https://mirrors.cloud.tencent.com/archlinux/$repo/os/$arch' | tee /etc/pacman.d/mirrorlist
RUN pacman -Syu --noconfirm cmake ninja gcc wget qemu-headless qemu-headless-arch-extra && pacman -Sc --noconfirm
RUN wget http://musl.cc/mipsel-linux-musl-cross.tgz && \
    tar xvzf mipsel-linux-musl-cross.tgz && \
    rm -f mipsel-linux-musl-cross.tgz && \
    ln -s /project/mipsel-linux-musl-cross/bin/mipsel-linux-musl-cc /usr/bin/mcc   && \
    ln -s /project/mipsel-linux-musl-cross/bin/mipsel-linux-musl-gcc /usr/bin/mgcc && \
    ln -s /project/mipsel-linux-musl-cross/bin/mipsel-linux-musl-g++ /usr/bin/mg++ && \
    ln -s /project/mipsel-linux-musl-cross/bin/mipsel-linux-musl-c++ /usr/bin/mc++ && \
    ln -s /project/mipsel-linux-musl-cross/bin/mipsel-linux-musl-nm /usr/bin/mnm   && \
    ln -s /project/mipsel-linux-musl-cross/bin/mipsel-linux-musl-objdump /usr/bin/mobjdump
RUN mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja && ninja all
