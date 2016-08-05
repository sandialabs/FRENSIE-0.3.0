//---------------------------------------------------------------------------//
//!
//! \file   DataGen_StandardAdjointAdjointElectronPhotonRelaxationDataGenerator_def.hpp
//! \author Luke Kersting
//! \brief  The standard adjoint electron-photon-relaxation data generator template def
//!
//---------------------------------------------------------------------------//

#ifndef DATA_GEN_STANDARD_ADJOINT_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_DEF_HPP
#define DATA_GEN_STANDARD_ADJOINT_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_DEF_HPP

// FRENSIE Includes
#include "Utility_TabularDistribution.hpp"

namespace DataGen{

// Create the cross section on the union energy grid
template <typename ElectroatomicReaction>
void StandardAdjointElectronPhotonRelaxationDataGenerator::createAdjointCrossSectionOnUnionEnergyGrid(
   const std::list<double>& union_energy_grid,
   const std::shared_ptr<DataGen::AdjointElectronCrossSectionEvaluator<ElectroatomicReaction> >
        adjoint_electron_cs_evaluator,
   const double evaluation_tolerance,
   std::vector<double>& cross_section,
   unsigned& threshold_index ) const
{
  std::vector<double> raw_cross_section( union_energy_grid.size() );

  std::list<double>::const_iterator energy_grid_pt = union_energy_grid.begin();

  unsigned index = 0u;

  while( energy_grid_pt != union_energy_grid.end() )
  {
    raw_cross_section[index] =
      adjoint_electron_cs_evaluator->evaluateAdjointCrossSection(
        *energy_grid_pt,
        evaluation_tolerance );

    ++energy_grid_pt;
    ++index;
  }

  std::vector<double>::iterator start =
    std::find_if( raw_cross_section.begin(),
		  raw_cross_section.end(),
		  notEqualZero );

  cross_section.assign( start, raw_cross_section.end() );

  threshold_index = std::distance( raw_cross_section.begin(), start );
}

// Create the cross section on the union energy grid
template <typename ElectroatomicReaction>
void StandardAdjointElectronPhotonRelaxationDataGenerator::createAdjointCrossSectionOnUnionEnergyGrid(
   const std::list<double>& union_energy_grid,
   const std::list<double>& old_union_energy_grid,
   const std::vector<double>& old_cross_section,
   const std::shared_ptr<DataGen::AdjointElectronCrossSectionEvaluator<ElectroatomicReaction> >
        adjoint_electron_cs_evaluator,
   const double evaluation_tolerance,
   std::vector<double>& cross_section,
   unsigned& threshold_index ) const
{
  std::vector<double> raw_cross_section( union_energy_grid.size() );

  std::list<double>::const_iterator energy_grid_pt = union_energy_grid.begin();
  std::list<double>::const_iterator old_energy_grid_pt =
    old_union_energy_grid.begin();

  unsigned index = 0u;
  unsigned old_index = 0u;

  while( energy_grid_pt != union_energy_grid.end() )
  {
    if ( *energy_grid_pt == *old_energy_grid_pt )
    {
      raw_cross_section[index] = old_cross_section[old_index];
   
      ++old_energy_grid_pt;
      ++old_index;
    }
    else
    {
      raw_cross_section[index] =
        adjoint_electron_cs_evaluator->evaluateAdjointCrossSection(
          *energy_grid_pt,
          evaluation_tolerance );
    }

    ++energy_grid_pt;
    ++index;
  }

  std::vector<double>::iterator start =
    std::find_if( raw_cross_section.begin(),
		  raw_cross_section.end(),
		  notEqualZero );

  cross_section.assign( start, raw_cross_section.end() );

  threshold_index = std::distance( raw_cross_section.begin(), start );
}

} // end DataGen namespace

#endif // end DATA_GEN_STANDARD_ADJOINT_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_DEF_HPP

//---------------------------------------------------------------------------//
// end DataGen_StandardAdjointElectronPhotonRelaxationDataGenerator_def.hpp
//---------------------------------------------------------------------------//