//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_Nuclide.cpp
//! \author Alex Robinson
//! \brief  The nuclide class declaration
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <stdexcept>
#include <sstream>

// FRENSIE Includes
#include "MonteCarlo_Nuclide.hpp"
#include "MonteCarlo_NeutronAbsorptionReaction.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_SearchAlgorithms.hpp"
#include "Utility_SortAlgorithms.hpp"
#include "Utility_InterpolationPolicy.hpp"
#include "Utility_ExceptionTestMacros.hpp"

namespace MonteCarlo{

// Initialize the static member data
boost::unordered_set<NuclearReactionType> Nuclide::absorption_reaction_types =
  Nuclide::setDefaultAbsorptionReactionTypes();

// Set the default absorption reaction types
boost::unordered_set<NuclearReactionType> 
Nuclide::setDefaultAbsorptionReactionTypes()
{
  boost::unordered_set<NuclearReactionType> absorption_reaction_type;
  absorption_reaction_type.insert( N__CAPTURE_REACTION );
  absorption_reaction_type.insert( N__GAMMA_REACTION ); // start capture
  absorption_reaction_type.insert( N__P_REACTION );
  absorption_reaction_type.insert( N__D_REACTION );
  absorption_reaction_type.insert( N__T_REACTION );
  absorption_reaction_type.insert( N__HE3_REACTION );
  absorption_reaction_type.insert( N__ALPHA_REACTION );
  absorption_reaction_type.insert( N__2ALPHA_REACTION );
  absorption_reaction_type.insert( N__3ALPHA_REACTION );
  absorption_reaction_type.insert( N__2P_REACTION );
  absorption_reaction_type.insert( N__P_ALPHA_REACTION );
  absorption_reaction_type.insert( N__T_2ALPHA_REACTION );
  absorption_reaction_type.insert( N__D_2ALPHA_REACTION );
  absorption_reaction_type.insert( N__P_D_REACTION );
  absorption_reaction_type.insert( N__P_T_REACTION );
  absorption_reaction_type.insert( N__D_ALPHA_REACTION ); // end capture

  return absorption_reaction_type;
}

// Set the nuclear reaction types that will be considered as absorption
void Nuclide::setAbsorptionReactionTypes( 
	 const Teuchos::Array<NuclearReactionType>& absorption_reaction_types )
{
  Nuclide::absorption_reaction_types.clear();
  
  Nuclide::absorption_reaction_types.insert( absorption_reaction_types.begin(),
					     absorption_reaction_types.end() );
}

// Add nuclear reaction types that will be considered as absorption
void Nuclide::addAbsorptionReactionTypes( 
	 const Teuchos::Array<NuclearReactionType>& absorption_reaction_types )
{
  for( unsigned i = 0; i < absorption_reaction_types.size(); ++i )
    Nuclide::absorption_reaction_types.insert( absorption_reaction_types[i] );
}

// Create a unique id for the nuclide based on its name
/*! The names are assumed to be in the form ZZZAAA.xxc. As long as this format
 * is used, this function will return a unique id corresponding to the name by
 * stripping away the "." and the "c".
 */
unsigned Nuclide::getUniqueIdFromName( const std::string& name )
{
  std::string name_copy( name );
  
  // Remove all but the numbers in the name
  unsigned char_loc = name_copy.find_first_not_of("0123456789");
  
  while( char_loc < name_copy.size() )
  {
    name_copy.erase( char_loc, 1 );

    char_loc = name_copy.find_first_not_of("0123456789");
  }

  std::stringstream ss;
  ss << name_copy;

  unsigned id;

  ss >> id;
  
  return id;
}

// Constructor
Nuclide::Nuclide( const std::string& name,
		  const unsigned atomic_number,
		  const unsigned atomic_mass_number,
		  const unsigned isomer_number,
		  const double atomic_weight_ratio,
		  const double temperature,
		  const Teuchos::ArrayRCP<double>& energy_grid,
		  const ReactionMap& standard_scattering_reactions,
		  const ReactionMap& standard_absorption_reactions )
  : d_name( name ),
    d_id( Nuclide::getUniqueIdFromName( name ) ),
    d_atomic_number( atomic_number ),
    d_atomic_mass_number( atomic_mass_number ),
    d_isomer_number( isomer_number ),
    d_atomic_weight_ratio( atomic_weight_ratio ),
    d_temperature( temperature ),
    d_total_reaction(),
    d_total_absorption_reaction()
{
  // Make sure the atomic weight ratio is valid
  testPrecondition( atomic_weight_ratio > 0.0 );
  // Make sure the temperature is valid
  testPrecondition( temperature > 0.0 );
  // Make sure the energy grid is valid
  testPrecondition( energy_grid.size() > 1 );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
						      energy_grid.end() ) );
  // There must be at least one reaction specified
  testPrecondition( standard_scattering_reactions.size() +
		    standard_absorption_reactions.size() > 0 );

  // Place the reactions in the appropriate group
  ReactionMap::const_iterator reaction_type_pointer, end_reaction_type_pointer;

  reaction_type_pointer = standard_scattering_reactions.begin();
  end_reaction_type_pointer = standard_scattering_reactions.end();

  while( reaction_type_pointer != end_reaction_type_pointer )
  {
    if(Nuclide::absorption_reaction_types.count(reaction_type_pointer->first))
      d_absorption_reactions.insert( *reaction_type_pointer );
    else
      d_scattering_reactions.insert( *reaction_type_pointer );

    ++reaction_type_pointer;
  }

  reaction_type_pointer = standard_absorption_reactions.begin();
  end_reaction_type_pointer = standard_absorption_reactions.end();

  while( reaction_type_pointer != end_reaction_type_pointer )
  {
    if(Nuclide::absorption_reaction_types.count(reaction_type_pointer->first))
      d_absorption_reactions.insert( *reaction_type_pointer );
    else
      d_miscellaneous_reactions.insert( *reaction_type_pointer );

    ++reaction_type_pointer;
  }

  // Calculate the total absorption cross section
  calculateTotalAbsorptionReaction( energy_grid );
  
  // Calculate the total cross section
  calculateTotalReaction( energy_grid );
}

