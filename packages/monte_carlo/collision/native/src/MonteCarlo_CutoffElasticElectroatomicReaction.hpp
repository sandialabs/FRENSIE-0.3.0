//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_CutoffElasticElectroatomicReaction.hpp
//! \author Luke Kersting
//! \brief  The cutoff scattering elastic electroatomic reaction class decl.
//! 
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_CUTOFF_ELASTIC_ELECTROATOMIC_REACTION_HPP
#define MONTE_CARLO_CUTOFF_ELASTIC_ELECTROATOMIC_REACTION_HPP

// Trilinos Includes
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "MonteCarlo_StandardElectroatomicReaction.hpp"
#include "MonteCarlo_CutoffElasticElectronScatteringDistribution.hpp"

namespace MonteCarlo{

//! The cutoff elastic electroatomic reaction class
template<typename InterpPolicy, bool processed_cross_section = false>
class CutoffElasticElectroatomicReaction : public StandardElectroatomicReaction<InterpPolicy,processed_cross_section>
{

public:

  //! Basic Constructor
  CutoffElasticElectroatomicReaction( 
	  const Teuchos::ArrayRCP<const double>& incoming_energy_grid,
	  const Teuchos::ArrayRCP<const double>& cross_section,
	  const unsigned threshold_energy_index,
      const Teuchos::RCP<const CutoffElasticElectronScatteringDistribution>&
            scattering_distribution );

  //! Constructor
  CutoffElasticElectroatomicReaction( 
	  const Teuchos::ArrayRCP<const double>& incoming_energy_grid,
	  const Teuchos::ArrayRCP<const double>& cross_section,
	  const unsigned threshold_energy_index,
      const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
      const Teuchos::RCP<const CutoffElasticElectronScatteringDistribution>&
            scattering_distribution );


  //! Destructor
  ~CutoffElasticElectroatomicReaction()
  { /* ... */ }

  //! Return the number of electrons emitted from the rxn at the given energy
  unsigned getNumberOfEmittedElectrons( const double energy ) const;

  //! Return the number of photons emitted from the rxn at the given energy
  unsigned getNumberOfEmittedPhotons( const double energy ) const;

  //! Return the reaction type
  ElectroatomicReactionType getReactionType() const;

  //! Simulate the reaction
  void react( ElectronState& electron, 
              ParticleBank& bank,
              Data::SubshellType& shell_of_interaction ) const;

  //! Return the cross section at the given energy
  double getCrossSection( const double energy ) const;

  //! Return the cross section at the given energy (efficient)
  double getCrossSection( const double energy,
                          const unsigned bin_index ) const;

private:


  // The cutoff_elastic scattering distribution
  Teuchos::RCP<const CutoffElasticElectronScatteringDistribution> 
    d_scattering_distribution;
};

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// Template Includes
//---------------------------------------------------------------------------//

#include "MonteCarlo_CutoffElasticElectroatomicReaction_def.hpp"

//---------------------------------------------------------------------------//

#endif // end MONTE_CARLO_CUTOFF_ELASTIC_ELECTROATOMIC_REACTION_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_CutoffElasticElectroatomicReaction.hpp
//---------------------------------------------------------------------------//