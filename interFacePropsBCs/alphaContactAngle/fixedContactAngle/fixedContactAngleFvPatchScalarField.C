
#include "fvPatchFields.H"
#include "fixedContactAngleFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volMesh.H"

#ifdef FOAMX
#include "foamTime.H"
#else
# include "Time.H"
#endif

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fixedContactAngleFvPatchScalarField::
fixedContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    alphaContactAngleFvPatchScalarField(p, iF),
    theta_(p.size(), scalar(90.))
{}


Foam::fixedContactAngleFvPatchScalarField::
fixedContactAngleFvPatchScalarField
(
    const fixedContactAngleFvPatchScalarField& gcpsf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    alphaContactAngleFvPatchScalarField(gcpsf, p, iF, mapper),
    theta_(gcpsf.theta_)
{}


Foam::fixedContactAngleFvPatchScalarField::
fixedContactAngleFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
	alphaContactAngleFvPatchScalarField(p, iF, dict),
	theta_("theta", dict, p.size())
{
	evaluate();
}


Foam::fixedContactAngleFvPatchScalarField::
fixedContactAngleFvPatchScalarField
(
    const fixedContactAngleFvPatchScalarField& gcpsf
)
:
    alphaContactAngleFvPatchScalarField(gcpsf),
    theta_(gcpsf.theta_)
{}


Foam::fixedContactAngleFvPatchScalarField::
fixedContactAngleFvPatchScalarField
(
    const fixedContactAngleFvPatchScalarField& gcpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    alphaContactAngleFvPatchScalarField(gcpsf, iF),
    theta_(gcpsf.theta_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::tmp<Foam::scalarField>
Foam::fixedContactAngleFvPatchScalarField::theta
(
    const vectorField& Up,
    const vectorField & nHat,
    const vectorField & nWall
) const
{
    return tmp<scalarField>(new scalarField(theta_));
}


void Foam::fixedContactAngleFvPatchScalarField::write(Ostream& os) const
{
    alphaContactAngleFvPatchScalarField::write(os);
    theta_.writeEntry("theta", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        fixedContactAngleFvPatchScalarField
    );
}


// ************************************************************************* //
