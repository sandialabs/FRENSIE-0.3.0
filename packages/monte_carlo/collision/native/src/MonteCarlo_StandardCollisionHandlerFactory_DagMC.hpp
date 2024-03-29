//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_StandardCollisionHandlerFactory_DagMC.hpp
//! \author Alex Robinson, Eli Moll
//! \brief  Standard collision handler factory class declaration.
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_STANDARD_COLLISION_HANDLER_FACTORY_DAGMC_HPP
#define MONTE_CARLO_STANDARD_COLLISION_HANDLER_FACTORY_DAGMC_HPP

// Std Lib Includes
#include <stdexcept>

// Trilinos Includes
#include <Teuchos_RCP.hpp>
#include <Teuchos_ParameterList.hpp>

// Moab Includes
#include "DagMC.hpp"

// FRENSIE Includes
#include "MonteCarlo_CollisionHandler.hpp"
#include "MonteCarlo_StandardCollisionHandlerFactory.hpp"
#include "MonteCarlo_AtomicRelaxationModelFactory.hpp"
#include "MonteCarlo_IncoherentModelType.hpp"
#include "MonteCarlo_BremsstrahlungAngularDistributionType.hpp"

namespace MonteCarlo{

/*! The specialization of the CollimatorHandlerFactory class for the DagMC 
 * geometry handler.
 * \ingroup estimator_module
 */ 
template<>
class StandardCollisionHandlerFactory<moab::DagMC> : public CollisionHandlerFactory
{

public:

  // Constructor
  StandardCollisionHandlerFactory( std::ostream* os_warn = &std::cerr )
    : CollisionHandlerFactory( os_warn )
  { /* ... */ }

  // Destructor
  ~StandardCollisionHandlerFactory()
  { /* ... */ }

protected:

  //! Validate the material ids
  void validateMaterialIds( const Teuchos::ParameterList& material_reps ) const;

  //! Create the cell id data maps
  void createCellIdDataMaps(
  boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                       std::vector<std::string> >& cell_id_mat_id_map,
  boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
	               std::vector<std::string> >& cell_id_density_map ) const;
};

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_STANDARD_COLLISION_HANDLER_FACTORY_DAGMC_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_StandardCollisionHandlerFactory_DagMC.hpp
//---------------------------------------------------------------------------//
