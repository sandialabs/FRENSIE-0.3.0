//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_HardElasticElectroatomicReaction.hpp
//! \author Luke Kersting
//! \brief  The single scattering elastic electroatomic reaction class decl.
//! 
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_HARD_ELASTIC_ELECTROATOMIC_REACTION_HPP
#define MONTE_CARLO_HARD_ELASTIC_ELECTROATOMIC_REACTION_HPP

// Trilinos Includes
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "MonteCarlo_StandardElectroatomicReaction.hpp"
#include "MonteCarlo_HardElasticElectronScatteringDistribution.hpp"

namespace MonteCarlo{

//! The hard elastic electroatomic reaction class
template<typename InterpPolicy, bool processed_cross_section = false>
class HardElasticElectroatomicReaction : public StandardElectroatomicReaction<InterpPolicy,processed_cross_section>
{

public:

  //! Constructor
  HardElasticElectroatomicReaction( 
	  const Teuchos::ArrayRCP<const double>& incoming_energy_grid,
	  const Teuchos::ArrayRCP<const double>& cross_section,
	  const unsigned threshold_energy_index,
          const Teuchos::RCP<const HardElasticElectronScatteringDistribution>&
            scattering_distribution,
      const double cutoff_angle_cosine );


  //! Destructor
  ~HardElasticElectroatomicReaction()
  { /* ... */ }

  //! Return the cross section at the given energy
  double getCrossSection( const double energy ) const;

  //! Return the number of electrons emitted from the rxn at the given energy
  unsigned getNumberOfEmittedElectrons( const double energy ) const;

  //! Return the number of photons emitted from the rxn at the given energy
  unsigned getNumberOfEmittedPhotons( const double energy ) const;

  //! Return the reaction type
  ElectroatomicReactionType getReactionType() const;

  //! Simulate the reaction
  void react( ElectronState& electron, 
              ParticleBank& bank,
              SubshellType& shell_of_interaction ) const;

private:


  // The hard_elastic scattering distribution
  Teuchos::RCP<const HardElasticElectronScatteringDistribution> 
    d_scattering_distribution;

  // The incoming energy grid (logarithms)
  Teuchos::ArrayRCP<const double> d_incoming_energy_grid;

  // The cross section values evaluated on the incoming energy grid
  Teuchos::ArrayRCP<const double> d_cross_section;

  // The threshold energy
  const unsigned d_threshold_energy_index;

  // The cutoff angle cosine between hard and soft scattering
  double d_cutoff_angle_cosine;
};

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// Template Includes
//---------------------------------------------------------------------------//

#include "MonteCarlo_HardElasticElectroatomicReaction_def.hpp"

//---------------------------------------------------------------------------//

#endif // end MONTE_CARLO_HARD_ELASTIC_ELECTROATOMIC_REACTION_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_HardElasticElectroatomicReaction.hpp
//---------------------------------------------------------------------------//
