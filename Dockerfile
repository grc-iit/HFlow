FROM ubuntu:bionic

RUN apt-get update \
&& apt-get install -y  gcc git

RUN git clone https://github.com/scs-lab/spack.git \
 && cd ./spack \
 && git branch -u origin/v0.15.4.scs \
 && cd scripts \
 && ./install.sh \
 && ./setup_rhea.sh \
 && cd ${HOME} \
 && spack env activate symbios \
 && echo "spack env activate symbios" >> ${HOME}/.bashrc
