/*-------------------------------------------------------------------------*\

 Least-square gradient at mesh points

 Copyright (C) 2014-2020  Mosayeb Shams

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
\*-------------------------------------------------------------------------*/



namespace Foam
{
	Foam::pointVectorField pointGardLeastSquare
	(
		const surfaceScalarField& alphaf,
		const pointScalarField alpha1P,
		const pointField points,
		const labelList& interfPoints
	)
	{
		const fvMesh& msh = alphaf.mesh();
		//const labelListList& pointCells = msh.pointCells();
		const pointMesh &  pMesh = alpha1P.mesh();

		pointVectorField gradAlphaP
		(
		IOobject  (  "gradAlphaP",  msh.time().timeName(),  msh  ),
			pMesh,
			dimensionedVector("gradAlphaP",dimless/dimLength,vector::zero)
		);


		const faceList &  faces=msh.faces();
		const surfaceVectorField & Cf=msh.Cf();
		const surfaceScalarField & magSf=msh.magSf();

		symmTensorField invGG(points.size(),symmTensor(1e-36,0.,0.,1e-36,0.,1e-36));

		forAll(Cf, fI) // if(magdS[fI]>1e-12)
		{
			vector Cpi = Cf[fI];
			scalar weight=magSf[fI];  weight*=weight;// *magdS[fI]
			const face& ff = faces[fI];
			forAll(ff, ip)
			{	label pI = ff[ip];
				vector pp = Cpi - points[pI];
				scalar weightpd2 = weight/magSqr(pp); //weightpd2*=weightpd2;
				invGG[pI] += weightpd2*sqr(pp);
			}
		}


		const fvBoundaryMesh& patches = msh.boundary();
		forAll(patches, bI)
		{
			label pStart=patches[bI].patch().start();
			const scalarField & pmagSf = magSf.boundaryField()[bI];
			const vectorField & Cfp = Cf.boundaryField()[bI];
			if (patches[bI].coupled())
			{
				forAll(Cfp, pfI) // if(pmagdS[pfI]>1e-12)
				{
					vector Cpi = Cfp[pfI];
					scalar weight=pmagSf[pfI];   weight*=weight*0.5; //*pmagdS[pfI]
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{	label pI = ff[ip];
						vector pp = Cpi - points[pI];
						scalar weightpd2 = weight/magSqr(pp); //weightpd2*=weightpd2;
						invGG[pI] += weightpd2*sqr(pp);
					}
				}
			}
			else
			{
				forAll(Cfp, pfI)  //if (pmagdS[pfI]>1e-8)
				{
					vector Cpi = Cfp[pfI];
					scalar weight=pmagSf[pfI];  weight*=weight;  //*pmagdS[pfI]
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{	label pI = ff[ip];
						vector pp = Cpi - points[pI];
						scalar weightpd2 = weight/magSqr(pp); //weightpd2*=weightpd2;
						invGG[pI] += weightpd2*sqr(pp);
					}
				}
			}
		}

		syncTools::syncPointList(msh, invGG, plusEqOp<symmTensor>(), pTraits<symmTensor>::zero);


		  forAll(points,pI)
		  {
				 if(interfPoints[pI])
					  invGG[pI] = inv(invGG[pI]);
					else
						invGG[pI] = symmTensor::zero;
		  }

		// TODO cache invGG

		forAll(alphaf, fI) // if(magdS[fI]>1e-12)
		{
			scalar weight=magSf[fI];  weight*=weight;// *magdS[fI]
			vector Cfi = Cf[fI];
			const face& ff = faces[fI];
			forAll(ff, ip)
			{	label pI = ff[ip];
				scalar weightpd2 = weight/magSqr(points[pI]-Cfi); //weightpd2*=weightpd2;
				gradAlphaP[pI] += weightpd2*(invGG[pI]&(Cfi - points[pI]))*(alphaf[fI] - alpha1P[pI]);
			}


		}


		forAll(patches, bI)
		{
			const label pStart=patches[bI].patch().start();
			const scalarField & pmagSf = magSf.boundaryField()[bI];
			const scalarField & alphafp = alphaf.boundaryField()[bI];
			const vectorField & Cfp = Cf.boundaryField()[bI];
			if (patches[bI].coupled())
			 {
				forAll(Cfp, pfI) // if(pmagdS[pfI]>1e-12)
				{
					scalar weight=pmagSf[pfI];weight*=weight*0.5;//*pmagdS[pfI]
					scalar alphafi=alphafp[pfI] ;
					vector Cfi = Cfp[pfI];
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{	label pI = ff[ip];
						scalar weightpd2 = weight/magSqr(points[pI]-Cfi); //weightpd2*=weightpd2;
						gradAlphaP[pI] += weightpd2*(invGG[pI]&(Cfi - points[pI]))*(alphafi - alpha1P[pI]);
					}
				}
			 }
			 else
			 {
				forAll(Cfp, pfI)  //if (pmagdS[pfI]>1e-8)
				{
					scalar weight=pmagSf[pfI]; weight*=weight;//*pmagdS[pfI]
					scalar alphafi=alphafp[pfI] ;
					vector Cfi = Cfp[pfI];
					const face& ff = faces[pStart+pfI];
					forAll(ff, ip)
					{	label pI = ff[ip];
						scalar weightpd2 = weight/magSqr(points[pI]-Cfi); //weightpd2*=weightpd2;
						gradAlphaP[pI] += weightpd2*(invGG[pI]&(Cfi - points[pI]))*(alphafi - alpha1P[pI]);
					}
				}
			 }
		  }

		syncTools::syncPointList(msh, gradAlphaP, plusEqOp<vector>(), pTraits<vector>::zero);





		  return gradAlphaP;
	 }





}
