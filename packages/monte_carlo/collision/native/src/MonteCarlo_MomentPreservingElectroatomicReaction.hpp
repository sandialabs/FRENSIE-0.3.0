//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_MomentPreservingElectroatomicReaction.hpp
//! \author Luke Kersting
//! \brief  The single scattering elastic electroatomic reaction class decl.
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_MOMENT_PRESERVING_ELECTROATOMIC_REACTION_HPP
#define MONTE_CARLO_MOMENT_PRESERVING_ELECTROATOMIC_REACTION_HPP

// Trilinos Includes
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "MonteCarlo_StandardElectroatomicReaction.hpp"
#include "MonteCarlo_MomentPreservingElectronScatteringDistribution.hpp"

namespace MonteCarlo{

//! The moment preserving elastic electroatomic reaction class
template<typename InterpPolicy, bool processed_cross_section = false>
class MomentPreservingElectroatomicReaction : public StandardElectroatomicReaction<InterpPolicy,processed_cross_section>
{

public:

  //! Constructor
  MomentPreservingElectroatomicReaction(
	  const Teuchos::ArrayRCP<const double>& incoming_energy_grid,
	  const Teuchos::ArrayRCP<const double>& cross_section,
	  const unsigned threshold_energy_index,
          const Teuchos::RCP<const MomentPreservingElectronScatteringDistribution>&
            scattering_distribution );


  //! Destructor
  ~MomentPreservingElectroatomicReaction()
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

private:


  // The moment preserving elastic scattering distribution
  Teuchos::RCP<const MomentPreservingElectronScatteringDistribution>
    d_scattering_distribution;
};

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// Template Includes
//---------------------------------------------------------------------------//

#include "MonteCarlo_MomentPreservingElectroatomicReaction_def.hpp"

//---------------------------------------------------------------------------//

#endif // end MONTE_CARLO_MOMENT_PRESERVING_ELECTROATOMIC_REACTION_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_MomentPreservingElectroatomicReaction.hpp
//---------------------------------------------------------------------------//