# Project Wyvern

The goal of this project is to design and implement techniques that lets us evaluate function arguments in a lazy fashion. Lazyfication ca be implemented directly into the LLVM IR, or can be applied onto programs written in a programming language that supports optional lazy evaluation, such as D, Scala or Kotlin. The final goal is to boost the runtime efficiency of programs, by avoiding unnecessary computations whenever that's possible.

## Getting Started

### Prerequisites
* LLVM - Version 7.0.0
* DMD - Version 2.086.0
* LDC - Version 1.16.0
* CMAKE - Version 3.12.1
* Homebrew - Any Version (Optional - Only for Mac)
* Ninja - Any Version

### Getting Homebrew, CMAKE and Ninja
Simply execute this commands on yout Terminal. If you're on Mac, after install Homebrew you can use "brew install " instead of apt-get to get Ninja and CMAKE 

```
 ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" < /dev/null 2> /dev/null
 sudo apt-get CMAKE
 sudo apt-get install ninja-build
```

### Building LLVM
Below is a set of instructions to build LLVM 7.0.0 from source. Remember the path you build LLVM because you gonna need later.

```
 svn co http://llvm.org/svn/llvm-project/llvm/tags/RELEASE_701/final llvm 
 cd llvm/tools 
 svn co http://llvm.org/svn/llvm-project/cfe/tags/RELEASE_701/final clang 
 cd ../projects/ 
 svn co http://llvm.org/svn/llvm-project/compiler-rt/tags/RELEASE_701/final compiler-rt 
 cd ../tools/clang/tools/ 
 svn co http://llvm.org/svn/llvm-project/clang-tools-extra/tags/RELEASE_701/final extra
```

### Compiling LLVM

Compile LLVM is an Art and you can do this only if, on a Terminal, you're on the LLVM folder, then follow this steps and wait.
To compile an Debug version to use all Debugs tools use **DEBUG** instead of RELEASE on **-DCMAKE_BUILD_TYPE**.

```
mkdir build
cd build
cmake -G "Unix Makefiles" .. -DCMAKE_BUILD_TYPE=RELEASE  -DCMAKE_INSTALL_PREFIX=~/projects/llvm/build ~/projects/llvm
make -j16
```

### Getting DMD and LDC

If you're on MacOS just type:
```
brew install dmd
brew install ldc
```
If not, type:

```
curl -fsS https://dlang.org/install.sh | bash -s dmd
curl -fsS https://dlang.org/install.sh | bash -s ldc
```

If you have any issue to install these tools, please have a look on the [Dlang WebPage](https://dlang.org/download.html#dmd).

### Installing 

Simply clone this repository

```
git clone --recursive http://cuda.dcc.ufmg.br:8080/Robertorosmaninho/Project_Wyvern.git
```

