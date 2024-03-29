//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_HardElasticElectronScatteringDistribution.cpp
//! \author Luke Kersting
//! \brief  The electron hard elastic scattering distribution definition
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <limits>

// Trilinos Includes
#include <Teuchos_Array.hpp>

// FRENSIE Includes
#include "MonteCarlo_HardElasticElectronScatteringDistribution.hpp"
#include "MonteCarlo_TwoDDistributionHelpers.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_SearchAlgorithms.hpp"
#include "Utility_DirectionHelpers.hpp"
#include "Utility_KinematicHelpers.hpp"
#include "Utility_PhysicalConstants.hpp"
#include "Utility_TabularDistribution.hpp"
#include "Utility_ElasticElectronDistribution.hpp"

namespace MonteCarlo{

// Initialize static member data

// cutoff angle cosine for analytical peak
double HardElasticElectronScatteringDistribution::s_rutherford_cutoff = 0.999999;

// Difference btw cutoff angle cosine for analytical peak and foward peak (mu=1)
double HardElasticElectronScatteringDistribution::s_delta_cutoff = 1.0e-6;
  //1.0 - s_rutherford_cutoff;

// The fine structure constant squared
double HardElasticElectronScatteringDistribution::s_fine_structure_const_squared=
        Utility::PhysicalConstants::fine_structure_constant *
        Utility::PhysicalConstants::fine_structure_constant;

// A parameter for moliere's atomic screening constant  (1/2*(fsc/0.885)**2)
double HardElasticElectronScatteringDistribution::s_screening_param1 = 
      HardElasticElectronScatteringDistribution::s_fine_structure_const_squared/
      ( 2.0*0.885*0.885 );

// Constructor 
HardElasticElectronScatteringDistribution::HardElasticElectronScatteringDistribution(
    const int atomic_number,
    const ElasticDistribution& elastic_scattering_distribution)
  : d_atomic_number( atomic_number ),
    d_Z_two_thirds_power( pow( atomic_number, 2.0/3.0 ) ),
    d_screening_param2( 3.76*s_fine_structure_const_squared*
                              d_atomic_number*d_atomic_number ),
    d_elastic_scattering_distribution( elastic_scattering_distribution )
{
  // Make sure the array is valid
  testPrecondition( d_elastic_scattering_distribution.size() > 0 );
}

// Evaluate the PDF
double HardElasticElectronScatteringDistribution::evaluatePDF( 
                            const double incoming_energy,
                            const double scattering_angle_cosine ) const
{
  // Make sure the energy and angle are valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( scattering_angle_cosine >= -1.0 );
  testPrecondition( scattering_angle_cosine <= 1.0 );

  return MonteCarlo::evaluateTwoDDistributionCorrelatedPDF( 
                         incoming_energy,
                         scattering_angle_cosine,
                         d_elastic_scattering_distribution );
}

// Evaluate the PDF
double HardElasticElectronScatteringDistribution::evaluatePDF( 
                            const unsigned incoming_energy_bin,
                            const double scattering_angle_cosine ) const
{
  // Make sure the energy and angle are valid
  testPrecondition( incoming_energy_bin < 
                    d_elastic_scattering_distribution.size() );
  testPrecondition( incoming_energy_bin >= 0 );
  testPrecondition( scattering_angle_cosine >= -1.0 );
  testPrecondition( scattering_angle_cosine <= 1.0 );

  double pdf = 
    d_elastic_scattering_distribution[incoming_energy_bin].second->evaluatePDF( 
        scattering_angle_cosine );

  return pdf;
}

// Evaluate the CDF
double HardElasticElectronScatteringDistribution::evaluateCDF( 
                            const double incoming_energy,
                            const double scattering_angle_cosine ) const
{
  // Make sure the energy and angle are valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( scattering_angle_cosine >= -1.0 );
  testPrecondition( scattering_angle_cosine <= 1.0 );

  return MonteCarlo::evaluateTwoDDistributionCorrelatedCDF( 
                         incoming_energy,
                         scattering_angle_cosine,
                         d_elastic_scattering_distribution );
}

// Evaluate the screened Rutherford PDF
double HardElasticElectronScatteringDistribution::evaluateScreenedRutherfordPDF( 
                            const double incoming_energy,
                            const long double scattering_angle_cosine ) const
{
  // Make sure the energy and angle are valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( scattering_angle_cosine >= s_rutherford_cutoff );
  testPrecondition( scattering_angle_cosine <= 1.0 );

  long double cutoff_pdf_value = 
        evaluatePDF( incoming_energy, s_rutherford_cutoff );

  long double screening_constant = 
        evaluateMoliereScreeningConstant( incoming_energy );

  long double delta_cosine = 1.0L - scattering_angle_cosine;

  long double ratio = ( screening_constant + 1.0e-6L )/
                      ( screening_constant + delta_cosine );

  long double ratio_squared = ratio*ratio;

  return cutoff_pdf_value*ratio_squared;
}

// Return the energy at a given energy bin
double HardElasticElectronScatteringDistribution::getEnergy( 
    const unsigned energy_bin ) const
{
  // Make sure the energy bin is valid
  testPrecondition( energy_bin < d_elastic_scattering_distribution.size() );
  testPrecondition( energy_bin >= 0 );

  return d_elastic_scattering_distribution[energy_bin].first;
}

// Evaluate the screened Rutherford cross section ratio above the cutoff mu
double HardElasticElectronScatteringDistribution::evaluateScreenedRutherfordCrossSectionRatio( 
                                  const double incoming_energy ) const
{
  // Make sure the energy and angle are valid
  testPrecondition( incoming_energy > 0.0 );

  // get Moliere's atomic screening constant
  double eta = evaluateMoliereScreeningConstant( incoming_energy );

  // get the PDF value at the incoming energy and cutoff mu
  double pdf_value = evaluatePDF( incoming_energy, s_rutherford_cutoff );

  return s_delta_cutoff*( s_delta_cutoff + eta )*pdf_value/eta;
}

// Sample an outgoing energy and direction from the distribution
void HardElasticElectronScatteringDistribution::sample( 
				     const double incoming_energy,
				     double& outgoing_energy,
				     double& scattering_angle_cosine ) const
{
  // The outgoing energy is always equal to the incoming energy
  outgoing_energy = incoming_energy;

  unsigned trial_dummy;

  // Sample an outgoing direction
  this->sampleAndRecordTrialsImpl( incoming_energy,
				   scattering_angle_cosine,
				   trial_dummy );
}

// Sample an outgoing energy and direction and record the number of trials
void HardElasticElectronScatteringDistribution::sampleAndRecordTrials( 
					    const double incoming_energy,
					    double& outgoing_energy,
					    double& scattering_angle_cosine,
					    unsigned& trials ) const
{
  // The outgoing energy is always equal to the incoming energy
  outgoing_energy = incoming_energy;
  
  // Sample an outgoing direction
  this->sampleAndRecordTrialsImpl( incoming_energy,
				   scattering_angle_cosine,
				   trials );
}

// Randomly scatter the electron
void HardElasticElectronScatteringDistribution::scatterElectron( 
				     ElectronState& electron,
				     ParticleBank& bank,
				     SubshellType& shell_of_interaction ) const
{
  double scattering_angle_cosine;

  unsigned trial_dummy;

  // Sample an outgoing direction
  this->sampleAndRecordTrialsImpl( electron.getEnergy(),
				                   scattering_angle_cosine,
				                   trial_dummy );

  shell_of_interaction = UNKNOWN_SUBSHELL;

  // Set the new direction
  electron.rotateDirection( scattering_angle_cosine, 
			  this->sampleAzimuthalAngle() );
}

// Randomly scatter the adjoint electron
void HardElasticElectronScatteringDistribution::scatterAdjointElectron( 
				     AdjointElectronState& adjoint_electron,
				     ParticleBank& bank,
				     SubshellType& shell_of_interaction ) const
{
  double scattering_angle_cosine;

  unsigned trial_dummy;

  // Sample an outgoing direction
  this->sampleAndRecordTrialsImpl( adjoint_electron.getEnergy(),
				                   scattering_angle_cosine,
				                   trial_dummy );
  
  shell_of_interaction = UNKNOWN_SUBSHELL;

  // Set the new direction
  adjoint_electron.rotateDirection( scattering_angle_cosine, 
				                    this->sampleAzimuthalAngle() );
}

// Evaluate Moliere's atomic screening constant (modified by Seltzer) at the given electron energy
double HardElasticElectronScatteringDistribution::evaluateMoliereScreeningConstant(
                                              const double energy ) const
{
  // get the momentum**2 of the electron in units of electron_rest_mass_energy
  double electron_momentum_squared = 
           Utility::calculateDimensionlessRelativisticMomentumSquared( 
                          Utility::PhysicalConstants::electron_rest_mass_energy,
                          energy );

  // get the velocity of the electron divided by the speed of light beta = v/c
  double beta_squared = Utility::calculateDimensionlessRelativisticSpeedSquared( 
           Utility::PhysicalConstants::electron_rest_mass_energy,
           energy );

  double screening_param3 = 1.0/beta_squared*sqrt( energy/
            ( energy + Utility::PhysicalConstants::electron_rest_mass_energy) );

 // Calculate Moliere's atomic screening constant
 return s_screening_param1 * 1.0/electron_momentum_squared * 
        d_Z_two_thirds_power * ( 1.13 + d_screening_param2*screening_param3 );
}


// Evaluate the scattering angle from the analytical screend Rutherford function
double HardElasticElectronScatteringDistribution::evaluateScreenedScatteringAngle(
                                                  const double energy ) const 
{
  double random_number = 
                      Utility::RandomNumberGenerator::getRandomNumber<double>();
    
  // evaluate the screening angle at the given electron energy
  double screening_constant = evaluateMoliereScreeningConstant( energy );

  // Calculate the screened scattering angle
  double arg = random_number*s_delta_cutoff;

  return ( screening_constant*s_rutherford_cutoff + 
           arg*( screening_constant + 1.0 ) ) /
         ( screening_constant + arg );
}


// Sample an outgoing direction from the distribution
void HardElasticElectronScatteringDistribution::sampleAndRecordTrialsImpl( 
                                                const double incoming_energy,
                                                double& scattering_angle_cosine,
                                                unsigned& trials ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy > 0.0 );

