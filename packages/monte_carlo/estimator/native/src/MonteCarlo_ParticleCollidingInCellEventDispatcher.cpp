//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ParticleCollidingInCellEventDispatcher.cpp
//! \author Alex Robinson
//! \brief  Particle colliding in cell event dispatcher class definition.
//!
//---------------------------------------------------------------------------//

// FRENSIE Includes
#include "MonteCarlo_ParticleCollidingInCellEventDispatcher.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Constructor
ParticleCollidingInCellEventDispatcher::ParticleCollidingInCellEventDispatcher(
	             const Geometry::ModuleTraits::InternalCellHandle cell_id )
  : ParticleEventDispatcher<Geometry::ModuleTraits::InternalCellHandle,
			    ParticleCollidingInCellEventObserver>( cell_id )
{ /* ... */ }

// Dispatch the new event to the observers
void 
ParticleCollidingInCellEventDispatcher::dispatchParticleCollidingInCellEvent(
	    const ParticleState& particle,
	    const Geometry::ModuleTraits::InternalCellHandle cell_of_collision,
	    const double inverse_total_cross_section )
{
  // Make sure the cell being collided in is valid
  testPrecondition( cell_of_collision == this->getId() );

  ObserverIdMap::iterator it = observer_id_map().begin();

  while( it != observer_id_map().end() )
  {
    it->second->updateFromParticleCollidingInCellEvent( 
						 particle, 
						 cell_of_collision,
						 inverse_total_cross_section );

    ++it;
  }
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_ParticleCollidingInCellEventDispatcher.cpp
//---------------------------------------------------------------------------//