// Return the nuclide name
const std::string& Nuclide::getName() const
{
  return d_name;
}

// Return the nuclide id
unsigned Nuclide::getId() const
{
  return d_id;
}

// Return the atomic number of the nuclide
unsigned Nuclide::getAtomicNumber() const
{
  return d_atomic_number;
}

// Return the atomic mass number of the nuclide
unsigned Nuclide::getAtomicMassNumber() const
{
  return d_atomic_mass_number;
}

// Return the nuclear isomer number of the nuclide
unsigned Nuclide::getIsomerNumber() const
{
  return d_isomer_number;
}
  
// Return the atomic weight ratio
double Nuclide::getAtomicWeightRatio() const
{
  return d_atomic_weight_ratio;
}

// Return the temperature of the nuclide (in MeV)
double Nuclide::getTemperature() const
{
  return d_temperature;
}

// Return the total cross section at the desired energy
double Nuclide::getTotalCrossSection( const double energy ) const
{
  return d_total_reaction->getCrossSection( energy );
}

// Return the total absorption cross section at the desired energy
double Nuclide::getAbsorptionCrossSection( const double energy ) const
{
  return d_total_absorption_reaction->getCrossSection( energy );
}
 
// Return the survival probability at the desired energy
double Nuclide::getSurvivalProbability( const double energy ) const
{
  // Make sure the energy is valid
  testPrecondition( !ST::isnaninf( energy ) );
  testPrecondition( energy > 0.0 );
  
  double survival_prob = 1.0 - 
    d_total_absorption_reaction->getCrossSection( energy )/
    d_total_reaction->getCrossSection( energy );

  // Make sure the survival probability is valid
  testPostcondition( survival_prob >= 0.0 );
  testPostcondition( survival_prob <= 1.0 );
  
  return survival_prob;
}

// Return the cross section for a specific nuclear reaction
double Nuclide::getReactionCrossSection( 
				     const double energy,
				     const NuclearReactionType reaction ) const
{
  switch( reaction )
  {
  case N__TOTAL_REACTION:
    return d_total_reaction->getCrossSection( energy );
  case N__TOTAL_ABSORPTION_REACTION:
    return d_total_absorption_reaction->getCrossSection( energy );
  default:
    ConstReactionMap::const_iterator nuclear_reaction = 
      d_scattering_reactions.find( reaction );
    
    if( nuclear_reaction != d_scattering_reactions.end() )
      return nuclear_reaction->second->getCrossSection( energy );
    
    nuclear_reaction = d_absorption_reactions.find( reaction );

    if( nuclear_reaction != d_absorption_reactions.end() )
      return nuclear_reaction->second->getCrossSection( energy );
    
    nuclear_reaction = d_miscellaneous_reactions.find( reaction );

    if( nuclear_reaction != d_miscellaneous_reactions.end() )
      return nuclear_reaction->second->getCrossSection( energy );
    else // If the reaction does not exist for the nuclide, return 0
      return 0.0;
  }
}
  
// Collide with a neutron
void Nuclide::collideAnalogue( NeutronState& neutron, 
			       ParticleBank& bank ) const
{
  double total_cross_section = 
    d_total_reaction->getCrossSection( neutron.getEnergy() );

  double scaled_random_number = 
    Utility::RandomNumberGenerator::getRandomNumber<double>()*
    total_cross_section;

  double absorption_cross_section = 
    d_total_absorption_reaction->getCrossSection( neutron.getEnergy() );

  // Check if absorption occurs
  if( scaled_random_number < absorption_cross_section )
  {
    sampleAbsorptionReaction( scaled_random_number, neutron, bank );

    // Set the neutron as gone regardless of the reaction that occurred.
    neutron.setAsGone(); 
  }
  else
  {
    sampleScatteringReaction( scaled_random_number - absorption_cross_section, 
			      neutron, 
			      bank );
  }
}

