//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_BremsstrahlungElectronScatteringDistribution.cpp
//! \author Luke Kersting
//! \brief  The electron bremsstrahlung scattering distribution definition
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <limits>

// Trilinos Includes
#include <Teuchos_Array.hpp>

// FRENSIE Includes
#include "MonteCarlo_BremsstrahlungElectronScatteringDistribution.hpp"
#include "MonteCarlo_TwoDDistributionHelpers.hpp"
#include "MonteCarlo_ElectronState.hpp"
#include "MonteCarlo_PhotonState.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_SearchAlgorithms.hpp"
#include "Utility_DirectionHelpers.hpp"
#include "Utility_KinematicHelpers.hpp"
#include "Utility_PhysicalConstants.hpp"
#include "Utility_TabularDistribution.hpp"

namespace MonteCarlo{
// Constructor with simple analytical photon angular distribution
BremsstrahlungElectronScatteringDistribution::BremsstrahlungElectronScatteringDistribution(
    const BremsstrahlungDistribution& bremsstrahlung_scattering_distribution )
  : d_bremsstrahlung_scattering_distribution( bremsstrahlung_scattering_distribution )
{
  // Make sure the array is valid
  testPrecondition( d_bremsstrahlung_scattering_distribution.size() > 0 );

  // Use simple analytical photon angular distribution
  d_angular_distribution_func = boost::bind<double>( 
           &BremsstrahlungElectronScatteringDistribution::SampleDipoleAngle,
           boost::cref( *this ),
           _1,
           _2 );
}

// Constructor with detailed tabular photon angular distribution
BremsstrahlungElectronScatteringDistribution::BremsstrahlungElectronScatteringDistribution(
    const BremsstrahlungDistribution& bremsstrahlung_scattering_distribution,
    const Teuchos::RCP<Utility::OneDDistribution>& angular_distribution,
    const double lower_cutoff_energy,
    const double upper_cutoff_energy )
  : d_bremsstrahlung_scattering_distribution( bremsstrahlung_scattering_distribution ),
    d_angular_distribution( angular_distribution ),
    d_lower_cutoff_energy( lower_cutoff_energy ),
    d_upper_cutoff_energy( upper_cutoff_energy )
{
  // Make sure the arraies are valid
  testPrecondition( d_bremsstrahlung_scattering_distribution.size() > 0 );
  testPrecondition( !d_angular_distribution.is_null() );

  // Use detailed photon angular distribution
  d_angular_distribution_func = boost::bind<double>( 
		    &BremsstrahlungElectronScatteringDistribution::SampleTabularAngle,
		    boost::cref( *this ),
		    _1,
            _2 );
}

// Constructor with detailed 2BS photon angular distribution
BremsstrahlungElectronScatteringDistribution::BremsstrahlungElectronScatteringDistribution(
  const BremsstrahlungDistribution& bremsstrahlung_scattering_distribution,
  const int atomic_number )
  : d_bremsstrahlung_scattering_distribution( bremsstrahlung_scattering_distribution ),
    d_atomic_number( atomic_number )
{
  // Make sure the arraies are valid
  testPrecondition( d_bremsstrahlung_scattering_distribution.size() > 0 );

  // Use detailed photon angular distribution
  d_angular_distribution_func = boost::bind<double>( 
		    &BremsstrahlungElectronScatteringDistribution::Sample2BSAngle,
		    boost::cref( *this ),
		    _1,
            _2 );
}


// Sample an outgoing energy and direction from the distribution
void BremsstrahlungElectronScatteringDistribution::sample( 
             const double incoming_energy,
             double& outgoing_energy,
             double& scattering_angle_cosine ) const
{
  // Electron angle scattering is assumed to be negligible
  scattering_angle_cosine = 1.0;

  outgoing_energy = incoming_energy - 
                    sampleTwoDDistributionCorrelated( 
                                     incoming_energy,
                                     d_bremsstrahlung_scattering_distribution );
}

// Sample an outgoing energy and direction and record the number of trials
void BremsstrahlungElectronScatteringDistribution::sampleAndRecordTrials( 
                            const double incoming_energy,
                            double& outgoing_energy,
                            double& scattering_angle_cosine,
                            unsigned& trials ) const
{
  trials++;

  sample( incoming_energy, outgoing_energy, scattering_angle_cosine );
  
}
// Randomly scatter the electron
void BremsstrahlungElectronScatteringDistribution::scatterElectron( 
                                ElectronState& electron,
                                ParticleBank& bank,
                                SubshellType& shell_of_interaction ) const
{
  // Incoming electron energy
  double incoming_energy = electron.getEnergy();

  // outgoing electron energy
  double outgoing_energy;

  // Scattering angle of the electron
  double scattering_angle_cosine;

  // energy of the bremsstrahlung photon
  double photon_energy;

  // photon outgoing angle cosine
  double photon_angle_cosine;

  // Sample outgoing electron energy
  sample( incoming_energy, outgoing_energy, scattering_angle_cosine );

  // Calculate the photon energy
  photon_energy = electron.getEnergy() - outgoing_energy;

  // Create new photon
  Teuchos::RCP<PhotonState> bremsstrahlung_photon( 
                           new PhotonState( electron, true, true ) );

  // Set photon energy
  bremsstrahlung_photon->setEnergy( photon_energy );

  // Sample the photon outgoing angle cosine
  photon_angle_cosine = d_angular_distribution_func( incoming_energy, 
                                                     photon_energy );

  bremsstrahlung_photon->rotateDirection( photon_angle_cosine,
			                  sampleAzimuthalAngle() );

  // Bank the photon
  bank.push( bremsstrahlung_photon );

  // Set the new electron energy
  electron.setEnergy( outgoing_energy );

  // Increment the electron generation number
  electron.incrementGenerationNumber();
}

// Sample the outgoing photon direction from the analytical function
double BremsstrahlungElectronScatteringDistribution::SampleDipoleAngle( 
                                               double& incoming_electron_energy, 
                                               double& photon_energy  ) const 
{
  // get the velocity of the electron divided by the speed of light beta = v/c
  double beta = sqrt ( Utility::calculateDimensionlessRelativisticSpeedSquared( 
                          Utility::PhysicalConstants::electron_rest_mass_energy,
                          incoming_electron_energy ) );

  double scaled_random_number = 
    2.0 * Utility::RandomNumberGenerator::getRandomNumber<double>();
  
  double parameter = -( 1.0 + beta );
                 
  return ( scaled_random_number        + parameter )/ 
         ( scaled_random_number * beta + parameter ); 
}

/* Sample the outgoing photon direction using the 2BS sampling routine of 
 * Kock and Motz
 */
double BremsstrahlungElectronScatteringDistribution::Sample2BSAngle( 
                                        double& incoming_electron_energy, 
                                        double& photon_energy ) const 
{
  double outgoing_electron_energy = incoming_electron_energy - photon_energy;
  double ratio = outgoing_electron_energy/incoming_electron_energy;
  double two_ratio = 2.0*ratio;
  double parameter = ( 1.0 + ratio*ratio );

  // calculate the minimum, mid, and maximum values of x
  double x_min = 0;
  double x_mid = 1.0;
  double x_max = Utility::PhysicalConstants::pi*Utility::PhysicalConstants::pi*
                 incoming_electron_energy*incoming_electron_energy;

  // Calculate the rejection function, g(x), for the minimum, mid, and maximum values of x
  double g_x_min = Calculate2BSRejection( outgoing_electron_energy, 
                                          two_ratio,
                                          parameter, 
                                          x_min );
  double g_x_mid = Calculate2BSRejection( outgoing_electron_energy, 
                                          two_ratio,
                                          parameter, 
                                          x_mid );
  double g_x_max = Calculate2BSRejection( outgoing_electron_energy, 
                                          two_ratio,
                                          parameter, 
                                          x_max );

  bool rejected = true;

  double rand1, rand;

  while( rejected )
  {
    rand = Utility::RandomNumberGenerator::getRandomNumber<double>();

    // sample for theta
    double theta = sqrt( rand/( 1.0 - rand + 1.0/x_max ) )/
                   incoming_electron_energy;
  
    double x = sqrt( incoming_electron_energy*theta );

    // Calculate the rejection function
    double g = Calculate2BSRejection( outgoing_electron_energy, 
                                      two_ratio,
                                      parameter, 
                                      x );

    // Normalized the rejection function
    g /= std::max( std::max( g_x_min, g_x_max ), g_x_mid );
 
    // Apply rejection scheme
    rand1 = Utility::RandomNumberGenerator::getRandomNumber<double>();

    if( rand1 < g )
    {
      return cos(theta);
    }
  }
}

// Calculate the rejection function for the 2BS sampling routine
double BremsstrahlungElectronScatteringDistribution::Calculate2BSRejection( 
                                               double& outgoing_electron_energy,
                                               double& two_ratio,
                                               double& parameter1,
                                               double& x ) const
{
  double parameter2 = ( 1.0 + x )*( 1.0 + x );

  double m = log( ( parameter1 - two_ratio )/
             ( 4.0*outgoing_electron_energy*outgoing_electron_energy ) +
             pow( d_atomic_number, 2.0/3.0)/( 12321*parameter2 ) );

  return 3.0*parameter1 - two_ratio - 
         ( 4.0 + m )*( parameter1 - 2.0*x*two_ratio/parameter2 );

}

// Sample the detailed outgoing photon direction
double BremsstrahlungElectronScatteringDistribution::SampleTabularAngle( 
                                               double& incoming_electron_energy, 
                                               double& photon_energy ) const
{
    if ( incoming_electron_energy > d_upper_cutoff_energy )
  {
    return SampleDipoleAngle( incoming_electron_energy, photon_energy );
  }
  else 
  {
    if ( incoming_electron_energy > d_lower_cutoff_energy )
    {
      return d_angular_distribution->evaluate( photon_energy );
    }
    else
    {
    return SampleDipoleAngle( incoming_electron_energy, photon_energy );
    } 
  }
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_BremsstrahlungElectronScatteringDistribution.cpp
//---------------------------------------------------------------------------//
