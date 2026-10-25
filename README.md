# porefoam2f Module: Development Version

Porefoam2f is a code that simulates incompressible two-phase flow
in 3D images of porous media using the OpenFOAM finite-volume library.

---


> [!WARNING]
> This code is experimental and requires further development;
> see the [Technical Note](./UserGuide.md#technical-note) in the [User Guide](./UserGuide.md)

---

## Instructions

See [porefoam2f/UserGuide.md](./UserGuide.md#Installation) for instructions on how to **organise**, **download**, **build** and **run** the code and its dependencies.

The download and build instructions are same as in the **parrent [porescale](https://github.com/aliraeini/porescale) repository**.

You can run a sample simulation by following the commands executed by `make test` from the [porefoam2f/test2f](./test2f) directory.
The sample simulation run with `make test` shows a significant level of stick-slip behaviour,
and this test case shall be updated to minimise these artefacts.

**TODO**: Add another test case based on the star-shaped geometry from Raeini et al. (2014).

---

## Dependencies

| Dependency | Used by | Notes |
|---|---|---|
| [foamx4m](https://github.com/aliraeini/foamxm) | all solvers and utilities (`USE_MINIFOAMX3=1`) | built from `pkgs/foamx4m`; includes cfMesh, used by the `*CFMesh*` cases |
| [image3kit](https://github.com/image3kit/image3kit) voxlib (or [libvoxel](https://github.com/aliraeini/libvoxel) in porescale) | `interFaceFoam`, voxel-to-mesh utilities | `USE_VOX_IMG=1` |
| pocketpy | `VxlPy { ... }` scripts in `.mhd` headers | `USE_POCKETPY=1`, shipped with image3kit |
| zlib, libtiff | image I/O | `USE_ZLIB=1`, `USE_TIFF=1` |
| OpenMP, MPI, scotch | parallel runs | system packages |
| [script](https://github.com/aliraeini/script) | build (`Makefile.in`), runtime env (`bashrc`) | `msSrc/script` |
| Python 3 | test post-processing (`test2f/post_process.py`) | |
| [pnmkit](https://github.com/difizix/pnmkit) (optional) | Python case runners and pytest tests | `pnmkit.porefoam` |

## Use from pnmkit

`pnmkit.porefoam` provides Python replacements for the `script/AllRunImageTwoPhase*` scripts
(`porefoam2f_case.py`, `porefoam2f_cli.py`, `run_2f_*.py`). They use the case templates in
[script/base](script/base) and the same executables, which must be on `PATH`.
See `pnmkit/porefoam/README.md` in the pnmkit repository.

## Tests

Both test sets are kept and run the same four voxel-cylinder cases (`SnpCCF`, `SnpFSF`, `CfmCCF`, `CfmFSF`),
compared against [test2f/ref](test2f/ref):

 - Native: `make test -j2` here runs [test2f](test2f) (bash `*.sh` + `post_process.py`) and
   [test2fCrnr](test2fCrnr) (star/triangle throats).
 - pytest: `pytest -m regression pnmkit/porefoam/tests` (see `pnmkit/porefoam/tests/README.md`;
   `make compare` there diffs the two sets of outputs).

---

### References

This code is an experimental merge of algorithms published in the following papers:

 - M Shams, A Q Raeini, M J Blunt, B Bijeljic, “A numerical model of two-phase flow at the micro-scale using the volume-of-fluid method”, J Comp. Phys. 357:159–82 (2018) https://doi.org/10.1016/j.jcp.2017.12.027

 - A Q Raeini, M J Blunt, and B Bijeljic, “Modelling two-phase flow in porous media at the pore scale using the volume-of-fluid method”,  J Comp. Phys. 231:5653–68 (2012) https://doi.org/10.1016/j.jcp.2012.04.011


This code has been used in the following works:

 - M Shams, K Singh, B Bijeljic, M J Blunt, “Direct Numerical Simulation of Pore-Scale Trapping Events During Capillary-Dominated Two-Phase Flow in Porous Media”, Transp Porous Med (2021). https://doi.org/10.1007/s11242-021-01619-w

 - A Q Raeini, B Bijeljic, M J Blunt, “Generalized network modelling: Capillary-dominated two-phase flow”, Phys. Rev. E,  97(2):023308, (2018). https://doi.org/10.1103/physreve.97.023308

---

### Credits

* **Prof. Stephane Zaleski**: For supervision on tracking the interface and computing interfacial forces.
* **Prof. Stephen Neethling**: For supervision on sharpening the interface transition to reduce spurious currents.
* **Dr. Branko Bijeljic**: PhD supervisor.
* **Prof. Martin J. Blunt**: PhD supervisor.

---

### Contacts

For any queries, please contact:

* Ali Q. Raeini, email: a.q.raeini@gmail.com
* Mosayeb Shams, email: m.shams14@imperial.ac.uk

For more information and contact details, please visit the [Imperial College Consortium on Pore-scale Modelling and Imaging website](
https://www.imperial.ac.uk/earth-science/research/research-groups/pore-scale-modelling)

---

### License

[GPLv3](https://www.gnu.org/licenses/gpl-3.0.txt)
