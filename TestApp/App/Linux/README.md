
# Linux 
sudo apt install cmake libbz2-dev libxkbcommon-dev
 libexpat1-dev opencl-headers ocl-icd-opencl-dev libvulkan-dev libboost-all-dev
 libopencv-dev zlib1g-dev libcairo2-dev qt6-base-dev qt6-base-dev-tools


# You may have to set up a python link to python3 for cmake
sudo ln -s /usr/bin/python3 /usr/bin/python

# MacOs
brew install bzip2 expat opencv zlib qt6

# Docker
cd docker
docker-compose build semitone

# Forward the X11 port so the application can access the display from the container
xhost +local:root

cd /path/to/parent/dir/of/mx/and/komp/source

docker run -it \
-e DISPLAY=$DISPLAY \
-v /tmp/.X11-unix:/tmp/.X11-unix \
--rm -v $(pwd):/workspace docker_semitone /bin/bash


# Build using cmake
mkdir build
cd build
cmake -DSEMITONE_ROOT=/path/to/parent/dir/of/mx/and/komp/source ..
make -jn (n is number of cores available for building)