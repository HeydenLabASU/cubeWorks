# cubeWorks

The cubeWorks package contains a group of programs to perform useful operations on files in the [Gaussian cube format](https://paulbourke.net/dataformats/cube/). 

## Installation

<details>
  <summary>UNIX Terminal</summary>

> - download and unzip the source code
> - enter **cubeWorks** directory, *e.g.*  
> `cd cubeWorks-1.0.0` or `cd cubeWorks-main`
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

Command line arguments in square brackets (`[ ]`) are optional.

## Short Description of Each Program

<details>
  <summary>cube2dat</summary>

> `usage: cube2dat input.cube [output_filename] [scaling factor] ["output_title"]`
>
> - converts file `input.cube` into ASCII data file with explicitly written x, y and z coordinates  
> - if `output_filename` is not specified, the `.cube` extension of the input file is replaced by `.dat`  
> - volume data in `input.cube` can be scaled with `scaling_factor`  
> - A title is written in first of 3 header lines of output file. By default, title is taken from header of `input.cube` file, but optionally a new title can be provided on command line (white spaces in title currently not supported).
- **NOTE:** only intended for orthorhombic data

</details>

<details>
  <summary>cubeAdd</summary>

> `usage: cubeAdd A.cube B.cube output.cube [output_title]`
> ` output.cube = A.cube + B.cube`
>
> - adds data in cube files `A.cube` and `B.cube` and writes result into `output.cube`  
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported).

</details>

<details>
  <summary>cubeAver</summary>

> `usage: cubeAver A.cube B.cube ... output.cube`
> ` output.cube = (A.cube + B.cube + ...) / N`
>
> - averages data in cubes file `A.cube`, `B.cube`, ... (no explicit limit on number of cube input files that can be specified) and writes result into `output.cube` (last command line argument)

</details>

<details>
  <summary>cubeCenter</summary>

> `usage: cubeCenter input.cube output.cube [output title]`
>
> - ensures that the center of the grid is located at 0 0 0 (atom coordinates are moved as well)  
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported).

</details>

<details>
  <summary>cubeDiv</summary>

> `usage: cubeDiv A.cube B.cube output.cube [escape]`
> ` output.cube = A.cube / B.cube`
> ` escape: result of division by zero`
>
> - divides data in cube files `A.cube` by data in `B.cube` and writes result into `output.cube`  
> - if division by zero is encountered, the result can be defined with `escape` (0 by default)

</details>

<details>
  <summary>cubeFilter</summary>

> `usage: cubeFilter input.cube sigma`
> ` input.cube: cube file to apply filter on`
> ` sigma     : sigma of Gaussian filter (Angstrom)`
>
> - applies 3D Gaussian blur filter on data in `input.cube` to suppress noise
> - sigma specifies the kernel width of the filter in Angstrom
>   - small sigma: 
> - output is written to cube file with name derived from `input.cube` and sigma, *e.g.* `input`
> - if division by zero is encountered, the result can be defined with `escape` (0 by default)

</details>
