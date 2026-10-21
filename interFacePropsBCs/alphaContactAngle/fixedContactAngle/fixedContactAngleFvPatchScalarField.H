/*---------------------------------------------------------------------------*\

License GPLv3

Class
    Foam::fixedContactAngleFvPatchScalarField

Description
    A spatially non-uniform alphaContactAngle boundary condition
    (alphaContactAngleFvPatchScalarField)

SourceFiles
    fixedContactAngleFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef fixedContactAngleFvPatchScalarField_H
#define fixedContactAngleFvPatchScalarField_H

#include "alphaContactAngleFvPatchScalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class fixedContactAngleFvPatch Declaration
\*---------------------------------------------------------------------------*/

class fixedContactAngleFvPatchScalarField
:
    public alphaContactAngleFvPatchScalarField
{
	// Private data

		//- Equilibrium contact angle
		scalarField theta_;

public:

	//- Runtime type information
	TypeName("fixedContactAngle");


	// Constructors

		//- Construct from patch and internal field
		fixedContactAngleFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct from patch, internal field and dictionary
		fixedContactAngleFvPatchScalarField
		(
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const dictionary&
		);

		//- Construct by mapping given
		//  fixedContactAngleFvPatchScalarField
		//  onto a new patch
		fixedContactAngleFvPatchScalarField
		(
			const fixedContactAngleFvPatchScalarField&,
			const fvPatch&,
			const DimensionedField<scalar, volMesh>&,
			const fvPatchFieldMapper&
		);

		//- Construct as copy
		fixedContactAngleFvPatchScalarField
		(
			const fixedContactAngleFvPatchScalarField&
		);

		//- Construct and return a clone
		virtual tmp<fvPatchScalarField> clone() const
		{
			return tmp<fvPatchScalarField>
			(
				 new fixedContactAngleFvPatchScalarField(*this)
			);
		}

		//- Construct as copy setting internal field reference
		fixedContactAngleFvPatchScalarField
		(
			const fixedContactAngleFvPatchScalarField&,
			const DimensionedField<scalar, volMesh>&
		);

		//- Construct and return a clone setting internal field reference
		virtual tmp<fvPatchScalarField> clone
		(
			const DimensionedField<scalar, volMesh>& iF
		) const
		{
			return tmp<fvPatchScalarField>
			(
				 new fixedContactAngleFvPatchScalarField(*this, iF)
			);
		}


  // Member functions

		//- Evaluate and return dynamic contact-angle
		virtual tmp<scalarField> theta
		(
			const vectorField& Up,
			const vectorField & nHat,
			const vectorField & nWall
		) const;

		//- Set function for theta
		scalarField& thetaRef() { return theta_; }

		//- Write
		virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
