# cubeWorks

The cubeWorks package contains a group of programs to perform useful operations on files in the [Gaussian cube format](https://paulbourke.net/dataformats/cube/). 

## Installation

<details>
  <summary>UNIX Terminal</summary>

- download and unzip the source code
- enter the cubeWorks directory
- compile with GNU make
    - type `make` to compile all programs (requires float version of [FFTW3](www.fftw.org))  
    OR
    - type `make noFT` to compile without fftw3 (no cubeFilter)
- make cubeWorks binaries findable
    - add cubeWorks/bin to $PATH:
    `dir=$(pwd)`
    `echo "export PATH=$PATH:${dir}/bin" \>& ~/.bash_profile`
    OR
    - copy contents of cubeWorks/bin to usr/local/bin:
    `sudo cp bin/* usr/local/bin/`
    OR
    - ...

</details>