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

ARG CUDA_VERSION=11.3
ARG CUDA_APT_VERSION=11-3

ARG NVIDIA_APT=https://developer.download.nvidia.com/compute/
RUN apt-key adv --fetch-keys ${NVIDIA_APT}/cuda/repos/debian10/x86_64/7fa2af80.pub \
    && add-apt-repository "deb ${NVIDIA_APT}/cuda/repos/debian10/x86_64/ /" \
    && add-apt-repository contrib \
    && apt-get update \
    && apt-get install -qqy --upgrade cuda-toolkit-${CUDA_APT_VERSION} \
    && apt-get install -qqy --upgrade \
       cuda-cudart-${CUDA_APT_VERSION} cuda-cudart-dev-${CUDA_APT_VERSION} \
    && ( apt-get install -qqy --upgrade  \
            cuda-cufft-${CUDA_APT_VERSION} cuda-npp-${CUDA_APT_VERSION} \
         || apt-get install -qqy --upgrade \
            libcufft-${CUDA_APT_VERSION}  libnpp-${CUDA_APT_VERSION} ) \
    && ldconfig \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# WHY DOESN'T DEBIAN 10 NOT HAVE CUDNN???
RUN add-apt-repository "deb ${NVIDIA_APT}/cuda/repos/ubuntu2004/x86_64/ /" \
    && apt-get update \
    && apt-get install -qqy \
       libcudnn8-dev="8*${CUDA_VERSION}" libcudnn8="8*${CUDA_VERSION}" \
       libnvinfer-dev="8*${CUDA_VERSION}" libnvinfer8="8*${CUDA_VERSION}" \
       libnvinfer-plugin-dev="8*${CUDA_VERSION}" libnvinfer-plugin8="8*${CUDA_VERSION}" \
    && ldconfig \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

#       python3-libnvinfer-dev="8*${CUDA_VERSION}" python3-libnvinfer="8*${CUDA_VERSION}" \

RUN cd /usr/local/cuda-${CUDA_VERSION}/targets/$(uname -m)-linux/lib/ \
    && cp -a $(find . -name libcudart.so.${CUDA_VERSION}.* -print -quit) libcudart.so.${CUDA_VERSION}

RUN ls -l /usr/local/cuda-11.3/targets/x86_64-linux/lib/libcudart.so.11.3

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

ARG TF_VERSION=2.3.1
ADD tensorflow-build-deps_${TF_VERSION}*_amd64.deb .
RUN apt-get update -q \
    && apt install -y ./tensorflow-build-deps_${TF_VERSION}*_amd64.deb \
    && rm -rf /var/lib/apt/lists/*

#RUN git clone --recursive https://salsa.debian.org/deeplearning-team/tensorflow.git salsa
RUN wget http://wookware.org/files/tensorflow_${TF_VERSION}.orig.tar.xz
ARG SHA=LATEST
RUN SHA=${SHA} \
    git clone --single-branch --branch=master --recursive \
       https://github.com/tranzmatt/tensorsalsa.git salsa

WORKDIR /tensorflow/salsa
RUN git config pull.rebase true \
    && git pull \
    && apt-get update -q \
    && mk-build-deps --install --tool 'apt-get --yes --no-install-recommends' --remove ./debian/control \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf tensorflow-build-deps_*.buildinfo \
    && rm -rf tensorflow-build-deps_*.changes

RUN debuild -sa -b -uc -us --jobs=8 || true

#RUN mk-build-deps \
#    && apt install -y ./$(ls tensorflow*.deb) \
#    && rm $(ls tensorflow*.deb) \
#    && dpkg-buildpackage -us -uc

