//---------------------------------------------------------------------------//
//!
//! \file   NeutronMaterial.cpp
//! \author Alex Robinson
//! \brief  Neutron material class definition
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <limits>

// FACEMC Includes
#include "NeutronMaterial.hpp"
#include "PhysicalConstants.hpp"
#include "MaterialHelpers.hpp"
#include "ContractException.hpp"

namespace FACEMC{

// Constructor
NeutronMaterial::NeutronMaterial(
		const Traits::ModuleTraits::InternalMaterialHandle id,
		const double density,
	        const boost::unordered_map<unsigned,Teuchos::RCP<Nuclide> >&
	        nuclide_id_map,
		const Teuchos::Array<Pair<unsigned,double> >&
		material_nuclide_id_fraction_data )
  : d_id( id ),
    d_number_density( density ),
    d_nuclides( material_nuclide_id_fraction_data.size() )
{
  // Make sure the fraction values are valid (all positive or all negative)
  testPrecondition( areFractionValuesValid<SECOND>( 
				   material_nuclide_id_fraction_data.begin(),
				   material_nuclide_id_fraction_data.end() ) );
		    
  // Copy the nuclides that make up this material
  for( unsigned i = 0u; i < material_nuclide_id_fraction_data.size(); ++i )
  {
    d_nuclides[i].first = material_nuclide_id_fraction_data[i].second;
    
    d_nuclides[i].second = nuclide_id_map.find( 
			  material_nuclide_id_fraction_data[i].first )->second;
  }

  // Convert weight fractions to atom fractions
  if( d_nuclides.front().first < 0.0 )
  {
    convertWeightFractionsToAtomFractions<FIRST,SECOND>( 
					     d_nuclides.begin(),
					     d_nuclides.end(),
					     d_nuclides.begin(),
					     d_nuclides.end(),
					     &NeutronMaterial::getNuclideAWR );
  }
  else // Normalize the atom fractions
  {
    normalizeFractionValues<FIRST>( d_nuclides.begin(), d_nuclides.end() );
  }
    
  // Convert the mass density to a number density
  if( density < 0.0 )
  {
    d_number_density = 
      convertMassDensityToNumberDensity<FIRST,SECOND>(
					     -1.0*density,
					     d_nuclides.begin(),
					     d_nuclides.end(),
					     d_nuclides.begin(),
					     d_nuclides.end(),
					     &NeutronMaterial::getNuclideAWR );
    
    // Atomic weight ratios were used to do the conversion - divide by the
    // neutron amu to get the correct number density
    d_number_density /= PhysicalConstants::neutron_rest_mass_amu;
  }

  // Convert the atom fractions to isotopic number densities
  scaleAtomFractionsByNumberDensity<FIRST>( d_number_density,
					    d_nuclides.begin(),
					    d_nuclides.end() );
}

// Return the material id
Traits::ModuleTraits::InternalMaterialHandle NeutronMaterial::getId() const
{
  return d_id;
}

// Return the material number density (atom/b-cm)
double NeutronMaterial::getNumberDensity() const
{
  return d_number_density;
}

// Return the macroscopic total crosss section (1/cm)
double NeutronMaterial::getMacroscopicTotalCrossSection( 
						    const double energy ) const
{
  double cross_section = 0.0;
  
  for( unsigned i = 0u; i < d_nuclides.size(); ++i )
  {
    cross_section += 
      d_nuclides[i].first*d_nuclides[i].second->getTotalCrossSection( energy );
  }

  return cross_section;
}

// Return the macroscopic absorption cross section (1/cm)
double NeutronMaterial::getMacroscopicAbsorptionCrossSection( 
						    const double energy ) const
{
  double cross_section = 0.0;
  
  for( unsigned i = 0u; i < d_nuclides.size(); ++i )
  {
    cross_section += d_nuclides[i].first*
      d_nuclides[i].second->getAbsorptionCrossSection( energy );
  }

  return cross_section;
}

// Return the survival probability
double NeutronMaterial::getSurvivalProbability( const double energy ) const
{
  double total_cross_sec = this->getMacroscopicTotalCrossSection( energy );

  double absorption_cross_sec = 
    this->getMacroscopicAbsorptionCrossSection( energy );

  return 1.0 - absorption_cross_sec/total_cross_sec;
}

// Return the macroscopic cross section (1/cm)
double NeutronMaterial::getMacroscopicReactionCrossSection( 
				     const double energy,
				     const NuclearReactionType reaction ) const
{
  double cross_section = 0.0;
  
  for( unsigned i = 0u; i < d_nuclides.size(); ++i )
  {
    cross_section += d_nuclides[i].first*
      d_nuclides[i].second->getReactionCrossSection( energy, reaction );
  }

  return cross_section;
}

// Collide with a neutron
void NeutronMaterial::collideAnalogue( NeutronState& neutron, 
				       ParticleBank& bank ) const
{
  unsigned nuclide_index = sampleCollisionNuclide( neutron.getEnergy() );

  d_nuclides[nuclide_index].second->collideAnalogue( neutron, bank );
}

// Collide with a neutron and survival bias
/*! \details The method of survival biasing that has been implemented is to
 * first select the nuclide that is collided with. The particle weight is
 * then multiplied by the survival probability associated with the 
 * microscopic cross sections for the collision nuclide. An alternative method
 * would be to multiply the weight of the particle by the macroscopic
 * survival probability associated with the material and then sample a 
 * collision nuclide. The latter method appears to be more involved than the
 * former, which is why the former was chosen.
 */
void NeutronMaterial::collideSurvivalBias( NeutronState& neutron, 
					   ParticleBank& bank ) const
{
  unsigned nuclide_index = sampleCollisionNuclide( neutron.getEnergy() );

  d_nuclides[nuclide_index].second->collideSurvivalBias( neutron, bank );
}

// Sample the nuclide that is collided with
unsigned NeutronMaterial::sampleCollisionNuclide( const double energy ) const
{
  double scaled_random_number = 
    RandomNumberGenerator::getRandomNumber<double>()*
    this->getMacroscopicTotalCrossSection( energy );

  double partial_total_cs = 0.0;

  unsigned collision_nuclide_index = std::numeric_limits<unsigned>::max();
  
  for( unsigned i = 0u; i < d_nuclides.size(); ++i )
  {
    partial_total_cs += d_nuclides[i].second->getTotalCrossSection( energy );
    
    if( scaled_random_number < partial_total_cs )
    {
      collision_nuclide_index = i;

      break;
    }
  }

  // Make sure a collision index was found
  testPostcondition( collision_nuclide_index != 
		     std::numeric_limits<unsigned>::max() );
  
  return collision_nuclide_index;
}

// Get the atomic weight ratio from a nuclide pointer
double NeutronMaterial::getNuclideAWR( 
			      const Pair<double,Teuchos::RCP<Nuclide> >& pair )
{
  return pair.second->getAtomicWeightRatio();
}

} // end FACEMC namespace

//---------------------------------------------------------------------------//
// end NeutronMaterial.cpp
//---------------------------------------------------------------------------//
