
#include "pointFVC.H"
#include "syncTools.H"
#include "volPointInterpolation.H"
#include "fvCFD.H"

namespace Foam
{
	pointScalarField faceToPointInterpolate(const surfaceScalarField& alphaf, const pointMesh&  pMesh)
	{
        const fvMesh& msh = alphaf.mesh();
		//const labelListList& pointCells = msh.pointCells();
		const pointField& points = msh.points();

		pointScalarField alphaP
		(
		  IOobject   ( "alphaP",  msh.time().timeName(),  msh ),
		 pMesh,  dimensionedScalar("alphaP",dimless,0.)
		);
		pointScalarField alphaPSumWeights
		(
		  IOobject  (  "alphaPSumWeights",  alphaf.time().timeName(),  msh  ),
		 pMesh, dimensionedScalar("alphaPSumWeights",dimless,1e-37)
		);


        //surfaceScalarField alphaf = fvc::interpolate(alpha1);
		const faceList &  faces=msh.faces();
		const surfaceVectorField & Cf=msh.Cf();




		forAll(alphaf, fI)
		{
			vector Cfi = Cf[fI];
			scalar alphafi = alphaf[fI];
			const face& ff = faces[fI];
			forAll(ff, ip)
				{
					label pI = ff[ip];
					scalar weight=mag((Cfi - points[pI]));  // so that bigger faces have bigger contributions!
					alphaPSumWeights[pI] += weight;
					alphaP[pI] += weight * alphafi;
				}
		}


		const fvBoundaryMesh& patches = msh.boundary();
		forAll(patches, bI)
		{
          label pStart=patches[bI].patch().start();
          const vectorField & Cfp = Cf.boundaryField()[bI];
          if (!patches[bI].coupled())
          {
            forAll(Cfp, pfI)  //if (pmagdS[pfI]>1e-8)
            {
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{
						//boundary point field is computed using boundary faces only
						label pI = ff[ip];
						alphaPSumWeights[pI] *= 1e-32;
						alphaP[pI] *= 1e-32;
					}
            }
          }
		}

		forAll(patches, bI)
		{
			label pStart=patches[bI].patch().start();
			const vectorField & Cfp = Cf.boundaryField()[bI];
			const scalarField & alphafp = alphaf.boundaryField()[bI];
			if (patches[bI].coupled())
			{
				forAll(Cfp, pfI)  //if (pmagdS[pfI]>1e-8)
				{
					vector Cfi = Cfp[pfI];
					scalar alphafi = alphafp[pfI];
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{
						label pI = ff[ip];
						scalar weight=mag((Cfi - points[pI]));// * 0.5;
						alphaPSumWeights[pI] += weight;
						alphaP[pI] += weight * alphafi;
					}
				}
			}
			else
			{
				forAll(Cfp, pfI)  //if (pmagdS[pfI]>1e-8)
				{
					vector Cfi = Cfp[pfI];
					scalar alphafi = alphafp[pfI];
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{
						label pI = ff[ip];
						scalar weight=0.0001*mag((Cfi - points[pI]));
						alphaPSumWeights[pI] += weight;
						alphaP[pI] += weight * alphafi;
					}
				}
			}
		}

		syncTools::syncPointList(msh, alphaPSumWeights, plusEqOp<scalar>(), 0.);
		syncTools::syncPointList(msh, alphaP, plusEqOp<scalar>(), 0.);


		return alphaP/alphaPSumWeights;
	}

	pointScalarField faceToPointWeightedInterpolate(
			const surfaceScalarField& alphaf,
			const surfaceScalarField& delS,
			const pointMesh&  pMesh)
	{
        const fvMesh& msh = alphaf.mesh();
		//const labelListList& pointCells = msh.pointCells();
		const pointField& points = msh.points();

		pointScalarField alphaP(
			IOobject("alphaP",  msh.time().timeName(), msh),
			pMesh, dimensionedScalar("alphaP", dimless,0.)
		);
		pointScalarField alphaPSumWeights(
			IOobject("alphaPSumWeights",  alphaf.time().timeName(),  msh),
			pMesh, dimensionedScalar("alphaPSumWeights",dimless,1e-37)
		);

		const faceList & faces=msh.faces();
		const surfaceVectorField & Cf=msh.Cf();


		forAll(alphaf, fI) {
			scalar weiD = mag(delS[fI]);
			if (weiD>1.0e-32) {
				vector Cfi = Cf[fI];
				scalar alphafi = alphaf[fI];
				const face& ff = faces[fI];
				forAll(ff, ip)
				{
					label pI = ff[ip];
					scalar weight=mag(Cfi - points[pI]) * weiD;  // so that bigger faces have bigger contributions!
					alphaPSumWeights[pI] += weight;
					alphaP[pI] += weight * alphafi;
				}
			}
		}

		const fvBoundaryMesh& patches = msh.boundary();
		forAll(patches, bI) {
			label pStart=patches[bI].patch().start();
			const vectorField & Cfp = Cf.boundaryField()[bI];
			if (!patches[bI].coupled()) {
			 forAll(Cfp, pfI) {
				const face& ff = faces[pStart+pfI];
				forAll(ff, ip) {
					//boundary point field is computed using boundary faces only
					label pI = ff[ip];
					alphaPSumWeights[pI] *= 1e-32;
					alphaP[pI] *= 1e-32;
				}
			 }
			}
		}

		forAll(patches, bI) {
			label pStart=patches[bI].patch().start();
			const vectorField & Cfp = Cf.boundaryField()[bI];
			const scalarField & alphafp = alphaf.boundaryField()[bI];
			const scalarField & delSp = delS.boundaryField()[bI];
			if (patches[bI].coupled()) {
				forAll(Cfp, pfI)  //if (pmagdS[pfI]>1e-8)
				{
					scalar weiD = mag(delSp[pfI]);// * 0.5;
					if (weiD>1.0e-32) {
						vector Cfi = Cfp[pfI];
						scalar alphafi = alphafp[pfI];
						const face& ff = faces[pStart+pfI];
						forAll(ff, ip)
						{
							label pI = ff[ip];
							scalar weight=mag((Cfi - points[pI])) * weiD;
							alphaPSumWeights[pI] += weight;
							alphaP[pI] += weight * alphafi;
						}
					}
				}
			}
			else {
				forAll(Cfp, pfI)
				{
					scalar weiD = mag(delSp[pfI]);
					if (weiD>1.0e-32) {
						vector Cfi = Cfp[pfI];
						scalar alphafi = alphafp[pfI];
						const face& ff = faces[pStart+pfI];
						forAll(ff, ip) {
							label pI = ff[ip];
							scalar weight=mag((Cfi - points[pI])) * weiD;
							alphaPSumWeights[pI] += weight;
							alphaP[pI] += weight * alphafi;
						}
					}
				}
			}
		}

		syncTools::syncPointList(msh, alphaPSumWeights, plusEqOp<scalar>(), 0.);
		syncTools::syncPointList(msh, alphaP, plusEqOp<scalar>(), 0.);


		return alphaP/alphaPSumWeights;
	}


