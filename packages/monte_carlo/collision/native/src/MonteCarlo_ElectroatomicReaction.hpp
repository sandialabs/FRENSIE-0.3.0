//---------------------------------------------------------------------------//
//!
//! \file MonteCarlo_ElectroAtomicReaction.hpp
//! \author Luke Kersting
//! \brief The Electron reaction base class declaration
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_ELECTROATOMIC_REACTION_HPP
#define MONTE_CARLO_ELECTROATOMIC_REACTION_HPP

// FRENSIE Includes
#include "MonteCarlo_ElectronState.hpp"
#include "MonteCarlo_ParticleBank.hpp"
#include "MonteCarlo_SubshellType.hpp"
#include "MonteCarlo_ElectroatomicReactionType.hpp"

namespace MonteCarlo{

//! The electron/positron reaction base class
class ElectroatomicReaction
{

public:

  //!Constructor
  ElectroatomicReaction()
  { /* ... */ }

  //!Destructor
  virtual ~ElectroatomicReaction()
  { /* ... */}
	
  //! Return the cross section at the given energy
  virtual double getCrossSection( const double energy ) const = 0;

  //! Return the threshold energy
  virtual double getThresholdEnergy() const = 0;

  //! Return the number of electrons emitted from the rxn at the given energy
  virtual unsigned getNumberOfEmittedElectrons( const double energy ) const = 0;

  //! Return the number of photons emitted from the rxn at the given energy
  virtual unsigned getNumberOfEmittedPhotons( const double energy ) const = 0;

  //! Return reaction type
  virtual ElectroatomicReactionType getReactionType() const = 0;

  //! Simulate the reaction
  virtual void react( ElectronState& electron, 
                      ParticleBank& bank,
                      SubshellType& shell_of_interaction ) const = 0;
};

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_ELECTROATOMIC_REACTION_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_ElectroAtomicReaction.hpp
//---------------------------------------------------------------------------//
