# porefoam2f Module: Development Version

Porefoam2f is a code that simulates incompressible two-phase flow
in 3D images of porous media using the OpenFOAM finite-volume library.

---


> [!WARNING]
> This code is experimental and requires further development;
> see the [Technical Note](./UserGuide.md#technical-note) in the [User Guide](./UserGuide.md)

---

## Instructions

See [src/porefoam2f/UserGuide.md](./UserGuide.md#Installation) for instructions on how to **organise**, **download**, **build** and **run** the code and its dependencies.

The download and build instructions are same as in the **parrent [porescale](https://github.com/aliraeini/porescale) repository**.

You can run a sample simulation by following the commands executed by `make test` from the [src/porefoam2f/test2f](./test2f) directory.
The sample simulation run with `make test` shows a significant level of stick-slip behaviour,
and this test case shall be updated to minimise these artefacts.

**TODO**: Add another test case based on the star-shaped geometry from Raeini et al. (2014).

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
