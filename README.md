# cubeWorks

The cubeWorks package contains a group of programs to perform useful operations on files in the [Gaussian cube format](https://paulbourke.net/dataformats/cube/). 

<details>
  <summary>**Installation (UNIX terminal)**</summary>

## Installation (UNIX terminal)

- Download and unzip the source code
- Enter the cubeWorks directory
- type `make` to compile all programs
- type `make noFT` to compile all programs other than cubeFilter
    - (cubeFilter relies on single-precision version of [FFTW3](www.fftw.org) library)
- add cubeWorks/bin to your $PATH, copy contents of cubeWorks/bin

</details>