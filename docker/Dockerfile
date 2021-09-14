FROM debian:11

ENV DEBIAN_FRONTEND="noninteractive"
ARG PLATFORM_ARCH=x86_64
ENV DEBIAN_FRONTEND=noninteractive
ENV PATH="/usr/local/cuda/bin:/usr/local/cuda/nvvm/bin:${PATH}"
ENV LD_LIBRARY_PATH="/usr/local/cuda/lib64:/usr/lib/${PLATFORM_ARCH}-linux-gnu:${LD_LIBRARY_PATH}:/usr/local/lib"
#ENV LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib/python3.6/dist-packages/numpy.libs

RUN apt-get update -q \
    && DEBIAN_FRONTEND="noninteractive" apt-get install -qqy --upgrade \
       gnupg2 wget gridsite-clients ca-certificates curl lsb-release \
       python3-pip software-properties-common git build-essential \
       devscripts dpkg-dev fakeroot debhelper-compat \
    && apt-get -qqy autoremove \
    && rm -rf /var/lib/apt/lists/*

ARG NVIDIA_APT=https://developer.download.nvidia.com/compute/
RUN apt-key adv --fetch-keys ${NVIDIA_APT}/cuda/repos/debian10/x86_64/7fa2af80.pub \
    && add-apt-repository "deb ${NVIDIA_APT}/cuda/repos/debian10/x86_64/ /" \
    && add-apt-repository contrib \
    && add-apt-repository "deb ${NVIDIA_APT}/machine-learning/repos/ubuntu2004/x86_64/ /" \
    && apt-get update

ARG CUDA_VERSION=11.3
ARG CUDA_APT_VERSION=11-3

RUN apt-get install -qqy --upgrade cuda-toolkit-${CUDA_APT_VERSION} \
    && ( apt-get install -qqy --upgrade  \
            cuda-cufft-${CUDA_APT_VERSION} cuda-npp-${CUDA_APT_VERSION} \
         || apt-get install -qqy --upgrade \
            libcufft-${CUDA_APT_VERSION}  libnpp-${CUDA_APT_VERSION} ) \
    && ldconfig \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install --upgrade pip \
    && python3 -m pip install --upgrade \
       grpcio \
       future \
       h5py \
       keras_applications \
       keras_preprocessing \
       matplotlib \
       mock \
       numpy \
       'opt-einsum>=2.3.2' \
       pandas \
       Pillow \
       portpicker \
       scipy \
       sklearn \
       termcolor>=1.1.0

WORKDIR /tensorflow

#RUN git clone --recursive https://salsa.debian.org/deeplearning-team/tensorflow.git salsa
RUN git clone --recursive https://github.com/tranzmatt/tensorsalsa.git salsa \
    && wget http://wookware.org/files/tensorflow_2.3.1.orig.tar.xz

RUN cd salsa \
    && mk-build-deps --install --tool 'apt-get --yes --no-install-recommends' --remove ./debian/control \
    && rm -rf tensorflow-build-deps_*.buildinfo \
    && rm -rf tensorflow-build-deps_*.changes \
    && debuild -sa -b -uc -us --jobs=8 || true

#RUN mk-build-deps \
#    && apt install -y ./$(ls tensorflow*.deb) \
#    && rm $(ls tensorflow*.deb) \
#    && dpkg-buildpackage -us -uc

