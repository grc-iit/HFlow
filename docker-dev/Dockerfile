FROM jcernuda/rhea:1.1.0

RUN apt-get update \
&& apt-get install -y ssh \
build-essential \
cmake \
autoconf \
libtool \
pkg-config \
wget \
checkinstall \
zlib1g-dev \
libssl-dev
gcc \
g++ \
gdb \
clang \
cmake \
rsync \
tar \
python \
vim \
&& apt-get clean

RUN sudo sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
ENTRYPOINT service ssh restart && bash