  // Increment the number of trials
  ++trials;

  // The cutoff CDF for applying the analytical screening function
  double cutoff_cdf_value;

  double random_number;

  // Energy is below the lowest grid point
  if( incoming_energy < d_elastic_scattering_distribution.front().first )
  {
    cutoff_cdf_value = 
      d_elastic_scattering_distribution.front().second->evaluateCDF( 
                                                    s_rutherford_cutoff );

    random_number = Utility::RandomNumberGenerator::getRandomNumber<double>();

    // Sample from the lower energy value of the distribution
    if( cutoff_cdf_value > random_number )
    {
      scattering_angle_cosine = 
        d_elastic_scattering_distribution.front().second->sampleInSubrange( 
						       s_rutherford_cutoff );
    }
    // Sample from the analytical screend Rutherford function
    else
      scattering_angle_cosine = evaluateScreenedScatteringAngle( 
                                                              incoming_energy );
  }
  // Energy is above the highest grid point
  else if( incoming_energy >= d_elastic_scattering_distribution.back().first )
  {
    cutoff_cdf_value = 
      d_elastic_scattering_distribution.back().second->evaluateCDF( 
                                                    s_rutherford_cutoff );

    random_number = Utility::RandomNumberGenerator::getRandomNumber<double>();

    // Sample from the upper energy value of the distribution
    if( cutoff_cdf_value > random_number )
    {
      scattering_angle_cosine = 
        d_elastic_scattering_distribution.back().second->sampleInSubrange( 
						       s_rutherford_cutoff );
    }
    // Sample from the analytical screend Rutherford function
    else
      scattering_angle_cosine = evaluateScreenedScatteringAngle( 
                                                              incoming_energy );
  }
  // Energy is inbetween two grid point
  else
  {
    ElasticDistribution::const_iterator lower_dist_boundary, 
                                        upper_dist_boundary;
    double interpolation_fraction;

    // Find upper and lower bin and calculate the interpolation fraction
    findLowerAndUpperBinBoundary( incoming_energy,
                                  d_elastic_scattering_distribution,
                                  lower_dist_boundary,
                                  upper_dist_boundary,
                                  interpolation_fraction );

    // evaluate the cutoff CDF for applying the analytical screening function
    cutoff_cdf_value = evaluateCorrelatedCDF( upper_dist_boundary->second,
                                              lower_dist_boundary->second,
                                              interpolation_fraction,
                                              s_rutherford_cutoff );

    double cutoff_pdf_value;
    // evaluate the cutoff PDF for applying the analytical screening function
    cutoff_pdf_value = evaluateCorrelatedPDF( upper_dist_boundary->second,
                                              lower_dist_boundary->second,
                                              interpolation_fraction,
                                              s_rutherford_cutoff );

    // evaluate the screening angle at the given electron energy
    double screening_constant = 
            evaluateMoliereScreeningConstant( incoming_energy );
/*
    double analytical_cdf = cutoff_pdf_value/screening_constant*
     ( s_delta_cutoff + screening_constant )*( s_delta_cutoff );

    double alternative_cutoff_cdf = cutoff_cdf_value/( cutoff_cdf_value + analytical_cdf );
*/  
    double random_number = 
      Utility::RandomNumberGenerator::getRandomNumber<double>();

    // Correlated sample from the distribution
    if( cutoff_cdf_value > random_number )
    {
    scattering_angle_cosine = 
                    correlatedSampleInSubrange( upper_dist_boundary->second,
                                                lower_dist_boundary->second,
                                                interpolation_fraction,
                                                s_rutherford_cutoff );
    }
    // Sample from the analytical screend Rutherford function
    else
    {
      scattering_angle_cosine = evaluateScreenedScatteringAngle( 
                                                    incoming_energy );
    }
  }

  // Make sure the scattering angle cosine is valid
  testPostcondition( scattering_angle_cosine >= -1.0 );
  testPostcondition( scattering_angle_cosine <= 1.0 );
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_HardElasticElectronScatteringDistribution.cpp
//---------------------------------------------------------------------------//
