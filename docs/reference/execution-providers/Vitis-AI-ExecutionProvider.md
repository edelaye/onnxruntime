---
title: Vitis AI
parent: Execution Providers
grand_parent: Reference
---

# Vitis-AI Execution Provider
{: .no_toc }

[Vitis-AI](https://github.com/Xilinx/Vitis-AI) is Xilinx's development stack for hardware-accelerated AI inference on Xilinx platforms, including both edge devices and Alveo cards. It consists of optimized IP, tools, libraries, models, and example designs. It is designed with high efficiency and ease of use in mind, unleashing the full potential of AI acceleration on Xilinx FPGA and ACAP.

The current Vitis-AI execution provider inside ONNX Runtime enables acceleration of Neural Network model inference using DPUCZDX8G (Zynq), DPUCADX8G (U200, U250) and DPUCAHX8H (U50, U280) . These DPU's are targetted at accelerating for Convolutional Neural Networks (CNN) on top of the Xilinx [Zynq Ultrascale+
MPSoc](https://www.xilinx.com/products/silicon-devices/soc/zynq-ultrascale-mpsoc.html) and [Alveo](https://www.xilinx.com/products/boards-and-kits/alveo.html) platforms. More information about the different DPU's can be found in the table underneath.

| DPU                             | Application     | HW Platform                                                             | Quantization Method                                        | Quantization Bitwidth                             | Design Target                                                            |
|---------------------------------|-----------------|-------------------------------------------------------------------------|------------------------------------------------------------|---------------------------------------------------|--------------------------------------------------------------------------|
| Deep Learning Processing Unit   | C: CNN R: RNN   | AD: Alveo DDR AH: Alveo HBM VD: Versal DDR with AIE & PL ZD: Zynq DDR   | X: DECENT I: Integer threshold F: Float threshold R: RNN   | 4: 4-bit 8: 8-bit 16: 16-bit M: Mixed Precision   | G: General purpose H: High throughput L: Low latency C: Cost optimized   |


## Contents
{: .no_toc }

* TOC placeholder
{:toc}

## Requirements

The following table lists system requirements for running docker containers as well as Alveo cards.  


| **Component**                                       | **Requirement**                                            |
|-----------------------------------------------------|------------------------------------------------------------|
| Motherboard                                         | PCI Express 3\.0\-compliant with one dual\-width x16 slot  |
| System Power Supply                                 | 225W                                                       |
| Operating System                                    | Ubuntu 16\.04, 18\.04                                      |
|                                                     | CentOS 7\.4, 7\.5                                          |
|                                                     | RHEL 7\.4, 7\.5                                            |
| CPU                                                 | Intel i3/i5/i7/i9/Xeon 64-bit CPU                          |
| GPU \(Optional to accelerate quantization\)         | NVIDIA GPU with a compute capability > 3.0                 |
| CUDA Driver \(Optional to accelerate quantization\) | nvidia\-410                                                |
| FPGA                                                | Xilinx Alveo U50, U200, U250, U280                         |
| Docker Version                                      | 19\.03\.1                                                  |

## Build
See [Build instructions](../../how-to/build/eps.md#vitis-ai).

### DPUCADX8G (U200, U250)

1. Follow the Vitis AI instructions to set up the board: https://github.com/Xilinx/Vitis-AI/tree/1.3.2/setup/alveo/u200_u250.
2. Install Docker, and add the user to the docker group. Link the user to docker installation instructions from the following docker's website:
    * https://docs.docker.com/install/linux/docker-ce/ubuntu/
    * https://docs.docker.com/install/linux/docker-ce/centos/
    * https://docs.docker.com/install/linux/linux-postinstall/
3. Build and start the ONNX Runtime - Vitis AI Docker Container.
   ```
   cd {onnxruntime-root}/dockerfiles
   docker build -t onnxruntime-vitisai -f Dockerfile.vitisai .
   ./scripts/docker_run_vitisai.sh
   ```
   
   Setup inside container
   ```
   source /opt/xilinx/xrt/setup.sh
   conda activate vitis-ai-tensorflow
   ```

### DPUCAHX8H (U50, U280)

1. Follow the Vitis AI instructions to set up the board: https://github.com/Xilinx/Vitis-AI/tree/1.3.2/setup/alveo/u50_u50lv_u280.
2. Install Docker, and add the user to the docker group. Link the user to docker installation instructions from the following docker's website:
    * https://docs.docker.com/install/linux/docker-ce/ubuntu/
    * https://docs.docker.com/install/linux/docker-ce/centos/
    * https://docs.docker.com/install/linux/linux-postinstall/
3. Build and start the ONNX Runtime - Vitis AI Docker Container.
   ```
   cd {onnxruntime-root}/dockerfiles
   docker build --build-arg PYXIR_FLAG="--use_vai_rt_dpucahx8h" -t onnxruntime-vitisai -f Dockerfile.vitisai .
   ./scripts/docker_run_vitisai.sh
   ```
   
   Setup inside container
   ```
   conda activate vitis-ai-tensorflow
   ```

### DPUCZDX8G (Zynq Ultrascale+)

Deploying models on the Zynq devices requires a host machine for compiling models using the ONNX Runtime - Vitis AI dcoker, and a Zynq device for running the compiled models. The supported development boards are: ZCU104, ZCU102 and Ultra96.

**Host setup**

1. Install Docker, and add the user to the docker group. Link the user to docker installation instructions from the following docker's website:
    * https://docs.docker.com/install/linux/docker-ce/ubuntu/
    * https://docs.docker.com/install/linux/docker-ce/centos/
    * https://docs.docker.com/install/linux/linux-postinstall/
2. Build and start the ONNX Runtime - Vitis AI Docker Container.
   ```
   cd {onnxruntime-root}/dockerfiles
   docker build --build-arg PYXIR_FLAG="--use_dpuczdx8g_vart" -t onnxruntime-vitisai -f Dockerfile.vitisai .
   ./scripts/docker_run_vitisai.sh
   ```
   
   Setup inside container
   ```
   conda activate vitis-ai-tensorflow
   ```
   
**Board setup**

1. Download the Petalinux image for your target:
    * [ZCU102](https://www.xilinx.com/bin/public/openDownload?filename=xilinx-zcu102-dpu-v2020.2-v1.3.1.img.gz)
    * [ZCU104](https://www.xilinx.com/bin/public/openDownload?filename=xilinx-zcu104-dpu-v2020.2-v1.3.1.img.gz)
2. Use Etcher software to burn the image file onto the SD card.
3. Insert the SD card with the image into the destination board.
4. Plug in the power and boot the board using the serial port to operate on the system.
5. Set up the IP information of the board using the serial port.
6. Install ONNX Runtime and the ONNX Runtime - Vitis AI flow dependencies. This can be done using the script underneath and will take a very long time (12+ hours) to compile ONNX Runtime. Note that this script will create 8GB of swap space so make sure that your SD card has enough space. If you run into memory issues while compiling ONNX Runtime on the board, try executing `./build.sh --use_openmp --config RelWithDebInfo --enable_pybind --build_wheel --use_vitisai --parallel --update --build --build_shared_lib` again inside the ONNX Runtime repo. If this has happened, you will still have to install ONNX Runtime with `pip install build/Linux/RelWithDebInfo/dist/*.whl` 

```
#!/bin/bash

# Copyright 2021 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless requ
ired by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

export ONNXRT_VAI_HOME=$(pwd)
export ONNXRT_HOME="${ONNXRT_VAI_HOME}"/onnxruntime
export PYXIR_HOME="${ONNXRT_VAI_HOME}"/pyxir

if [ -d "${ONNXRT_HOME}" ]; then
  rm -rf ${ONNXRT_HOME}
fi
if [ -d "${PYXIR_HOME}" ]; then
  rm -rf ${PYXIR_HOME}
fi

# CREATE SWAP SPACE
if [ ! -f "/swapfile" ]; then
  fallocate -l 8G /swapfile
  chmod 600 /swapfile
  mkswap /swapfile
  swapon /swapfile
  echo "/swapfile swap swap defaults 0 0" > /etc/fstab
else
  echo "Couldn't allocate swap space as /swapfile already exists"
fi

# INSTALL DEPENDENCIES
if ! command -v h5cc &> /dev/null; then
  cd /tmp && \
    wget https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.7/src/hdf5-1.10.7.tar.gz && \
    tar -zxvf hdf5-1.10.7.tar.gz && \
    cd hdf5-1.10.7 && \
    ./configure --prefix=/usr && \
    make -j$(nproc) && \
    make install && \
    cd /tmp && rm -rf hdf5-1.10.7*
fi

cd ${ONNXRT_VAI_HOME}

pip3 install h5py==2.10.0 pillow

# DOWNLOAD PYXIR AND ONNX RUNTIME
git clone --recursive --branch v0.2.0 --single-branch https://github.com/Xilinx/pyxir.git "${PYXIR_HOME}"
git clone --recursive --single-branch https://github.com/microsoft/onnxruntime.git "${ONNXRT_HOME}"

# BUILD PYXIR FOR EDGE
cd "${PYXIR_HOME}"
sudo python3 setup.py install --use_vai_rt_dpuczdx8g --use_dpuczdx8g_vart

# BUILD ONNX RUNTIME
cd "${ONNXRT_HOME}"
./build.sh --use_openmp --config RelWithDebInfo --enable_pybind --build_wheel --use_vitisai --parallel --update --build --build_shared_lib
pip install build/Linux/RelWithDebInfo/dist/*.whl
```

**For details on steps 2-5, please refer to [Setting Up the Evaluation Board](https://www.xilinx.com/html_docs/vitis_ai/1_3/installation.html#yjf1570690235238)**


## Usage

The ONNX Runtime with Vitis AI flow contains two stages: Compilation and Execution. During the compilation a user can choose a model to compile for the DPU target devices that are currently supported. Once a model is compiled, the generated files can be used to run the model on a the specified target device during the Execution stage.

### Compiling a model

**Declare the target**

```python
dpu_target = 'DPUCADX8G' # options: 'DPUCADX8G', 'DPUCAHX8H-u50', 'DPUCAHX8H-u280', 'DPUCZDX8G-zcu104', 'DPUCZDX8G-zcu102', 'DPUCZDX8G-ultra96'
```

**Create an InferenceSession**

```
# Import pyxir before onnxruntime
import pyxir
import pyxir.frontend.onnx

import onnxruntime

# Add other imports 
# ...

# Load inputs and do preprocessing
# ...

# Create an inference session using the Vitis-AI execution provider
vitis_ai_provider_options = {'target': target, 'export_runtime_module': 'vitis_ai_dpu.rtmod'}
session = onnxruntime.InferenceSession('[model_file].onnx', None, ["VitisAIExecutionProvider"],
                                       [vitis_ai_provider_options])
```

**Quantize the model**

Usually, to be able to accelerate inference of Neural Network models with Vitis-AI DPU accelerators, those models need to quantized upfront. In the ONNX Runtime Vitis-AI execution provider we make use of on-the-fly quantization to remove this additional preprocessing step. In this flow, one doesn't need to quantize his/her model upfront but can make use of the typical inference execution calls (InferenceSession.run) to quantize the model on-the-fly using the first N inputs that are provided (see more information below). This will set up and calibrate the Vitis AI DPU and from that point onwards inference will be accelerated for all next inputs.

```
# First N (default = 128) inputs are used for quantization calibration and will
#   be executed on the CPU
# This config can be changed by setting the 'PX_QUANT_SIZE' (e.g. export PX_QUANT_SIZE=64)
input_name = [...]
outputs = [session.run([], {input_name: calib_inputs[i]})[0] for i in range(128)]
```

By default, the number of images used for quantization is set to 128. you could change the OTF Quantization behavior using the environment variables below:

| **Environment Variable**   | **Default if unset**      | **Explanation**                                         |
|----------------------------|---------------------------|---------------------------------------------------------|
| PX_QUANT_SIZE              | 128                    | The number of inputs that will be used for quantization (necessary for Vitis AI acceleration) |

For example, execute the following line in the terminal before calling the compilation script to reduce the quantization calibration dataset to eight images. This can be used for quick testing.

```
$ export PX_QUANT_SIZE=8
```

### Running a model

After the InferenceSession has been set up and quantization and compilation has completed, the InferenceSession can be used for accelerated inference on the DPU:

```
# Afterwards, computations will be accelerated on the FPGA
input_data = [...]
result = session.run([], {input_name: input_data})
```

Also, after the compilation step a `vitis_ai_dpu.rtmod` file got generated. This file can be used to load the quantized runtime module. This is necessary if you want to run on a Zynq device after quantizing and compiling on a host machine. For this, move the file to the Zynq board and use the `load_runtime_module` config to skip the Vitis AI quantization and compilation step. For example:

target = "DPUCZDX8G-zcu104"

```
vitis_ai_provider_options = {'target': target, 'load_runtime_module': 'vitis_ai_dpu.rtmod'}
session = onnxruntime.InferenceSession(model_path, None, ["VitisAIExecutionProvider"],
                                       [vitis_ai_provider_options])
```
