# cubeWorks

The cubeWorks package contains a group of programs to perform useful operations on files in the [Gaussian cube format](https://paulbourke.net/dataformats/cube/). 

## Installation

<details>
  <summary>UNIX Terminal</summary>

- Download and unzip the source code
- Enter the cubeWorks directory
- type `make` to compile all programs (requires float version of [FFTW3](www.fftw.org))
OR
- type `make noFT` to compile without fftw3 (no cubeFilter)
- add cubeWorks/bin to your $PATH, copy contents of cubeWorks/bin

</details>