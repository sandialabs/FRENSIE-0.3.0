//---------------------------------------------------------------------------//
//!
//! \file   tstParticleLeavingCellEventDispatcher.cpp
//! \author Alex Robinson
//! \brief  Particle leaving cell event dispatcher unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_Array.hpp>
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "MonteCarlo_CellPulseHeightEstimator.hpp"
#include "MonteCarlo_ParticleLeavingCellEventDispatcher.hpp"
#include "MonteCarlo_PhotonState.hpp"
#include "Geometry_ModuleTraits.hpp"

//---------------------------------------------------------------------------//
// Testing Variables
//---------------------------------------------------------------------------//
Teuchos::RCP<MonteCarlo::CellPulseHeightEstimator<MonteCarlo::WeightMultiplier> >
estimator_1;

Teuchos::RCP<MonteCarlo::CellPulseHeightEstimator<MonteCarlo::WeightAndEnergyMultiplier> >
estimator_2;

Teuchos::RCP<MonteCarlo::ParticleLeavingCellEventDispatcher> dispatcher(
			new MonteCarlo::ParticleLeavingCellEventDispatcher( 0 ) );

//---------------------------------------------------------------------------//
// Testing Functions.
//---------------------------------------------------------------------------//
// Initialize the estimator
template<typename CellPulseHeightEstimator>
void initializeCellPulseHeightEstimator(
			    const unsigned estimator_id,
			    Teuchos::RCP<CellPulseHeightEstimator>& estimator )
{
  // Set the entity ids
  Teuchos::Array<Geometry::ModuleTraits::InternalCellHandle>
    cell_ids( 2 );
  cell_ids[0] = 0;
  cell_ids[1] = 1;

  // Set the estimator multiplier
  double estimator_multiplier = 1.0;

  estimator.reset( new CellPulseHeightEstimator( estimator_id,
						 estimator_multiplier,
						 cell_ids ) );

  Teuchos::Array<MonteCarlo::ParticleType> particle_types( 1 );
  particle_types[0] = MonteCarlo::PHOTON;
  
  estimator->setParticleTypes( particle_types );
}

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that the cell id associated with the dispatcher can be returned
TEUCHOS_UNIT_TEST( ParticleLeavingCellEventDispatcher, getCellId )
{
  TEST_EQUALITY_CONST( dispatcher->getId(), 0 );
}

//---------------------------------------------------------------------------//
// Check that the number of observers attached to the disp. can be returned
TEUCHOS_UNIT_TEST( ParticleLeavingCellEventDispatcher, 
		   getNumberOfObservers )
{
  TEST_EQUALITY_CONST( dispatcher->getNumberOfObservers(), 0 );
}

//---------------------------------------------------------------------------//
// Check that an observer can be attached to the dispatcher
TEUCHOS_UNIT_TEST( ParticleLeavingCellEventDispatcher, attachObserver )
{
  initializeCellPulseHeightEstimator( 0u, estimator_1 );
  initializeCellPulseHeightEstimator( 1u, estimator_2 );

  Teuchos::RCP<MonteCarlo::ParticleLeavingCellEventObserver> observer_1 =
    Teuchos::rcp_dynamic_cast<MonteCarlo::ParticleLeavingCellEventObserver>( 
								 estimator_1 );
  Teuchos::RCP<MonteCarlo::ParticleLeavingCellEventObserver> observer_2 =
    Teuchos::rcp_dynamic_cast<MonteCarlo::ParticleLeavingCellEventObserver>(
								 estimator_2 );
  
  dispatcher->attachObserver( estimator_1->getId(), observer_1 );
  dispatcher->attachObserver( estimator_2->getId(), observer_2 );
  
  observer_1.reset();
  observer_2.reset();

  TEST_EQUALITY_CONST( estimator_1.total_count(), 2 );
  TEST_EQUALITY_CONST( estimator_2.total_count(), 2 );
  TEST_EQUALITY_CONST( dispatcher->getNumberOfObservers(), 2 );
}

//---------------------------------------------------------------------------//
// Check that a collision event can be dispatched
TEUCHOS_UNIT_TEST( ParticleLeavingCellEventDispatcher, 
		   dispatchParticleLeavingCellEvent )
{
  MonteCarlo::PhotonState particle( 0ull );
  particle.setWeight( 1.0 );
  particle.setEnergy( 1.0 );

  TEST_ASSERT( !estimator_1->hasUncommittedHistoryContribution() );
  TEST_ASSERT( !estimator_2->hasUncommittedHistoryContribution() );

  dispatcher->dispatchParticleLeavingCellEvent( particle, 0 );

  TEST_ASSERT( estimator_1->hasUncommittedHistoryContribution() );
  TEST_ASSERT( estimator_2->hasUncommittedHistoryContribution() );
}

//---------------------------------------------------------------------------//
// Check that an observer can be detached from the dispatcher
TEUCHOS_UNIT_TEST( ParticleLeavingCellEventDispatcher, detachObserver )
{
  dispatcher->detachObserver( 0u );

  TEST_EQUALITY_CONST( estimator_1.total_count(), 1 );
  TEST_EQUALITY_CONST( estimator_2.total_count(), 2 );
  TEST_EQUALITY_CONST( dispatcher->getNumberOfObservers(), 1 );

  dispatcher->detachObserver( 1u );

  TEST_EQUALITY_CONST( estimator_1.total_count(), 1 );
  TEST_EQUALITY_CONST( estimator_2.total_count(), 1 );
  TEST_EQUALITY_CONST( dispatcher->getNumberOfObservers(), 0 );  

  // Remove nonexistent estimator
  dispatcher->detachObserver( 2u );

  TEST_EQUALITY_CONST( estimator_1.total_count(), 1 );
  TEST_EQUALITY_CONST( estimator_2.total_count(), 1 );
  TEST_EQUALITY_CONST( dispatcher->getNumberOfObservers(), 0 );  
}

//---------------------------------------------------------------------------//
// end tstParticleLeavingCellEventDispatcher.cpp
//---------------------------------------------------------------------------//
