# Installing stationsimcpp

These instructions will get you a copy of stationsimcpp up and running on your machine with Ubuntu.

### Prerequisites

To get the code you will need git. To install git on Ubuntu use the following commands in a terminal

```bash
sudo apt update
sudo apt install git
```

To clone the repository from github use the following command in a terminal

```bash
git clone git@github.com:avramidis/stationsimcpp.git
```

To generate the makefiles for compiling stationsimcpp you will need cmake with release version equal of higher than 3.15.3. Go to https://cmake.org/download/
and download the latest binary distribution for Linux x86_64. For example, the latest file at the time when this doc was written was
cmake-3.16.2-Linux-x86_64.sh. To install cmake use the following command in a terminal

```bash
sudo cmake-3.16.2-Linux-x86_64.sh
```

To build and run stationsimcpp you will need a C++ compiler, the Python3 interpreter, numpy and matplotlib.

Use the following commands in a terminal to install the GNU compiler

```bash
sudo apt update
sudo apt install build-essential g++ gcc
```

To install the Python3 interpreter, numpy and matplotlib use the following command in a terminal

```bash
sudo apt install python3 python3-dev python3-pip python3-tk python3-numpy python3-matplotlib
```

### Build

Build with gcc navigate to the directory with 

```bash
mkdir build-gcc
cd build-gcc
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
sudo make install
```

Note: if you have an anaconda installation of python, then you may need to tell CMake where the environment that you
want to use is actually located. Assuming your python environment is called `stationsimcpp` then you can replace the 
`cmake -DCMAKE_BUILD_TYPE=Release ..` line above with:

```
cmake -DCMAKE_BUILD_TYPE=Release -DPython3_ROOT_DIR=~/opt/anaconda3/envs/stationsimcpp/ -DHDF5_ROOT=~/opt/anaconda3/envs/stationsimcpp/ ..
```

That line not only tells CMake where the `python3` installation that you want to use can be found, but it also tells
it to use the `hdf5` library included in that environment, not the default one found somewhere else on the system. 


## Support

If you encounter any problems, found a bug or need help, please open an [issue on github](https://github.com/avramidis/stationsimcpp/issues)  
with your system details, *exact* commands used, and logs posted as a [gist](https://gist.github.com/).