//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ParticleEventDispatcher.hpp
//! \author Alex Robinson
//! \brief  Particle event dispatcher base class declaration.
//!
//---------------------------------------------------------------------------//

#ifndef FACEMC_PARTICLE_EVENT_DISPATCHER_HPP
#define FACEMC_PARTICLE_EVENT_DISPATCHER_HPP

// Boost Includes
#include <boost/unordered_map.hpp>

// Teuchos Includes
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "MonteCarlo_ModuleTraits.hpp"

namespace MonteCarlo{

//! The particle event dispatcher base class
template<typename EntityHandle, typename Observer>
class ParticleEventDispatcher
{

public:

  //! Typedef for EntityHandle type
  typedef EntityHandle EntityHandleType;
  
  //! Typedef for Observer type
  typedef Observer ObserverType;

  //! Constructor
  ParticleEventDispatcher( const EntityHandle entity_id );

  //! Destructor
  virtual ~ParticleEventDispatcher()
  { /* ... */ }

  //! Attach an observer to the dispatcher
  void attachObserver( const ModuleTraits::InternalEstimatorHandle id,
		       Teuchos::RCP<Observer>& observer );

  //! Detach an observer from the dispatcher
  void detachObserver( const ModuleTraits::InternalEstimatorHandle id );

  //! Get the entity id corresponding to this particle event dispatcher
  EntityHandle getId() const;

  //! Get the number of attached observers
  unsigned getNumberOfObservers() const;

protected:

  // The observer map
  typedef typename boost::unordered_map<ModuleTraits::InternalEstimatorHandle,
					Teuchos::RCP<Observer> > ObserverIdMap;


  // Get the observer map
  ObserverIdMap& observer_id_map();
  
private:

  // The entity id for which the particle event will be dispatched
  EntityHandle d_entity_id;

  ObserverIdMap d_observer_map;
};

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// Template Includes.
//---------------------------------------------------------------------------//

#include "MonteCarlo_ParticleEventDispatcher_def.hpp"

//---------------------------------------------------------------------------//

#endif // end FACEMC_PARTICLE_EVENT_DISPATCHER_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_ParticleEventDispatcher.hpp
//---------------------------------------------------------------------------//
