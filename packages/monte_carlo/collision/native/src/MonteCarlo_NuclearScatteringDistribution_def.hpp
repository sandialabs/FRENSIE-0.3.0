//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_NuclearScatteringDistribution_def.hpp
//! \author Alex Robinson
//! \brief  The nuclear scattering distribution base class template definitions
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_NUCLEAR_SCATTERING_DISTRIBUTION_DEF_HPP
#define MONTE_CARLO_NUCLEAR_SCATTERING_DISTRIBUTION_DEF_HPP

// FRENSIE Includes
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Constructor
template<typename IncomingParticleType, typename OutgoingParticleType>
NuclearScatteringDistribution<IncomingParticleType,OutgoingParticleType>::NuclearScatteringDistribution( const double atomic_weight_ratio )
  : d_atomic_weight_ratio( atomic_weight_ratio )
{
  // Make sure the atomic weight ratio is valid
  testPrecondition( atomic_weight_ratio > 0.0 );
}

// Constructor
template<typename ParticleType>
NuclearScatteringDistribution<ParticleType,ParticleType>::NuclearScatteringDistribution( const double atomic_weight_ratio )
  : d_atomic_weight_ratio( atomic_weight_ratio )
{
  // Make sure the atomic weight ratio is valid
  testPrecondition( atomic_weight_ratio > 0.0 );
}

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_NUCLEAR_SCATTERING_DISTRIBUTION_DEF_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_NuclearScatteringDistribution_def.hpp
//---------------------------------------------------------------------------//
