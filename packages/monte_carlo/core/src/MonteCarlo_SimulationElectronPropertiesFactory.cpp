//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_SimulationElectronPropertiesFactory.cpp
//! \author Alex Robinson, Luke Kersting
//! \brief  Simulation electron properties factory class definition.
//!
//---------------------------------------------------------------------------//

// FRENSIE Includes
#include "MonteCarlo_BremsstrahlungAngularDistributionType.hpp"
#include "MonteCarlo_SimulationElectronPropertiesFactory.hpp"
#include "MonteCarlo_SimulationElectronProperties.hpp"
#include "Utility_ExceptionTestMacros.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

//! Initialize the simulation properties
void SimulationElectronPropertiesFactory::initializeSimulationElectronProperties( 
				    const Teuchos::ParameterList& properties )
{  
  // Get the min electron energy - optional
  if( properties.isParameter( "Min Electron Energy" ) )
  {
    double min_energy = properties.get<double>( "Min Electron Energy" );

    if( min_energy >= SimulationElectronProperties::getAbsoluteMinElectronEnergy() )
      SimulationElectronProperties::setMinElectronEnergy( min_energy );
    else
    {
      SimulationElectronProperties::setMinElectronEnergy(
			SimulationElectronProperties::getAbsoluteMinElectronEnergy() );
      
      std::cerr << "Warning: the lowest supported electron energy is "
		<< SimulationElectronProperties::getAbsoluteMinElectronEnergy()
		<< ". This value will be used instead of "
		<< min_energy << "." << std::endl;
    }
  }

  // Get the max electron energy - optional
  if( properties.isParameter( "Max Electron Energy" ) )
  {
    double max_energy = properties.get<double>( "Max Electron Energy" );

    if( max_energy <= SimulationElectronProperties::getAbsoluteMaxElectronEnergy() )
      SimulationElectronProperties::setMaxElectronEnergy( max_energy );
    else
    {
      SimulationElectronProperties::setMaxElectronEnergy(
			SimulationElectronProperties::getAbsoluteMaxElectronEnergy() );
      
      std::cerr << "Warning: the highest supported electron energy is "
		<< SimulationElectronProperties::getAbsoluteMaxElectronEnergy()
		<< ". This value will be used instead of "
		<< max_energy << "." << std::endl;
    }
  }

  // Get the atomic relaxation mode - optional
  if( properties.isParameter( "Electron Atomic Relaxation" ) )
  {
    if( !properties.get<bool>( "Electron Atomic Relaxation" ) )
      SimulationElectronProperties::setAtomicRelaxationModeOff();
  }

  // Get the bremsstrahlung photon angular distribution function - optional
  if( properties.isParameter( "Bremsstrahlung Angular Distribution" ) )
  {
    std::string raw_function = 
           properties.get<std::string>( "Bremsstrahlung Angular Distribution" );
    
     MonteCarlo::BremsstrahlungAngularDistributionType function;
    
    if( raw_function == "Dipole" || raw_function == "dipole" || raw_function == "DIPOLE" )
      function = MonteCarlo::DIPOLE_DISTRIBUTION;
    else if( raw_function == "Tabular" || raw_function == "tabular" || raw_function == "TABULAR" )
      function = MonteCarlo::DIPOLE_DISTRIBUTION;
    else if( raw_function == "2BS" || raw_function == "2bs" || raw_function == "twobs" )
      function = MonteCarlo::TWOBS_DISTRIBUTION;
    else
    {
      THROW_EXCEPTION( std::runtime_error,
		       "Error: bremsstrahlung angular distribution " << raw_function << 
               " is not currently supported!" );
    }
   
     SimulationElectronProperties::setBremsstrahlungAngularDistributionFunction( 
                                                                     function );
  }

  // Get the elastic cutoff angle cosine - optional
  if( properties.isParameter( "Elastic Cutoff Angle" ) )
  {
    double cutoff_angle = 
            properties.get<double>( "Elastic Cutoff Angle" );

    if( cutoff_angle >= 0.0 && cutoff_angle <= 2.0 )
    {
      SimulationElectronProperties::setElasticCutoffAngle( cutoff_angle );
    }
    else
    {
      std::cerr << "Warning: the elastic cutoff angle must have a "
		<< "value between 0 and 2. The default value of "
		<< SimulationElectronProperties::getElasticCutoffAngle()
		<< " will be used instead of " << cutoff_angle << "." 
		<< std::endl;
    }
  }

  // Get the number of photon hash grid bins - optional
  if( properties.isParameter( "Electron Hash Grid Bins" ) )
  {
    unsigned bins = properties.get<unsigned>( "Electron Hash Grid Bins" );

    SimulationElectronProperties::setNumberOfElectronHashGridBins( bins );
  }
  
  properties.unused( std::cerr );
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_SimulationElectronPropertiesFactory.cpp
//---------------------------------------------------------------------------//