// Collide with a neutron and survival bias
void Nuclide::collideSurvivalBias( NeutronState& neutron, 
				   ParticleBank& bank) const
{
  double random_number = 
    Utility::RandomNumberGenerator::getRandomNumber<double>();
  
  double total_cross_section = 
    d_total_reaction->getCrossSection( neutron.getEnergy() );

  double scattering_cross_section = total_cross_section - 
    d_total_absorption_reaction->getCrossSection( neutron.getEnergy() );

  double survival_prob = scattering_cross_section/total_cross_section;
  
  // Multiply the neutron's weight by the survival probability
  if( survival_prob > 0.0 )
  {
    neutron.multiplyWeight( survival_prob );

    sampleScatteringReaction( random_number*scattering_cross_section,
			      neutron,
			      bank );
  }
  else
    neutron.setAsGone();
}

// Calculate the total absorption cross section
void Nuclide::calculateTotalAbsorptionReaction( 
				 const Teuchos::ArrayRCP<double>& energy_grid )
{
  ConstReactionMap::const_iterator reaction_type_pointer, 
    end_reaction_type_pointer;

  end_reaction_type_pointer = d_absorption_reactions.end();

  Teuchos::ArrayRCP<double> cross_section( energy_grid.size() );
  
  // Calculate the absorption cross section
  for( unsigned i = 0; i < energy_grid.size(); ++i )
  {
    cross_section[i] = 0.0; 
      
    reaction_type_pointer = d_absorption_reactions.begin();

    while( reaction_type_pointer != end_reaction_type_pointer )
    {
      cross_section[i] += 
	reaction_type_pointer->second->getCrossSection( energy_grid[i] );

      ++reaction_type_pointer;
    }
  }

  // Create the total absorption reaction
  d_total_absorption_reaction.reset( new NeutronAbsorptionReaction(
						  N__TOTAL_ABSORPTION_REACTION,
						  d_temperature,
						  0.0,
						  energy_grid[0],
						  energy_grid,
						  cross_section ) );
}

// Calculate the total cross section
void Nuclide::calculateTotalReaction(
				 const Teuchos::ArrayRCP<double>& energy_grid )
{
  ConstReactionMap::const_iterator reaction_type_pointer, 
    end_reaction_type_pointer;

  end_reaction_type_pointer = d_scattering_reactions.end();

  Teuchos::ArrayRCP<double> cross_section( energy_grid.size() );

  // Calculate the total cross section
  for( unsigned i = 0; i < energy_grid.size(); ++i )
  {
    cross_section[i] = 
      d_total_absorption_reaction->getCrossSection( energy_grid[i] );
      
    reaction_type_pointer = d_scattering_reactions.begin();

    while( reaction_type_pointer != end_reaction_type_pointer )
    {
      cross_section[i] += 
	reaction_type_pointer->second->getCrossSection( energy_grid[i] );

      ++reaction_type_pointer;
    }
  }
  
  // Create the total reaction
  d_total_reaction.reset( new NeutronAbsorptionReaction( N__TOTAL_REACTION,
							 d_temperature,
							 0.0,
							 energy_grid[0],
							 energy_grid,
							 cross_section ) );
}

// Sample a scattering reaction
// NOTE: The scaled random number must be a random number multiplied by the
//       total scattering cross section then subtracted by the absorption xs.
void Nuclide::sampleScatteringReaction( const double scaled_random_number,
					NeutronState& neutron,
					ParticleBank& bank ) const
{
  double partial_cross_section = 0.0;
    
  ConstReactionMap::const_iterator nuclear_reaction, nuclear_reaction_end;
    
  nuclear_reaction = d_scattering_reactions.begin();
  nuclear_reaction_end = d_scattering_reactions.end();
    
  while( nuclear_reaction != nuclear_reaction_end )
  {
    partial_cross_section += 
      nuclear_reaction->second->getCrossSection( neutron.getEnergy() );

    if( scaled_random_number < partial_cross_section )
      break;
      
    ++nuclear_reaction;
  }
  
  // Make sure a reaction was found
  testPostcondition( nuclear_reaction != nuclear_reaction_end );
  
  // Undergo reaction selected
  nuclear_reaction->second->react( neutron, bank );
}

// Sample an absorption reaction
// NOTE: The scaled random number must be a random number multiplied by the
//       total absorption cross section
void Nuclide::sampleAbsorptionReaction( const double scaled_random_number,
					NeutronState& neutron,
					ParticleBank& bank ) const
{
  double partial_cross_section = 0.0;
    
  ConstReactionMap::const_iterator nuclear_reaction, nuclear_reaction_end;
  
  nuclear_reaction = d_absorption_reactions.begin();
  nuclear_reaction_end = d_absorption_reactions.end();
  
  while( nuclear_reaction != nuclear_reaction_end )
  {
    partial_cross_section += 
      nuclear_reaction->second->getCrossSection( neutron.getEnergy() );
    
    if( scaled_random_number < partial_cross_section )
      break;
    
    ++nuclear_reaction;
  }

  // Make sure a reaction was selected
  testPostcondition( nuclear_reaction != nuclear_reaction_end );

  // Undergo the reaction selected
  nuclear_reaction->second->react( neutron, bank );
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_Nuclide.cpp
//---------------------------------------------------------------------------//
