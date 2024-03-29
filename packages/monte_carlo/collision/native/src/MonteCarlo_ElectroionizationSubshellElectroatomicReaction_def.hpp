//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ElectroionizationSubshellElectroatomicReaction_def.hpp
//! \author Luke Kersting
//! \brief  The electroionization subshell electroatomic reaction class definition
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_ELECTROIONIZATION_SUBSHELL_ELECTROATOMIC_REACTION_DEF_HPP
#define MONTE_CARLO_ELECTROIONIZATION_SUBSHELL_ELECTROATOMIC_REACTION_DEF_HPP

// Boost Includes
#include <boost/function.hpp>
#include <boost/bind.hpp>

// FRENSIE Includes
#include "MonteCarlo_ElectroatomicReactionType.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Basic Constructor 
template<typename InterpPolicy, bool processed_cross_section>
ElectroionizationSubshellElectroatomicReaction<InterpPolicy,processed_cross_section>::ElectroionizationSubshellElectroatomicReaction(
    const Teuchos::ArrayRCP<const double>& incoming_energy_grid,
    const Teuchos::ArrayRCP<const double>& cross_section,
    const unsigned threshold_energy_index,
    const SubshellType interaction_subshell,
    const Teuchos::RCP<const ElectroionizationSubshellElectronScatteringDistribution>&
            electroionization_subshell_distribution )
  : ElectroionizationElectroatomicReaction<InterpPolicy,processed_cross_section>(
                                                       incoming_energy_grid,
                                                       cross_section,
                                                       threshold_energy_index ),
    d_interaction_subshell( interaction_subshell ),
    d_electroionization_subshell_distribution( electroionization_subshell_distribution ),
    d_reaction_type( convertSubshellEnumToElectroatomicReactionEnum( 
                                                        interaction_subshell ) )
{
  // Make sure the interaction subshell is valid
  testPrecondition( interaction_subshell != INVALID_SUBSHELL );
  testPrecondition( interaction_subshell != UNKNOWN_SUBSHELL );

  // Make sure the distribution data is valid
  testPrecondition( !electroionization_subshell_distribution.is_null() );

/*
  // Make sure the electroionization subshell scattering distribution data is valid
  testPrecondition( electroionization_subshell_scattering_distribution.size() > 0 );
*/
}


// Constructor 
template<typename InterpPolicy, bool processed_cross_section>
ElectroionizationSubshellElectroatomicReaction<InterpPolicy,processed_cross_section>::ElectroionizationSubshellElectroatomicReaction(
    const Teuchos::ArrayRCP<const double>& incoming_energy_grid,
    const Teuchos::ArrayRCP<const double>& cross_section,
    const unsigned threshold_energy_index,
    const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
    const SubshellType interaction_subshell,
    const Teuchos::RCP<const ElectroionizationSubshellElectronScatteringDistribution>&
            electroionization_subshell_distribution )
  : ElectroionizationElectroatomicReaction<InterpPolicy,processed_cross_section>(
            incoming_energy_grid,
            cross_section,
            threshold_energy_index,
            grid_searcher ),
    d_interaction_subshell( interaction_subshell ),
    d_electroionization_subshell_distribution( 
            electroionization_subshell_distribution ),
    d_reaction_type( convertSubshellEnumToElectroatomicReactionEnum( 
            interaction_subshell ) )
{
  // Make sure the interaction subshell is valid
  testPrecondition( interaction_subshell != INVALID_SUBSHELL );
  testPrecondition( interaction_subshell != UNKNOWN_SUBSHELL );

  // Make sure the distribution data is valid
  testPrecondition( !electroionization_subshell_distribution.is_null() );

/*
  // Make sure the electroionization subshell scattering distribution data is valid
  testPrecondition( electroionization_subshell_scattering_distribution.size() > 0 );
*/
}

// Simulate the reaction
template<typename InterpPolicy, bool processed_cross_section>
void ElectroionizationSubshellElectroatomicReaction<InterpPolicy,processed_cross_section>::react( 
				     ElectronState& electron, 
				     ParticleBank& bank,
				     SubshellType& shell_of_interaction ) const
{
  d_electroionization_subshell_distribution->scatterElectron( 
                                               electron, 
                                               bank, 
                                               shell_of_interaction);

  electron.incrementCollisionNumber();

  shell_of_interaction = d_interaction_subshell;
}

// Return the reaction type
template<typename InterpPolicy, bool processed_cross_section>
ElectroatomicReactionType ElectroionizationSubshellElectroatomicReaction<InterpPolicy,processed_cross_section>::getReactionType() const
{
  return d_reaction_type;
}

// Get the interaction subshell (non-standard interface)
template<typename InterpPolicy, bool processed_cross_section>
unsigned ElectroionizationSubshellElectroatomicReaction<InterpPolicy,processed_cross_section>::getSubshell() const
{
  return d_interaction_subshell;
}

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_ELECTROIONIZATION_SUBSHELL_ELECTROATOMIC_REACTION_DEF_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_ElectroionizationSubshellElectroatomicReaction_def.hpp
//---------------------------------------------------------------------------//
