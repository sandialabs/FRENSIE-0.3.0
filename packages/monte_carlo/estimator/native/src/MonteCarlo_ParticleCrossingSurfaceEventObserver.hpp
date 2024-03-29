//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ParticleCrossingSurfaceEventObserver.hpp
//! \author Alex Robinson
//! \brief  Particle crossing surface event observer base class declaration
//!
//---------------------------------------------------------------------------//

#ifndef FACEMC_PARTICLE_CROSSING_SURFACE_EVENT_OBSERVER_HPP
#define FACEMC_PARTICLE_CROSSING_SURFACE_EVENT_OBSERVER_HPP

// Trilinos Includes
#include <Teuchos_Array.hpp>

// FRENSIE Includes
#include "MonteCarlo_ModuleTraits.hpp"
#include "MonteCarlo_ParticleState.hpp"
#include "Geometry_ModuleTraits.hpp"

namespace MonteCarlo{

//! The particle crossing surface event observer base class
class ParticleCrossingSurfaceEventObserver
{

public:

  //! The event tag type
  struct ParticleCrossingSurfaceEvent{};

  //! Typedef for the observer event tag
  typedef ParticleCrossingSurfaceEvent EventTag;

  //! Constructor
  ParticleCrossingSurfaceEventObserver()
  { /* ... */ }

  //! Destructor
  virtual ~ParticleCrossingSurfaceEventObserver()
  { /* ... */ }

  //! Update the observer
  virtual void updateFromParticleCrossingSurfaceEvent(
	  const ParticleState& particle,
	  const Geometry::ModuleTraits::InternalSurfaceHandle surface_crossing,
	  const double angle_cosine ) = 0;
};
  
} // end MonteCarlo namespace

#endif // end FACEMC_PARTICLE_CROSSING_SURFACE_EVENT_OBSERVER_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_ParticleCrossingSurfaceEventObserver.hpp
//---------------------------------------------------------------------------//
