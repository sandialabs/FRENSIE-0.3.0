//---------------------------------------------------------------------------//
//! 
//! \file   NeutronNeutronScatteringDistributionFactor.hpp
//! \author Alex Robinson
//! \brief  NeutronNeutron scattering distribution factory class declaration
//!
//---------------------------------------------------------------------------//

#ifndef NEUTRON_SCATTERING_DISTRIBUTION_FACTORY_HPP
#define NEUTRON_SCATTERING_DISTRIBUTION_FACTORY_HPP

// Std Lib Includes
#include <string>

// Boost Includes
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

// Trilinos Includes
#include <Teuchos_RCP.hpp>
#include <Teuchos_Array.hpp>

// FACEMC Includes
#include "NeutronNeutronScatteringDistribution.hpp"
#include "ElasticNeutronScatteringDistribution.hpp"
#include "NuclearReactionType.hpp"

namespace FACEMC{

//! The scattering distribution factory class
class NeutronNeutronScatteringDistributionFactory
{
  
public:

  //! Constructor 
  NeutronNeutronScatteringDistributionFactory( 
			   const std::string& table_name,
			   const double atomic_weight_ratio,
			   const Teuchos::ArrayView<const double>& mtr_block,
			   const Teuchos::ArrayView<const double>& tyr_block,
			   const Teuchos::ArrayView<const double>& land_block,
			   const Teuchos::ArrayView<const double>& and_block,
			   const Teuchos::ArrayView<const double>& ldlw_block,
			   const Teuchos::ArrayView<const double>& dlw_block );

  //! Destructor
  ~NeutronNeutronScatteringDistributionFactory()
  { /* ... */ }

  //! Create the elastic scattering distribution
  void createElasticScatteringDist( 
                            Teuchos::RCP<NeutronNeutronScatteringDistribution>&
			    elastic_distribution ) const;

  //! Create a scattering distribution 
  void createScatteringDist( 
	      Teuchos::RCP<NeutronNeutronScatteringDistribution>& distribution,
	      const NuclearReactionType reaction_type ) const;

private:

  // Initialize the reaction type ordering map
  void initializeReactionOrderingMap( 
			   const Teuchos::ArrayView<const double>& mtr_block,
			   const Teuchos::ArrayView<const double>& tyr_block );

  // Initialize the reaction type scattering ref. frame map
  void initializeReactionRefFrameMap( 
			   const Teuchos::ArrayView<const double>& mtr_block,
			   const Teuchos::ArrayView<const double>& tyr_block );

  // Initialize the reaction type angular distribution start index map
  void initializeReactionAngularDistStartIndexMap(
			  const Teuchos::ArrayView<const double>& land_block );

  // Initialize the reaction type angular distribution map
  void initializeReactionAngularDistMap(
			   const Teuchos::ArrayView<const double>& land_block,
			   const Teuchos::ArrayView<const double>& and_block );

  // Initialize the reaction type energy distribution map
  void initializeReactionEnergyDistMap(
			   const Teuchos::ArrayView<const double>& ldlw_block,
			   const Teuchos::ArrayView<const double>& dlw_block );

  // Calculate the AND block angular distribution array sizes
  void calculateAngularDistArraySizes( 
                    const Teuchos::ArrayView<const double>& land_block,
		    const Teuchos::ArrayView<const double>& and_block,
                    Teuchos::Array<unsigned>& angular_dist_array_sizes ) const;

  // The default (isotropic) angle cosine distribution
  static Teuchos::RCP<OneDDistribution> isotropic_angle_cosine_dist;

  // The table name
  std::string d_table_name;

  // The atomic weight ratio
  double d_atomic_weight_ratio;

  // A map of the reaction types (MT #s) and their AND block ordering
  boost::unordered_map<NuclearReactionType,unsigned> d_reaction_ordering;

  // A map of the reaction types (MT #s) and the scattering reference frame
  // Note: True = center-of-mass, False = lab
  boost::unordered_map<NuclearReactionType,bool> d_reaction_cm_scattering;
  
  // A set of the reaction types (MT #s) with isotropic scattering only
  boost::unordered_set<NuclearReactionType>
  d_reactions_with_isotropic_scattering_only;

  // A set of the reaction types (MT #s) with coupled energy-angle dist
  boost::unordered_set<NuclearReactionType>
  d_reactions_with_coupled_energy_angle_dist;

  // A map of the reaction types (MT #s) and the corresponding angular dist
  boost::unordered_map<NuclearReactionType,Teuchos::ArrayView<const double> >
  d_reaction_angular_dist;

  // A map of the reaction types (MT #s) and the angular dist start index
  boost::unordered_map<NuclearReactionType,int> 
  d_reaction_angular_dist_start_index;
  
  // A map of the reaction types (MT #s) and the corresponding energy dist
  boost::unordered_map<NuclearReactionType,Teuchos::ArrayView<const double> >
  d_reaction_energy_dist;
};

} // end FACEMC namespace

#endif // end NEUTRON_SCATTERING_DISTRIBUTION_FACTORY_HPP

//---------------------------------------------------------------------------//
// end NeutronScatteringDistributionFactory.hpp
//---------------------------------------------------------------------------//