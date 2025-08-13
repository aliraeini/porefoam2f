# Capillary-Dominated Two-Phase Flow Simulation Using Porefoam

Ali Q. Raeini, Mosayeb Shams, Branko Bijeljic and Martin J. Blunt

Prepared by
- Ali Q. Raeini (2012-2021, 2025)

## Summary

To simplify and automate the use of the preprocessing, processing, and post-processing tools,
several bash scripts have been developed for different types of simulations using porefoam codes.
This document describes the scripts used to perform direct two-phase flow simulations,
specifically a primary-drainage simulation followed by water-injection simulations.

A brief description of the tools used by the scripts is provided.
While it is not necessary to know these details to use the scripts,
they can be useful for modifying the simulation setup in ways not covered by the scripts.

## Technical Note

### Experimental Code

The interface curvature algorithm in Shams et al. (2018) significantly improved the
discretization of capillary forces and, consequently, the accuracy of capillary pressure computations.
However, in complex flow domains, filtering is still necessary to reduce spurious currents,
especially in low-quality regions of unstructured meshes.

The code from Shams et al. (2018) was merged with the
filtering algorithm proposed in Raeini et al. (2012) after major revisions.
However, when filtering parallel to the interface is used,
the contact line and interface motion can exhibit an artificial **stick-slip behaviour**.
Note that stick-slip behaviour is also observed in natural contact line motion too,
but for a different, physical, reason.
Over-filtering capillary fluxes perpendicular to the interface could alleviate this issue,
but it risks suppressing trapping.

I certain case such as computing a layer's flow resistivity (Raeini et al. 2018),
the filtering side-effects are negligible because stick-slip only occurs when interfaces move.
The viscous forces that are used to compute layer conductivity are not affected when the flow is in steady state.
The filtering does not severely affect the computation of capillary pressure neither.

However, when modelling the motion of closed interfaces, such as ganglia and droplets,
the artificial stick-slip behaviour can significantly impair the accuracy of the interface motion.
Therefore, the default settings in the scripts shall be adjusted.

If you are interested in further developing the code, please see the contact details below.

## Installation

> [!WARNING]
> This document assumes the codes are saved in a folder named `~/Code/porefoam`.
> This path should be replaced with the actual path where the codes are downloaded.

### Prerequisites

In addition to standard Linux compilers and libraries (g++, cmake, and an MPI library),
other prerequisites are located in a folder named `pkgs`.
The `pkgs` folder includes zlib, libtiff, and a minified version of OpenFOAM called `foamx4m`.

> [!NOTE]
> Foamx4m requires a working `MPI` and `libscotch` to be installed on the system.
> Once compiled, foamx4m can coexist with other OpenFOAM installations without conflict.
> If you have other OpenFOAM versions installed,
> it is recommended to delete the executables in `~/Code/porefoam/pkgs/foamx4m/`
> to avoid having multiple copies of the same application.

### Downloading the Codes

To download the porefoam2f source code and its dependencies,
run the following commands in a bash terminal:

```bash
mkdir -p ~/Code/porefoam
cd ~/Code/porefoam
test -z "$(ls)" || ! echo "please run this in an empty folder"
mkdir ./src ./pkgs
echo include src/script/Makefile.msRoot > Makefile
git init
git submodule add  https://github.com/aliraeini/script.git      src/script
git submodule add  https://github.com/aliraeini/sirun.git       src/include
git submodule add  https://github.com/aliraeini/libtiff.git     pkgs/libtiff
git submodule add  https://github.com/aliraeini/zlib.git        pkgs/zlib
git submodule add  https://github.com/aliraeini/libvoxel.git    src/libvoxel
git submodule add  https://github.com/aliraeini/foamxm.git      pkgs/foamx4m
git submodule add  https://github.com/aliraeini/porefoam2f.git  src/porefoam2f
```

