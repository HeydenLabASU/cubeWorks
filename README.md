# cubeWorks

The cubeWorks package contains a group of programs to perform operations on files in the [Gaussian cube format](https://paulbourke.net/dataformats/cube/). This format (initially developed for electron densities) is used for multiple output files by [3D-2PT](https://github.com/MatthiasHeyden/3D-2PT).

## Installation

<details>
  <summary>UNIX Terminal</summary>

> - download and unzip the source code
> - enter **cubeWorks** directory, *e.g.*  
> `cd cubeWorks-1.0` or `cd cubeWorks-main`
> - compile with [GNU make](www.gnu.org/software/make/)
>    - type `make` to compile all programs (requires float version of [FFTW3](www.fftw.org))  
>    or
>    - type `make noFT` to compile without -lfftw3f (skips compilation of **cubeFilter**)
> - make **cubeWorks** binaries findable
>    - add cubeWorks/bin to $PATH:  
>    `dir=$(pwd)`  
>    `echo "export PATH=\$PATH:${dir}/bin" >> ~/.bash_profile`  
>    or
>    - copy contents of cubeWorks/bin to usr/local/bin:  
>    `sudo cp bin/* usr/local/bin/`  
>    or
>    - ...  

</details>

## Using cubeWorks

All cubeWorks programs print a short usage message, *e.g.*  

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
> - A title is written in first of 3 header lines of output file. By default, title is taken from header of `input.cube` file, but optionally a new title can be provided on command line (white spaces in `output_title` currently not supported)

</details>

<details>
  <summary>cubeAdd</summary>

> `usage: cubeAdd A.cube B.cube output.cube [output_title]`  
> ` output.cube = A.cube + B.cube`  
>
> - adds data in cube files `A.cube` and `B.cube` and writes result into `output.cube`  
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

</details>

<details>
  <summary>cubeAver</summary>

> `usage: cubeAver A.cube B.cube ... output.cube`  
> ` output.cube = (A.cube + B.cube + ...) / N`  
>
> - averages data in cube files `A.cube`, `B.cube`, ... (no explicit limit on number of cube input files that can be specified) and writes result into `output.cube` (last command line argument)

</details>

<details>
  <summary>cubeCenter</summary>

> `usage: cubeCenter input.cube output.cube [output title]`  
>
> - ensures that the center of the grid is located at 0 0 0 (atom coordinates are moved as well)  
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

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
> - output is written to cube file with name derived from `input.cube` and sigma, *e.g.* `input_f-1.00A.cube`

</details>

<details>
  <summary>cubeMax</summary>

> `usage: cubeMax input.cube`  
> ` input.cube: cube file with density`  

> - identifies all local maxima in the density stored in `input.cube`
> - results are stored in a formatted ASCII file that conatins:
>   - coordinates of the maxima
>   - 1D indices of corresponding voxels
>   - peak intensities
>   - peak integrals (sum over voxels belonging to maximum)
>   - number of voxels belonging to peak
>   - indices of all voxels associated with each peak
> - output file name derived from `input.cube`, *e.g.* `input_maxima.dat`

</details>

<details>
  <summary>cubeMirror</summary>

> `usage: cubeMirror input.cube plane output.cube [output_title]`  
>
> - computes tha average of mirror images associated with a selected mirror plane
> - resulting data will be symmetric with respect to selected mirror plane
> - useful to reduce noise in symmetric systems
> - available mirror planes: XY, XZ, YZ
> - mirror operation is performed using indices in a 3D array storing the data, not actual coordinates (relevant if cube voxels not aligned with x,y,z axes -> triggers WARNING message)
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

</details>

<details>
  <summary>cubeMove</summary>

> `usage: cubeMove input.cube dX dY dZ output.cube [output_title]`  
> ` output.cube = input.cube + (dX dY dZ)`  
>
> - translates `input.cube` according to vector (dX dY dZ) (in Angstroms) and writes result to `output.cube`
> - translation is applied to grid points and atoms sepcified in `input.cube`
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

</details>

<details>
  <summary>cubeMult</summary>

> `usage: cubeMult A.cube B.cube output.cube [output_title]`  
> ` output.cube = A.cube * B.cube`  
>
> - multiplies data in cube files `A.cube` by data in `B.cube` and writes result into `output.cube`  
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

</details>

<details>
  <summary>cubeRot</summary>

> `usage: cubeRot A.cube axisX axisY axisZ angle_degree output.cube [output_title]`  
> ` output.cube = rot(axis,angle) @ input.cube`  
>
> - rotates `input.cube` around axis (axisX axisY axisZ) by specified angle in degrees and writes result to `output.cube`
> - rotation axis does not need to be normalized
> - rotation is applied to grid points and atoms sepcified in `input.cube`
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

</details>

<details>
  <summary>cubeScale</summary>

> `usage: cubeScale input.cube scale output.cube [output_title]`  
> ` output.cube = scale * input.cube`  
>
> - multiplies data in cube file `A.cube` by `scale` and writes result into `output.cube`  
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

</details>

<details>
  <summary>cubeScaleInv</summary>

> `usage: cubeScaleInv input.cube scale output.cube [output_title]`  
> ` output.cube = input.cube / scale`  
>
> - divides data in cube file `A.cube` by `scale` and writes result into `output.cube`  
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

</details>

<details>
  <summary>cubeSub</summary>

> `usage: cubeSub A.cube B.cube output.cube [output_title]`  
> ` output.cube = A.cube - B.cube`  
>
> - subtracts data in cube file `B.cube` from data in `A.cube` and writes result into `output.cube`  
> - title in header of `output.cube` is taken from `A.cube` or can be provided with `output_title` (white spaces in title currently not supported)

</details>

<details>
  <summary>cubeSum</summary>

> `usage: cubeSum A.cube B.cube ... output.cube`  
> ` output.cube = A.cube + B.cube + ...`  
>
> - sums data in cube files `A.cube`, `B.cube`, ... (no explicit limit on number of cube input files that can be specified) and writes result into `output.cube` (last command line argument)

</details>

<details>
  <summary>cubeTitle</summary>

> `usage: cubeTitle input.cube new_title`  
>
> - replaces title in header of `input.cube` with `new_title` (white spaces in `new_title` currently not supported)
> - overwrites `input.cube`  

</details>

<details>
  <summary>resolvate</summary>

> `usage: resolvate input.pdb input.cube dist`  
> ` input.pdb : structure file to be solvated`  
> ` input.cube: cube file with water density`  

> - assumes that `input.cube` contains a water density
> - identifies all local maxima of density in `input.cube`
> - results are stored in a formatted ASCII file that conatins:
>   - coordinates of the maxima
>   - 1D indices of corresponding voxels
>   - peak intensities
>   - peak integrals (sum over voxels belonging to maximum)
>   - number of voxels belonging to peak
>   - indices of all voxels associated with each peak
> - output file name derived from `input.cube`, *e.g.* `input_maxima.dat`
> - identifies all maxima within distance `dist` of any atom in `input.pdb`
> - write PDB file with HETATM entries for water oxygens associated position of all density maxima within distance `dist`
> - occupancy field of new HETATM entires correspond to peak integrals
> - PDB output file name derived from `input.cube` and `dist`, *e.g.* `input+shell-5.00A.pdb`

</details>
