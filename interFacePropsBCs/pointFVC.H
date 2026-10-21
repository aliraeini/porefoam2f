
#include "pointMesh.H"
#include "surfaceFields.H"
namespace Foam
{

	pointScalarField faceToPointInterpolate(const surfaceScalarField& alphaf, const pointMesh&  pMesh);


	pointScalarField faceToPointWeightedInterpolate(
			const surfaceScalarField& alphaf,
			const surfaceScalarField& delS,
			const pointMesh&          pMesh );

	void pointToFaceAverage(
			surfaceScalarField&     alphaf,
			const pointScalarField& alphaP,
			const pointMesh&        pMesh,
			scalar                  relaxFactor );

	pointVectorField pointInterpolate(const volVectorField& U, const pointMesh &  pMesh);

}
