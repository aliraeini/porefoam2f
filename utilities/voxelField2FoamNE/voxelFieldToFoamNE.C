/*-------------------------------------------------------------------------*\
 Copyright (C) 2018-2020  Ali Qaseminejad Raeini

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


#include "fvCFD.H"

#include "argList.H"
#include "timeSelector.H"
#include "graph.H"
#include "mathematicalConstants.H"

#include "OFstream.H"

#include "voxelImageI.h"


using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:





int main(int argc, char *argv[])
{

    argList::validArgs.append("NEd");
    argList::validArgs.append("title");


#   include "setRootCase.H"


#   include "createTime.H"
#   include "createMesh.H"


	fileName extractionDir(args.argRead<word>(1));
	fileName imageBaseName(args.argRead<word>(2));


   Info<<endl<<"imageBaseName:    "<<imageBaseName<<endl;
   Info<<"extractionDir:    "<<extractionDir<<endl<<endl;






    VoxelImage vximage(extractionDir+"/"+imageBaseName+".mhd", readOpt::procAndConvert); //dummy

    int3 n =	vximage.size3();
    dbl3 xmin=vximage.X0(); // xmin*=1e-6;
    dbl3 dx=vximage.dx();   //dx*=1e-6;

	std::vector<word> fieldNames;

	fieldNames.push_back("VElems");
	fieldNames.push_back("VSlems");

	const fvBoundaryMesh& boundary = mesh.boundary();


	for(size_t ifn=0;ifn<fieldNames.size();++ifn)
	{


		volScalarField p
		(
			IOobject
			(
				fieldNames[ifn],
				runTime.timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			mesh,dimensionedScalar(fieldNames[ifn],dimless,0.)
		);



		VoxelField<int> PdirVoxelField(n[0]+2,n[1]+2,n[2]+2,0.);
		//Info<<PdirVoxelField.size()<<endl;

		const vectorField & C =	mesh.C().internalField();
		if (PdirVoxelField.readBin(runTime.path()+"/"+extractionDir+"/"+imageBaseName+"_"+fieldNames[ifn]+imgExt()))
		{
		  forAll(C,c)
		  {
			int i=(C[c][0]-xmin[0])/dx[0];
			int j=(C[c][1]-xmin[1])/dx[1];
			int k=(C[c][2]-xmin[2])/dx[2];
			//Info<<i<<" "<<j<<" "<<k<<endl;
			p[c]=PdirVoxelField(i+1,j+1,k+1);
		  }
		}
		else Info<<endl<<"can not read "<<runTime.path()+"/"+extractionDir+"/"+imageBaseName+"_"+fieldNames[ifn]+imgExt()<<endl<<endl<<endl;

		forAll(boundary, bi)
		{
			p.boundaryFieldRef()[bi]==p.boundaryField()[bi].patchInternalField();
		}


		OFstream POF(p.time().timePath()+"/"+fieldNames[ifn]);
		p.writeHeader(POF);
		p.writeData(POF);

		Info<<fieldNames[ifn]<<" ranges from "<<min(p).value()<<" to "<<max(p).value()<<endl;


	}
	Info<<label(fieldNames.size())<<" int fields written"<<endl;

	//radius
	{


		volScalarField p
		(
			IOobject
			(
				"radius",
				runTime.timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			mesh,dimensionedScalar("radius",dimless,0.), "fixedValue"
		);


		VoxelField<float> PdirVoxelField(n[0],n[1],n[2],0.);

		const vectorField & C =	mesh.C().internalField();
		if (PdirVoxelField.readBin(runTime.path()+"/"+extractionDir+"/"+imageBaseName+"_radius"+imgExt()))
		{
		  forAll(C,c)
		  {
			int i=max((C[c][0]-xmin[0])/dx[0],0.5);
			int j=max((C[c][1]-xmin[1])/dx[1],0.5);
			int k=max((C[c][2]-xmin[2])/dx[2],0.5);
			p[c]=PdirVoxelField(i,j,k);
		  }
		}


		label iGrainwalls = mesh.boundaryMesh().findPatchID("Grainwalls");
		if (iGrainwalls < 0)
		{
			Info	<< "Unable to find  patch Grainwalls" << nl	<< endl;
		}

		forAll(boundary, bi)
			if (bi == iGrainwalls)			p.boundaryFieldRef()[bi]==0.;
			else
				p.boundaryFieldRef()[bi]==p.boundaryField()[bi].patchInternalField();



		OFstream POF(runTime.timePath()+"/radius");
		p.writeHeader(POF);
		p.writeData(POF);

		Info<<"radius"<<" ranges from "<<max(p.internalField())<<" to "<<max(p.internalField())<<endl;

		std::cout<<" 45 "<<std::endl;

	}


    Info<< "end" << endl;

    return 0;
}


// ************************************************************************* //
