//---------------------------------------------------------------------------//
//!
//! \file   DataGen_StandardElectronPhotonRelaxationDataGenerator_def.hpp
//! \author Alex Robinson
//! \brief  The standard electron-photon-relaxation data generator template def
//!
//---------------------------------------------------------------------------//

#ifndef DATA_GEN_STANDARD_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_DEF_HPP
#define DATA_GEN_STANDARD_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_DEF_HPP

// FRENSIE Includes
#include "Utility_TabularDistribution.hpp"

namespace DataGen{

// Extract the average photon heating numbers
template<typename InterpPolicy>
void StandardElectronPhotonRelaxationDataGenerator::extractCrossSection(
	   Teuchos::ArrayView<const double> raw_energy_grid,
	   Teuchos::ArrayView<const double> raw_cross_section,
	   Teuchos::RCP<const Utility::OneDDistribution>& cross_section ) const
{
  // Find the first non-zero cross section value
  Teuchos::ArrayView<const double>::iterator start = 
    std::find_if( raw_cross_section.begin(),
		  raw_cross_section.end(),
		  notEqualZero );

  Teuchos::Array<double> processed_cross_section;
  processed_cross_section.assign( start, raw_cross_section.end() );

  unsigned start_energy_index = 
    raw_energy_grid.size() - processed_cross_section.size();

  start = raw_energy_grid.begin();
  std::advance( start, start_energy_index );
  
  Teuchos::Array<double> energy_grid;
  energy_grid.assign( start, raw_energy_grid.end() );

  // Recover the original energy grid and cross_section
  for( unsigned i = 0; i < energy_grid.size(); ++i )
  {
    energy_grid[i] = InterpPolicy::recoverProcessedIndepVar( energy_grid[i] );
    
    processed_cross_section[i] = 
      InterpPolicy::recoverProcessedDepVar( processed_cross_section[i] );
  }

  cross_section.reset( new Utility::TabularDistribution<InterpPolicy>(
						   energy_grid,
						   processed_cross_section ) );
}

// Extract electron cross section
template<typename InterpPolicy>
void StandardElectronPhotonRelaxationDataGenerator::extractElectronCrossSection(
       std::vector<double>& raw_energy_grid,
       std::vector<double>& raw_cross_section,        
	   Teuchos::RCP<const Utility::OneDDistribution>& cross_section ) const
{
  Teuchos::Array<double> processed_cross_section( raw_cross_section );
  Teuchos::Array<double> energy_grid( raw_energy_grid );

  cross_section.reset( new Utility::TabularDistribution<InterpPolicy>(
						   energy_grid,
						   processed_cross_section ) );
}

} // end DataGen namespace

#endif // end DATA_GEN_STANDARD_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_DEF_HPP

//---------------------------------------------------------------------------//
// end DataGen_StandardElectronPhotonRelaxationDataGenerator_def.hpp
//---------------------------------------------------------------------------//
