//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_DetailedSubshellRelaxationModel.cpp
//! \author Alex Robinson
//! \brief  Detailed subshell relaxation model class definition.
//!
//---------------------------------------------------------------------------//

// FRENSIE Includes
#include "MonteCarlo_DetailedSubshellRelaxationModel.hpp"
#include "MonteCarlo_SimulationElectronProperties.hpp"
#include "MonteCarlo_SimulationPhotonProperties.hpp"
#include "MonteCarlo_PhotonState.hpp"
#include "MonteCarlo_ElectronState.hpp"
#include "Utility_DiscreteDistribution.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_PhysicalConstants.hpp"
#include "Utility_DirectionHelpers.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Constructor
DetailedSubshellRelaxationModel::DetailedSubshellRelaxationModel( 
       const SubshellType vacancy_subshell,
       const Teuchos::Array<SubshellType>& primary_transition_vacancy_shells,
       const Teuchos::Array<SubshellType>& secondary_transition_vacancy_shells,
       const Teuchos::Array<double>& outgoing_particle_energies,
       const Teuchos::Array<double>& transition_pdf_or_cdf,
       const bool interpret_as_cdf )
  : SubshellRelaxationModel( vacancy_subshell ),
    d_transition_distribution(),
    d_outgoing_particle_energies( outgoing_particle_energies ),
    d_transition_vacancy_shells( primary_transition_vacancy_shells.size() )
{
  // Make sure the vacancy subshell is valid
  testPrecondition( vacancy_subshell != INVALID_SUBSHELL );
  testPrecondition( vacancy_subshell != UNKNOWN_SUBSHELL );
  // Make sure the arrays are valid
  testPrecondition( primary_transition_vacancy_shells.size() > 0 );
  testPrecondition( secondary_transition_vacancy_shells.size() == 
		    primary_transition_vacancy_shells.size() );
  testPrecondition( outgoing_particle_energies.size() ==
		    primary_transition_vacancy_shells.size() );
  testPrecondition( transition_pdf_or_cdf.size() ==
		    primary_transition_vacancy_shells.size() );

  // Create the transition distribution
  Teuchos::Array<double> dummy_indep_values( transition_pdf_or_cdf.size() );
  
  d_transition_distribution.reset( new Utility::DiscreteDistribution(
						         dummy_indep_values,
						         transition_pdf_or_cdf,
							 interpret_as_cdf ) );

  // Store the transition vacancy shells
  for( unsigned i = 0; i < primary_transition_vacancy_shells.size(); ++i )
  {
    d_transition_vacancy_shells[i].first = 
      primary_transition_vacancy_shells[i];
    d_transition_vacancy_shells[i].second = 
      secondary_transition_vacancy_shells[i];
  }
}

// Relax the shell
/*! \details The particle must be passed so that its position, history number,
 * collision number and generation number can be copied. The relaxation
 * photon or electron that is created will be banked. The new primary vacancy
 * shell corresponds to the shell that the current vacancy moves to. The
 * new secondary vacancy shell corresponds to a new vacancy that is created
 * in the transition. In a radiative transition, the vacancy only moves - no
 * new vacancies are created (the new secondary vacancy shell will always
 * be set to INVALID_SUBSHELL). In a non-radiative the secondary vacancy shell 
 * corresponds to the shell where the Auger electron is actually emitted from.
 * \todo Determine whether incrementing the collision number and/or generation
 * number is appropriate.
 */ 
void DetailedSubshellRelaxationModel::relaxSubshell( 
			      const ParticleState& particle,
			      ParticleBank& bank,
			      SubshellType& new_primary_vacancy_shell,
		              SubshellType& new_secondary_vacancy_shell ) const
{
  // Sample the transition that occurs
  unsigned transition_index;
  
  d_transition_distribution->sampleAndRecordBinIndex( transition_index );
  
  double new_particle_energy = d_outgoing_particle_energies[transition_index];
   
  // Set the new vacancies shells
  new_primary_vacancy_shell = 
    d_transition_vacancy_shells[transition_index].first;
  
  new_secondary_vacancy_shell = 
    d_transition_vacancy_shells[transition_index].second;

  // A secondary transition will only occur with Auger electron emission
  if( new_secondary_vacancy_shell == INVALID_SUBSHELL ||
      new_secondary_vacancy_shell == UNKNOWN_SUBSHELL )
    this->generateFluorescencePhoton( particle, new_particle_energy, bank );
  else
    this->generateAugerElectron( particle, new_particle_energy, bank );
}

// Generate a fluorescence photon
void DetailedSubshellRelaxationModel::generateFluorescencePhoton( 
						const ParticleState& particle,
					        const double new_photon_energy,
						ParticleBank& bank ) const     
{
  // Make sure the new energy is valid
  testPrecondition( new_photon_energy > 0.0 );

  // Only generate the photon if it is above the cutoff energy
  if( new_photon_energy >= SimulationPhotonProperties::getMinPhotonEnergy() )
  {
    Teuchos::RCP<ParticleState> fluorescence_photon( 
				     new PhotonState( particle, true, true ) );

    // Set the new energy
    fluorescence_photon->setEnergy( new_photon_energy );

    double angle_cosine, azimuthal_angle;

    this->sampleEmissionDirection( angle_cosine, azimuthal_angle );

    // Set the new direction
    fluorescence_photon->rotateDirection( angle_cosine, azimuthal_angle );
  
    // Bank the relaxation particle
    bank.push( fluorescence_photon );
  }
}

// Generate an Auger electron
void DetailedSubshellRelaxationModel::generateAugerElectron( 
					      const ParticleState& particle,
					      const double new_electron_energy,
					      ParticleBank& bank ) const       
{
  // Make sure the new energy is valid - surprisingly, 0.0 can appear in the
  // table
  testPrecondition( new_electron_energy >= 0.0 );

  if( new_electron_energy >= SimulationElectronProperties::getMinElectronEnergy() )
  {
    Teuchos::RCP<ParticleState> auger_electron( 
				   new ElectronState( particle, true, true ) );

    // Set the new energy
    auger_electron->setEnergy( new_electron_energy );

    double angle_cosine, azimuthal_angle;

    this->sampleEmissionDirection( angle_cosine, azimuthal_angle );

    // Set the new direction
    auger_electron->rotateDirection( angle_cosine, azimuthal_angle );
  
    // Bank the relaxation particle
    bank.push( auger_electron );
  }
}

// Sample emission direction
void DetailedSubshellRelaxationModel::sampleEmissionDirection( 
					        double& angle_cosine,
						double& azimuthal_angle ) const
{
  // Sample an isotropic outgoing angle cosine for the relaxation particle
  angle_cosine = -1.0 + 
    2.0*Utility::RandomNumberGenerator::getRandomNumber<double>();
  
  // Sample the azimuthal angle
  azimuthal_angle = 2.0*Utility::PhysicalConstants::pi*
    Utility::RandomNumberGenerator::getRandomNumber<double>();

  // Make sure the scattering angle cosine is valid
  testPostcondition( angle_cosine >= -1.0 );
  testPostcondition( angle_cosine <= 1.0 );
  // Make sure the azimuthal angle is valid
  testPostcondition( azimuthal_angle >= 0.0 );
  testPostcondition( azimuthal_angle <= 2*Utility::PhysicalConstants::pi );
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_Detailed SubshellRelaxationModel.cpp
//---------------------------------------------------------------------------//
