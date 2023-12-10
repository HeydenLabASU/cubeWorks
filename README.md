# cubeWorks

The cubeWorks package contains a group of programs to perform useful operations on files in the [Gaussian cube format](https://paulbourke.net/dataformats/cube/). 

## Installation

<details>
  <summary>UNIX Terminal</summary>

> - download and unzip the source code
> - enter **cubeWorks** directory, *e.g.*  
> `cd cubeWorks-1.0`
> - compile with [GNU make](www.gnu.org/software/make/)
>    - type `make` to compile all programs (requires float version of [FFTW3](www.fftw.org))  
>    or
>    - type `make noFT` to compile without -lfftw3f (skips compilation of **cubeFilter**)
> - make **cubeWorks** binaries findable
>    - add cubeWorks/bin to $PATH:  
>    `dir=$(pwd)`  
>    `echo "export PATH=$PATH:${dir}/bin" >> ~/.bash_profile`  
>    or
>    - copy contents of cubeWorks/bin to usr/local/bin:  
>    `sudo cp bin/* usr/local/bin/`  
>    or
>    - ...  

</details>

## Using cubeWorks

All cubeWorks programs print a short usage, *e.g.*  

`[user@localhost:~]$ cubeAdd`  
`usage: cubeAdd A.cube B.cube output.cube [output_title]`  
` output.cube = A.cube + B.cube`

Command line arguments in square brackets (`[ 

]`) are optional.

## Short Description of Each Program

<details>
  <summary>cube2dat</summary>

> `usage: cube2dat input.cube [output_filename] [scaling factor] ["output_title"]`
>
> - converts file `input.cube` into ASCII data file with explicitly written x, y and z coordinates  
> - if `output_filename` is not specified, the `.cube` extension of the input file is replaced by `.dat`  
> - volume data in `input.cube` can be scaled with `scaling_factor`  
> - A title is written in first of 4 header lines of output file. By default, title is taken from header of `input.cube` file, but optionally a new title can be provided on command line (white spaces in `output_title` currently not supported).

  </details>