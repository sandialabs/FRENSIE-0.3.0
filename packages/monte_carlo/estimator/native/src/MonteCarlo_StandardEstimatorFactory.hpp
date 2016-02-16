//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_StandardEstimatorFactory.hpp
//! \author Alex Robinson
//! \brief  The standard estimator factory class declaration
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_STANDARD_ESTIMATOR_FACTORY_DAGMC_HPP
#define MONTE_CARLO_STANDARD_ESTIMATOR_FACTORY_DAGMC_HPP

// FRENSIE Includes
#include "MonteCarlo_EstimatorFactory.hpp"

namespace MonteCarlo{

//! The standard estimator factory class
template<typename GeometryHandler>
class StandardEstimatorFactory : public EstimatorFactory
{
  
public:

  //! Constructor
  StandardEstimatorFactory( 
          const std::shared_ptr<EventHandler>& event_handler,
          const boost::unordered_map<unsigned,Teuchos::RCP<ResponseFunction> >&
          response_function_id_map,
          std::ostream* os_warn = &std::cerr )
  { GeometryHandler::geometry_handler_is_missing_specialization(); }

  //! Destructor
  ~StandardEstimatorFactory()
  { /* ... */ }

  //! Create and register an estimator
  void createAndRegisterEstimator(const Teuchos::ParameterList& estimator_rep )
  { GeometryHandler::geometry_handler_is_missing_specialization(); }

  //! Create and register cached estimators
  void createAndRegisterCachedEstimators()
  { GeometryHandler::geometry_handler_is_missing_specialization(); }
};

//! Helper function for creating an estimatory factory instance
template<typename GeometryHandler>
inline std::shared_ptr<EstimatorFactory>
getEstimatorHandlerFactoryInstance( 
          const std::shared_ptr<EventHandler>& event_handler,
          const boost::unordered_map<unsigned,Teuchos::RCP<ResponseFunction> >&
          response_function_id_map,
          std::ostream* os_warn = &std::cerr )
{
  return std::shared_ptr<EstimatorFactory>( 
       new StandardEstimatorFactory<GeometryHandler>( event_handler,
                                                      response_function_id_map,
                                                      os_warn ) );
}

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_STANDARD_ESTIMATOR_FACTORY_DAGMC_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_StandardEstimatorFactory.hpp
//---------------------------------------------------------------------------//
