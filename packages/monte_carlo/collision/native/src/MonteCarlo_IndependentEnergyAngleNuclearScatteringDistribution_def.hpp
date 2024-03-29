//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_IndependentEnergyAngleNuclearScatteringDistribution_def.hpp
//! \author Alex Robinson, Alex Bennett
//! \brief  The independent energy-angle scattering distribution class def.
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_INDEPENDENT_ENERGY_ANGLE_NUCLEAR_SCATTERING_DISTRIBUTION_DEF_HPP
#define MONTE_CARLO_INDEPENDENT_ENERGY_ANGLE_NUCLEAR_SCATTERING_DISTRIBUTION_DEF_HPP

// FRENSIE Includes
#include "Utility_DirectionHelpers.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Constructor
template<typename IncomingParticleType,
	 typename OutgoingParticleType,
	 typename SystemConversionPolicy>
IndependentEnergyAngleNuclearScatteringDistribution<IncomingParticleType,
						    OutgoingParticleType,
						    SystemConversionPolicy>::IndependentEnergyAngleNuclearScatteringDistribution( 
		     const double atomic_weight_ratio,
		     const Teuchos::RCP<NuclearScatteringEnergyDistribution>&
		     energy_scattering_distribution,
		     const Teuchos::RCP<NuclearScatteringAngularDistribution>&
		     angular_scattering_distribution )
  : NuclearScatteringDistribution<IncomingParticleType,OutgoingParticleType>( atomic_weight_ratio ),
    d_energy_scattering_distribution( energy_scattering_distribution ),
    d_angular_scattering_distribution( angular_scattering_distribution )
{
  // Check to make sure the energy distribution pointer is valid
  testPrecondition( !energy_scattering_distribution.is_null() );
    // Check to make sure the angular distribution pointer is valid
  testPrecondition( !angular_scattering_distribution.is_null() );
}

// Randomly scatter the neutron
template<typename IncomingParticleType,
	 typename OutgoingParticleType,
	 typename SystemConversionPolicy>
void 
IndependentEnergyAngleNuclearScatteringDistribution<IncomingParticleType,
						    OutgoingParticleType,
						    SystemConversionPolicy>::scatterParticle(
			        const IncomingParticleType& incoming_particle,
				OutgoingParticleType& outgoing_particle,
				const double temperature ) const
{
  double outgoing_sys_energy = 
    d_energy_scattering_distribution->sampleEnergy( 
					       incoming_particle.getEnergy() );

  double sys_scattering_angle_cosine = 
    d_angular_scattering_distribution->sampleAngleCosine( 
					       incoming_particle.getEnergy() );

  // convert the outgoing energy from this system to the lab system
  double outgoing_energy = 
    SystemConversionPolicy::convertToLabEnergy( incoming_particle.getEnergy(),
  						outgoing_sys_energy,
  						sys_scattering_angle_cosine,
  						this->getAtomicWeightRatio() );

  // convert the scattering angle cosine from this system to the lab system
  double scattering_angle_cosine = 
    SystemConversionPolicy::convertToLabAngleCosine( 
  						incoming_particle.getEnergy(),
  						outgoing_sys_energy,
  						outgoing_energy,
  						sys_scattering_angle_cosine,
  						this->getAtomicWeightRatio() );
  
  // Make sure the scattering angle cosine is valid
  testPostcondition( scattering_angle_cosine >= -1.0 );
  testPostcondition( scattering_angle_cosine <= 1.0 );

  // Set the new direction
  outgoing_particle.rotateDirection( scattering_angle_cosine,
				     this->sampleAzimuthalAngle() );

  // Set the new energy
  outgoing_particle.setEnergy( outgoing_energy );
}

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_INDEPENDENT_ENERGY_ANGLE_NUCLEAR_SCATTERING_DISTRIBUTION_DEF_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_IndependentEnergyAngleNuclearScatteringDistribution_def.hpp
//---------------------------------------------------------------------------//
