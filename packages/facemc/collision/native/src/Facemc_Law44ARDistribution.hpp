//---------------------------------------------------------------------------//
//!
//! \file   Facemc_Law44ARDistribution.hpp
//! \author Alex Robinson
//! \brief  The law 44 AR distribution base class decl.
//!
//---------------------------------------------------------------------------//

#ifndef FACEMC_LAW_44_AR_DISTRIBUTION_HPP
#define FACEMC_LAW_44_AR_DISTRIBUTION_HPP

// Trilinos Includes
#include <Teuchos_ArrayRCP.hpp>
#include <Teuchos_Array.hpp>
#include <Teuchos_ArrayView.hpp>

// FRENSIE Includes
#include "Utility_ContractException.hpp"

namespace Facemc{

//! The law 44 AR distribution base class
class Law44ARDistribution
{

public:

  //! Constructor
  Law44ARDistribution( 
		  const Teuchos::ArrayView<const double>& outgoing_energy_grid,
		  const Teuchos::ArrayView<const double>& A_array,
		  const Teuchos::ArrayView<const double>& R_array );

  //! Destructor
  virtual ~Law44ARDistribution()
  { /* ... */ }

  //! Sample A and R from the distribution
  virtual void sampleAR( const unsigned outgoing_index,
			 const double energy_prime,
			 double& sampled_A,
			 double& sampled_R ) const = 0;

protected:

  //! Return the outgoing energy grid
  double getOutgoingEnergyGridPoint( const unsigned outgoing_index ) const;

  //! Return the A value
  double getAValue( const unsigned outgoing_index ) const;

  //! Return the R value
  double getRValue( const unsigned outgoing_index ) const;

private:

  // The outgoing energy grid
  Teuchos::Array<double> d_outgoing_energy_grid;
  
  // The A array
  Teuchos::Array<double> d_A;

  // The R array
  Teuchos::Array<double> d_R;
};

// Return the outgoing energy grid
inline double Law44ARDistribution::getOutgoingEnergyGridPoint( 
				         const unsigned outgoing_index ) const
{
  // Make sure the outgoing index is valid
  testPrecondition( outgoing_index < d_outgoing_energy_grid.size() );
  
  return d_outgoing_energy_grid[outgoing_index];
}

// Return the A value
inline double Law44ARDistribution::getAValue( const unsigned outgoing_index ) const
{
  // Make sure the outgoing index is valid
  testPrecondition( outgoing_index < d_A.size() );
  
  return d_A[outgoing_index];
}

// Return the R value
inline double Law44ARDistribution::getRValue( const unsigned outgoing_index ) const
{
  // Make sure the outgoing index is valid
  testPrecondition( outgoing_index < d_R.size() );

  return d_R[outgoing_index];
}

} // end Facemc namespace

#endif // end FACEMC_LAW_44_AR_DISTRIBUTION_HPP

//---------------------------------------------------------------------------//
// end Facemc_Law44ARDistribution.hpp
//---------------------------------------------------------------------------//
