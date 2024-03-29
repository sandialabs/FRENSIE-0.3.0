//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_AtomicExcitationElectronScatteringDistribution.cpp
//! \author Luke Kersting
//! \brief  The electron hard elastic scattering distribution definition
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <limits>

// Trilinos Includes
#include <Teuchos_Array.hpp>
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "MonteCarlo_AtomicExcitationElectronScatteringDistribution.hpp"
#include "Utility_OneDDistribution.hpp"

namespace MonteCarlo{

// Constructor
AtomicExcitationElectronScatteringDistribution::AtomicExcitationElectronScatteringDistribution(
    const AtomicDistribution& energy_loss_distribution )
    : d_energy_loss_distribution( energy_loss_distribution )
{
  // Make sure the array is valid
  testPrecondition( !d_energy_loss_distribution.is_null() );
}

// Sample an outgoing energy and direction from the distribution
void AtomicExcitationElectronScatteringDistribution::sample( 
             const double incoming_energy,
             double& outgoing_energy,
             double& scattering_angle_cosine ) const
{
  // Theree is no angle scattering
  scattering_angle_cosine = 1.0;

  // Get enery loss
  double energy_loss = d_energy_loss_distribution->evaluate( incoming_energy );

  // Calculate outgoing energy
  outgoing_energy = incoming_energy - energy_loss;
}

// Sample an outgoing energy and direction and record the number of trials
void AtomicExcitationElectronScatteringDistribution::sampleAndRecordTrials( 
             const double incoming_energy,
             double& outgoing_energy,
             double& scattering_angle_cosine,
             unsigned& trials ) const
{
  // Update trial number
  trials++;

  // Sample an outgoing direction
  this->sample( incoming_energy,
                outgoing_energy,
                scattering_angle_cosine ); 
}

// Randomly scatter the electron
void AtomicExcitationElectronScatteringDistribution::scatterElectron( ElectronState& electron,
                      ParticleBank& bank,
                      SubshellType& shell_of_interaction ) const
{
  double outgoing_energy;
  double scattering_angle_cosine;

  // Sample an outgoing energy
  this->sample( electron.getEnergy(),
                outgoing_energy,
                scattering_angle_cosine );
  
  // Set the new energy
  electron.setEnergy( outgoing_energy );
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_AtomicExcitationElectronScatteringDistribution.cpp
//---------------------------------------------------------------------------//