	void pointToFaceAverage(
			surfaceScalarField& alphaf,
			const pointScalarField& alphaP,
			const pointMesh&  pMesh,
			scalar relaxFactor
		)
	{
        const fvMesh& msh = alphaf.mesh();
		const pointField& points = msh.points();


		const faceList &  faces=msh.faces();
		const surfaceVectorField & Cf=msh.Cf();


		forAll(alphaf, fI)
		{
			vector Cfi = Cf[fI];
			scalar sumAlpha = 0., sumWeight = 1.0e-32;
			const face& ff = faces[fI];
			forAll(ff, ip)
			{
				label pI = ff[ip];
				const scalar weight = mag((Cfi - points[pI]));  // so that bigger faces have bigger contributions!

				sumAlpha += alphaP[pI] * weight;
				sumWeight += weight; // mag(Cfi - points[pI]);
			}
			alphaf[fI] = (sumAlpha / sumWeight) * (1.-relaxFactor)  +  relaxFactor * alphaf[fI];
		}
	}

}
namespace Foam
{
    Foam::pointVectorField pointInterpolate
    (
        const volVectorField& U,
        const pointMesh &  pMesh
    )
    {
        const fvMesh& msh = U.mesh();
        //const labelListList& pointCells = msh.pointCells();
        const pointField& points = msh.points();

        pointVectorField UP
                        (
                          IOobject ( "UP",  U.time().timeName(), msh ),
                         pMesh,  dimensionedVector("UP",U.dimensions(),vector::zero)
                       );
        pointScalarField  UPSumWeights
                        (
                          IOobject  ( "UPSumWeights", U.time().timeName(), msh ),
                         pMesh,  dimensionedScalar("UPSumWeights",dimless,1e-37)
                       );


        surfaceVectorField Uf = fvc::interpolate(U);
        const faceList &  faces=msh.faces();
        const surfaceVectorField & Cf=msh.Cf();




        forAll(Uf, fI)
        {
            vector Cfi = Cf[fI];
            vector Ufi = Uf[fI];
            const face& ff = faces[fI];
            forAll(ff, ip)
				{
					label pI = ff[ip];
					scalar weight=mag((Cfi - points[pI]));
					UPSumWeights[pI] += weight;
					UP[pI] += weight * Ufi;
				}
        }


        const fvBoundaryMesh& patches = msh.boundary();
        forAll(patches, bI)
        {
          label pStart=patches[bI].patch().start();
          const vectorField & Cfp = Cf.boundaryField()[bI];
          const vectorField & Ufp = U.boundaryField()[bI];
          if (patches[bI].coupled())
          {
            forAll(Cfp, pfI)  //if (pmagdS[pfI]>1e-8)
            {
					vector Cfi = Cfp[pfI];
					vector Ufi = Ufp[pfI];
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{
						label pI = ff[ip];
						scalar weight=mag((Cfi - points[pI]));
						UPSumWeights[pI] += weight;
						UP[pI] += weight * Ufi;
					}
            }
          }
          else
          {
            forAll(Cfp, pfI)  //if (pmagdS[pfI]>1e-8)
            {
					vector Cfi = Cfp[pfI];
					vector Ufi = Ufp[pfI];
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{
						label pI = ff[ip];
						scalar weight=0.0001*mag((Cfi - points[pI]));
						UPSumWeights[pI] += weight;
						UP[pI] += weight * Ufi;
					}
            }
          }
        }

        syncTools::syncPointList(msh, UPSumWeights, plusEqOp<scalar>(), 0.);
        syncTools::syncPointList(msh, UP, plusEqOp<vector>(), vector::zero);


        return UP/UPSumWeights;
    }
}
