//---------------------------------------------------------------------------//
//!
//! \file   DataGen_FormFactorSquaredEvaluator_def.hpp
//! \author Alex Robinson
//! \brief  The form factor squared evaluator declaration
//!
//---------------------------------------------------------------------------//

#ifndef DATA_GEN_FORM_FACTOR_SQUARED_EVALUATOR_DEF_HPP
#define DATA_GEN_FORM_FACTOR_SQUARED_EVALUATOR_DEF_HPP

// FRENSIE Includes
#include "MonteCarlo_StandardFormFactor.hpp"
#include "Utility_TabularDistribution.hpp"
#include "Utility_SortAlgorithms.hpp"
#include "Utility_ContractException.hpp"

namespace DataGen{

// Construction helper
template<typename InterpPolicy,
         typename GridArgumentUnit,
         template<typename,typename...> class Array>
std::shared_ptr<FormFactorSquaredEvaluator>
FormFactorSquaredEvaluator::createEvaluator(
                                      const Array<double>& argument_grid,
                                      const Array<double>& form_factor_values )
{
  // Make sure the argument grid is valid
  testPrecondition( argument_grid.front() >= 0.0 );
  testPrecondition( Utility::Sort::isSortedAscending( argument_grid.begin(),
                                                      argument_grid.end() ) );
  // Make sure the form factor values are valid
  testPrecondition( form_factor_values.back() >= 0.0 );
  testPrecondition( Utility::Sort::isSortedAscending(
                                                 form_factor_values.rbegin(),
                                                 form_factor_values.rend() ) );

  // Create the raw form factor
  std::shared_ptr<Utility::UnitAwareOneDDistribution<GridArgumentUnit,void> >
    raw_form_factor( new Utility::UnitAwareTabularDistribution<InterpPolicy,GridArgumentUnit,void>(
                                                        argument_grid,
                                                        form_factor_values ) );

  std::unique_ptr<const MonteCarlo::FormFactor> form_factor(
                         new MonteCarlo::StandardFormFactor<GridArgumentUnit>(
                                                           raw_form_factor ) );

  return std::shared_ptr<FormFactorSquaredEvaluator>(
                               new FormFactorSquaredEvaluator( form_factor ) );
}

} // end DataGen namespace

#endif // end DATA_GEN_FORM_FACTOR_SQUARED_EVALUATOR_DEF_HPP

//---------------------------------------------------------------------------//
// end DataGen_FormFactorSquaredEvaluator_def.hpp
//---------------------------------------------------------------------------//