See the **parrent [porescale](https://github.com/aliraeini/porescale)** for adding
other related modules such as **single-phase flow simulator**, **contact angle** calculation
and **pore-network models**.

### Compiling the Codes

The codes require a recent C++ compiler that supports the `-std=c++20` flag.
The compiler is set using the `msCXX` variable in the `~/Code/porefoam/src/script/Makefile.in` file.
The default compiler (g++) will likely work, so no changes are usually needed.

To compile the codes, open a terminal and type the following commands:

```bash
(cd ~/Code/porefoam && make -j 8)
```

Change 8 based on the number of CPU cores on your computer.


To test the compilation, run:

```bash
(cd ~/Code/porefoam && make test)
```

After everything is compiled and working, you can run the following command to clean the temporary files:

```bash
(cd ~/Code/porefoam && make clean)
```

Running `make distclean` instead of `make clean` will delete the `~/Code/porefoam/lib/`,
`~/Code/porefoam/bin/`, `~/Code/porefoam/include/`, and `~/Code/porefoam/shared/` folders,
so *be extra careful* when using this option.

### Installation

To install the code in a terminal, run:

```bash
source ~/Code/porefoam/src/script/bashrc
```

Alternatively, you can edit your `~/.bashrc` (hidden) file and add the above command at the end.
This makes the porefoam scripts accessible in any new terminal you open.

### Input File Structure

The format specification for micro-CT images and their header files is provided in the
sample header file `Image.mhd`, located in the `~/Code/porefoam/docs/` directory.

**Background**: An "OpenFOAM case", or simply a "case", refers to the directory
containing the input files required by OpenFOAM and where the results are saved.
It should have two subdirectories named "constant" and "system", and directories
with numbers as their names, such as "0" or "0.1".

Some input parameters are controlled by a script used to automate the simulation setup,
specifically `AllRunImageTwoPhase` and `AllRunImageTwoPhaseCFMesh`,
located in the `~/Code/porefoam/porefoam2f/script` folder.
Comments in this file explain how to set simulation parameters,
and the most important ones are listed below:

```bash
#!/bin/bash
# usage: `AllRunImageTwoPhase  Image.mhd` three times
#   AllRunImageTwoPhase  #first run:  prepares two-phase flow mesh and inputs,
# ./AllRunImageTwoPhase  #second run: decompose mesh and,
# ./AllRunImageTwoPhase  #third run:  launches the simulation.

######################## MAIN INPUT PARAMETERS #########################

# contact angle for drainage measured through phase 1 (oil)
: ${theta0s:="150"}

# Inlet BCs, Darcy velocity (um/s) for oil phase:
: ${UD1s:=" 200 "}

# Inlet BCs, Darcy velocity (um/s) for water phase:
: ${UD0s:="10"}

# Used if oilFilldFracs<0, initialize from VSubElems image
: ${VSElml:=1}

# fill a small portion of the image near the inlet with oil (injecting phase)
# give up to two numbers one <<1 and the other >1
: ${oilFilldFracs:=" 0.1"} # or 1.05 for single-phase flow

# In case you want to refine the mesh increase this, or vise-versa.
: ${RefineLevel:=1}

# number of processors used for flow simulation
: ${nProc:=24}

#...
```

See Section [Simulation Parameters](#simulation-parameters) for more information.

## Running a Two-Phase Drainage Simulation

Copy the sample input data file to a directory with enough disk space
and run the `AllRunImageTwoPhase` script in the same directory:

```bash
# In case you haven't put these in your ~/.bashrc file:
source ~/Code/porefoam/src/script/bashrc
cd PATH_TO_.raw_.mhd_FILES
AllRunImageTwoPhase  # prepare the input script/files, note: no './'
```

This command prepares a base folder and a local `./AllRunImageTwoPhase` script if they do not already exist.
You can modify the input parameters in the local script and the `base` folder as needed
(see [Simulation Parameters](#simulation-parameters) for details).
You can then set up and run the simulations by running the local `./AllRunImageTwoPhase` script three or more times.
In a terminal, type (replace `Image.mhd` with the name of your image):

```bash
./AllRunImageTwoPhase Image.mhd # Generate the mesh
./AllRunImageTwoPhase Image.mhd # Decompose and set initial and BCs
./AllRunImageTwoPhase Image.mhd # Run a drainage simulation
```

**Important**: You must visualize the generated mesh and the decomposed mesh using Paraview before running the flow simulations.

## Running a Secondary Imbibition Simulation

After completing the drainage simulations, the boundary and initial conditions must
be changed for an imbibition simulation.
The scripts `PrepareForImbibition` and `PrepareForReverseImbibition`,
located in the `~/Code/porefoam/src/porefoam2f/script` folder, are used to make these adjustments.
`PrepareForReverseImbibition` performs the same function as `PrepareForImbibition`,
but prepares the simulation for water injection from the opposite direction.

To use these scripts to prepare drainage simulation results for an imbibition simulation,
type the following in a terminal:

```bash
PrepareForImbibition arg1 arg2 arg3 arg4 arg5 arg6 arg7
```

where:

- `arg1`: time at/before the end of drainage to be used as the start of water-injection.
- `arg2`: Darcy velocity (m/s).
- `arg3`: contact angle at solid walls.
- `arg4`: fraction of the image to be filled with water (initial condition), from the inlet.
- `arg5`: directory of the drainage case.
- `arg6`: base name for the water-injection case.

Example:

```bash
export PATH=$PATH:~/Code/porefoam/src/porefoam2f/script #in case

PrepareForImbibition 0.1 0.001 135 0.1 Berea8_0.007 BereaImb

# Alternatively, to reverse the flow direction:
PrepareForReverseImbibition 0.1 0.001 135 0.1 Berea8_0.007 BImbRev
```

After preparing the case for imbibition, navigate to the case directory in a terminal
and manually run the `interFaceFoam` two-phase flow solver.
(Note: You can also use this method to run a drainage simulation instead of the
third `./AllRunImageTwoPhase` run discussed in the previous section).

```bash
cd BereaImb*/ # go to the directory created for imbibition simulation
source  ~/Code/porefoam/src/script/bashrc
mpirun -np 8 interFaceFoam -parallel
# replace 8 with the number of decomposed processor domains
# ( = number of BereaImb*/*/processor* subfolders)
```

## Troubleshooting

The `AllRunImageTwoPhase` script runs a series of applications and redirects their
output to a `log.application` file in the directory where the application is being run.
If an error occurs (the application returns a non-zero code),
the location of the log file is printed in the terminal.
To identify and fix the problem, open the log files in a text editor,
starting with the log of the first crashed application.
It may also be helpful to check the log files of the applications that ran before and after the crashed one.

## Simulation Parameters

After running the `AllRunImageTwoPhase` command for the first time,
a local copy of this script and a base folder are created in the local directory.
You can edit these for a customized simulation setup.
The following section briefly discusses parameters you might need to change
for more accurate or stable results, along with some general guidelines.

After editing the local `./AllRunImageTwoPhase` script, you should run it three times
to generate a mesh, decompose it for a parallel run, and launch the simulation.
The first `./AllRunImageTwoPhase Image.mhd` run copies data from the base folder.
The second run copies data from the `Berea` folder.
The third run launches the flow simulator without making any changes to the input files.

**Note** that for the first and second runs of `./AllRunImageTwoPhase Image.mhd`,
which generate and decompose the mesh, respectively,
the values assigned in `./AllRunImageTwoPhase` take precedence and overwrite the values in the `base` or `Image/` folder.

### Parameters Adjustable from the `AllRunImageTwoPhase` Script:

* `cPc=0.2`: Capillary pressure compression coefficient.
  You generally do not need to change this, but if you do, the value should preferably be between 0.1 and 0.49.
* `cAlpha=1.0`: Indicator function (alpha) compression coefficient.
  You do not need to change this, but if you do, the value should preferably be between 0.5 and 2.0.
* `cPcCorrection=.1`: A correction coefficient to eliminate non-physical components
  of the capillary pressure gradient that are parallel to the interface.
  The value should be between 0.05 and 0.2.
  A higher value results in less spurious currents but more stick-slip behavior.
  This behavior is due to numerical errors in the computed curvature as the interface moves between grid blocks.
  This keyword increases these variations, which become dominant as the capillary number decreases.
* `SmoothingKernel=12`: Smoothing coefficient for computing interface normal vectors,
  which are used to calculate interface curvature.
  Values can range from 10 (no smoothing) to 19 (9 smoothing iterations).
  A lower value is recommended for coarse meshes because a higher value can decouple
  the indicator function and the computed curvature/pressure, causing simulations to diverge.
  Higher smoothing leads to a better capillary pressure estimate and is recommended for high-resolution meshes.
* `SmoothingRelaxFNearInterf=0.7`: Relaxation coefficient for smoothing interface normal vectors.
  The value can be between 0.5 and 1.0.
  For coarse meshes, a value close to 1 is recommended to prevent decoupling of the
  indicator function and the computed curvature/pressure, which can cause simulations to diverge.
  A lower value results in more smoothing and a better capillary pressure estimate,
  and is recommended for high-resolution meshes.
* `wallSmoothingKernel=0`: Smoothing coefficient for wall normal vectors.
  Use this to improve accuracy if the mesh is generated from a complex voxelized image.
  If the solid walls are smooth, setting this value to zero may lead to better accuracy.
* `Ufilter1=0.015`: This keyword filters (deletes) capillary fluxes
  (forces and velocities from capillary pressure and curvature force imbalance)
  when the capillary force is close to equilibrium with the capillary pressure gradient.
  A value of 0.01, for example, removes capillary force imbalances of less than 1% of the capillary force.
  This results in smoother interface motion and eliminates small spurious currents.
  Any value between 0.005 and 0.02 will produce physical results.
  Lower values may allow spurious currents,
  while higher values are considered over-filtering and may lead to under-prediction of trapping.
* `maxDeltaT=1e-5`: The largest allowed `dt` (time step).
  This value should be proportional to the grid size.

### `system/controlDict` File:

* `maxCo 0.1;`: The maximum Courant number, which should be between 0.05 and 0.2.
  Exercise caution when choosing higher values.
  Lower values lead to higher accuracy of time discretization but also longer simulation times.
* `maxAlphaCo 30;`: The maximum interface Courant number (see Raeini et al 2012 for details).
  This value should be between 0.05 and 0.2, and caution should be exercised with higher values.
  Lower values lead to higher accuracy of time discretization but also longer simulation times.

### `system/fvSolution` File:

* `cAlpha 1;`: Alpha compression factor.
* `cPc .2;`: Capillary pressure sharpening factor.
* `cBC 250;`: Boundary condition correction coefficient, which makes the boundary condition second-order accurate.
* `cPcCorrection 0.1;`: Corrects surface tensions parallel to the interface.
* `cPcCorrRelax2 1.;`: Relaxes (reduces) the amount of filtering applied by the `cPcCorrection` keyword.
  This value should be 1 or smaller; values smaller than 1 may lead to spurious currents.
* `smoothingKernel 12;`: Smoothing kernel (10 + number of smoothing iterations, now obsolete).
* `smoothingRelaxFactor 1.;`: Relaxation factor for curvature smoothing.
* `wallSmoothingKernel 5;`: Solid wall smoothing kernel (number of iterations).
* `uFilter1 .01;`: Filters surface tensions perpendicular to interfaces.
* `lambda 0;`: Slip length.
* `lambdaS 0;`: Slip length near the interface.
* `cSSlip 0.05;`: Threshold value for the indicator function (alpha) for detecting
  the interface location when applying the interface slip length (`lambdaS`).
  `lambdaS` is applied to all cells with alpha `< 1.0 - cSSlip`.
* `NSlip 1;`: The distance away from the interface (in number of cells) that `lambdaS` is applied.
* `UBoundByUSmoothFactor 2.;`: A filtering factor to eliminate locally high velocities
  that could destabilize the interface when it is not accurately represented
  (e.g., in coarse meshes or very thin films).
  The value can be anything greater than one,
  but a value less than 1.5 may introduce significant errors in the computed velocity.
  Essentially, the velocity of any cell that is more than
  this factor times the average velocity of its adjacent cells is penalized to
  this factor multiplied by the average of adjacent cell velocities.

### `constant/transportProperties` File:

```cpp
sigma sigma [ 1 0 -2 0 0 0 0 ] 0.03; //surface tension (SI units)
```

## Post-Processing Output Data

Basic post-processing can be done by visualizing simulation results with Paraview.
Simply open the `.foam` or `system/controlDict` file in the simulation results directory,
and Paraview will load the OpenFOAM case for visualization.

Advanced post-processing can be performed using the `upscale_grads` utility,
which also runs during `interFaceFoam` simulations.
During the `interFaceFoam` run, the average of various flow parameters is computed
every 10 time steps and written to a file named `data_out_for_plot`.
A header file, `data_out_for_plot_header`, is also created to assist in extracting
relevant parameters in Excel or Matlab.
The header file looks like this (all entries in a single line):

```cpp
t maxMagU aAvg aAvgL aAvgR avgUAlpha1_0 avgUAlpha1_1 avgUAlpha1_2 avgUAlpha2_0 avgUAlpha2_1 avgUAlpha2_2 QIn QOut Dp Dpc pcAvg ADarcy S1-alpha S1-U S1-vol S1-f_1 S1-dpddz S1-dpcdz S1-dpcdz_1 S1-dpddz_1 S1-viscz S1-viscz_1 S1-phiu S1-phiu_1 S1-delPdelZ S1-delPcelZ S1-viscInterf_1 S1-viscE S1-viscE_1 S1-dpEc S1-dpEc_1 S1-dpEd S1-dpEd_1 S1-phiE S1-phiE_1 S1-ZERO S1-Pc S1-xDropAvg S1-xDrop1 S1-xDrop2 S1-x1 S1-x2 ...
```

This data can be generated by running `upscale_grads` after the simulations are finished.
Post-processing is controlled by a file named `system/postProcessDict`.
This file can also be named `postProcessDict_Image`,
where "Image" is the name of the input `.mhd` header file without its suffix.

A more detailed description of the parameters written by `upscale_grads` is as follows:

Variables defined over the whole flow domain:

* `t`: Time (seconds).
* `maxMagU`: Maximum magnitude of the velocity field (U).
* `aAvg`: Average of the indicator function.
* `aAvgL`: Average of the indicator function on the `Left`-side (small x) boundary (usually the inlet).
* `aAvgR`: Average of the indicator function on the `Right`-side (large x) boundary (usually the outlet).
* `avgUAlpha1_0`: Average velocity of phase 1 (oil) in the x-direction (U1x).
* `avgUAlpha1_1`: Average velocity of phase 1 (oil) in the y-direction (U1y).
* `avgUAlpha1_2`: Average velocity of phase 1 (oil) in the z-direction (U1z).
* `avgUAlpha2_0`: Average velocity of phase 0 (water) in the x-direction (U0x).
* `avgUAlpha2_1`: Average velocity of phase 0 (water) in the y-direction (U0y).
* `avgUAlpha2_2`: Average velocity of phase 0 (water) in the z-direction (U0z).
* `QIn`: Flow rate at the left-side boundary.
* `QOut`: Flow rate at the right-side boundary.
* `Dp`: Average (arithmetic) dynamic pressure drop over the flow domain (`Pd_left - Pd_right`).
* `Dpc`: Average (arithmetic) capillary pressure drop over the flow domain (`Pc_left - Pc_right`).
* `pcAvg`: Average (volume-weighted) capillary pressure difference between the two phases.
* `ADarcy`: Darcy area (`=Dy x Dz`).

Variables defined over each control volume (numbered S1, S2, S3, etc., based on their order in `system/postProcessDict`):

* `S1-alpha`: Saturation (average of the indicator function, alpha).
* `S1-U`: Average pore velocity (of both phases).
* `S1-vol`: Volume of the control volume.
* `S1-f_1`: Fractional flow rate of phase 1 (oil).
* `S1-dpddz`: Average force per unit volume due to dynamic pressure gradients.
* `S1-dpcdz`: Average force per unit volume due to microscopic capillary pressure gradients
   (i.e., imbalance between microscopic capillary pressure and capillary forces).
* `S1-dpcdz_1`: Average force per unit volume due to microscopic capillary pressure gradients in phase 1.
* `S1-dpddz_1`: Average force per unit volume due to microscopic dynamic pressure gradients in phase 1.
* `S1-viscz`: Average force per unit volume due to viscous forces.
* `S1-viscz_1`: Average force per unit volume due to viscous forces inside phase 1.
* `S1-phiu`: Average force per unit volume due to advection of momentum.
* `S1-phiu_1`: Average force per unit volume due to advection of momentum in phase 1.
* `S1-delPdelZ`: Rate of energy (per unit time and volume, in SI units)
  entering/exiting the control volume boundaries due to dynamic pressure differences.
* `S1-delPcelZ`: Rate of energy entering/exiting the control volume boundaries due to capillary pressure differences.
* `S1-viscInterf_1`: Rate of energy crossing the boundary between the two fluids.
* `S1-viscE`: Rate of energy loss due to viscous forces (used for computing pressure drop).
* `S1-viscE_1`: Rate of energy loss due to viscous forces inside phase 1 (oil).
* `S1-dpEc`: Rate of energy introduced by microscopic capillary pressure gradient.
* `S1-dpEc_1`: Rate of energy introduced by microscopic capillary pressure gradient in phase 1.
* `S1-dpEd`: Rate of energy introduced by microscopic dynamic pressure gradient.
* `S1-dpEd_1`: Rate of energy introduced by microscopic dynamic pressure gradient in phase 1.
* `S1-phiE`: Rate of kinetic (inertial) energy introduced.
* `S1-phiE_1`: Rate of kinetic (inertial) energy introduced in phase 1.
* `S1-ZERO`: Dummy.
* `S1-Pc`: Average capillary pressure (the difference between the pc of two phases) in the control volume.
* `S1-xDropAvg`: An estimate of the length of the bounding box of the control volume.
* `S1-xDrop1`: An estimate of the length of the bounding box of phase 1 in the control volume.
* `S1-xDrop2`: An estimate of the length of the bounding box of phase 0 (water) in the control volume.
* `S1-x1`: Smallest x-coordinate covered by the control volume (left side).
* `S1-x2`: Largest x-coordinate covered by the control volume (right side).

This data is processed by a Python script named `groupGrads.py`,
which averages the data to produce relative permeability curves.

## Overview of Main Applications

### Third-Party Software:

* **OpenFOAM utilities and libraries**: Used for pre/post-processing and
  writing specialized pre/post-processing and simulation codes.
* **Paraview**: Used for visualization and some post-processing tasks.
* **OpenSCAD**: Used for automating the creation of surface models for simple geometries (now obsolete).
* **In-house developed codes (C++)**: Used when no alternatives were available.
* **Linux bash utilities**: Used as a user interface for simple calculations,
  changing input parameters, and running pre/post-processing and simulation codes.

### OpenFOAM Applications:

* `blockMesh`: Creates simple meshes or background meshes for `snappyHexMesh` (now obsolete, replaced by `cfMesh`).
* `renumberMesh`: Renumbers a mesh to improve performance.
* `decomposePar`: Decomposes a mesh into several pieces for parallel runs.
* `reconstructPar`: Reconstructs a decomposed mesh (not needed as everything is run in parallel).
* `setFields`: Used to set or modify the initial condition for the indicator function.
* `createPatch`: The version of `snappyHexmesh` used for mesh generation
  can interfere with boundaries (called 'patches' in OpenFOAM);
  `createPatch` is used to recreate the inlet/outlet boundaries (now obsolete).

### New/Modified Applications

* `voxelToFoam(Par)`: Converts `.raw`/`.tif`/`.am` files to the OpenFOAM format,
  used for single-phase flow simulation.
* `calc_perm`: Calculates single-phase permeability and porosity for single-phase simulations (obsolete).
* `vxlToSurf`: Creates a 3D surface between void and solid,
  used in mesh generation with `snappyHexMesh`/`cfMesh` for two-phase flow simulations.
* `surfaceSmoothVP`: Smoothes `voxelToSurface` output for `cfMesh` while preserving volume.
* `upscale_grads`: Calculates work and energy losses, used to plot relative permeability.
* `imageFileConvert`: Converts between raw file format and ASCII format,
  and performs simple image processing like cropping and thresholding.
* `interFaceFoam` and `interFacePropsBCs`: Direct two-phase flow simulators,
  including a new surface tension model,
  a pressure-velocity-surface-tension coupling algorithm, and several new boundary conditions.

## References

* M. Shams, A. Q. Raeini, M. J. Blunt, B. Bijeljic,
  “A numerical model of two-phase flow at the micro-scale using the volume-of-fluid method,”
  *J. Comp. Phys.* 357:159-82 (2018).  https://doi.org/10.1016/j.jcp.2017.12.027
* A. Q. Raeini, M. J. Blunt, and B. Bijeljic,
  “Modelling two-phase flow in porous media at the pore scale using the volume-of-fluid method”,
 *J. Comp. Phys.* 231:5653-68 (2012).  https://doi.org/10.1016/j.jcp.2012.04.011

This code has been used in the following works:

* M. Shams, K. Singh, B. Bijeljic, M. J. Blunt,
  “Direct Numerical Simulation of Pore-Scale Trapping Events During Capillary-Dominated Two-Phase Flow in Porous Media,”
  *Transp. Porous Med.* (2021).  https://doi.org/10.1007/s11242-021-01619-w
* A. Q. Raeini, B. Bijeljic, M. J. Blunt,
  “Generalized network modelling: Capillary-dominated two-phase flow”,
  *Phys. Rev. E*, 97(2):023308, (2018). https://doi.org/10.1103/physreve.97.023308

Note that the scripts used in the papers above, which used `snappyHexMesh` for mesh generation,
have been deprecated and are not included in this repository.

For more information, visit the [Imperial College Consortium on Pore-scale  Modelling Imaging](https://www.imperial.ac.uk/earth-science/research/research-groups/pore-scale-modelling).
