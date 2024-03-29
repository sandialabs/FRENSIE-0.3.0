//---------------------------------------------------------------------------//
//!
//! \file   Utility_EquiprobableBinDistribution_def.hpp
//! \author Alex Robinson
//! \brief  Equiprobable bin distribution class definition.
//!
//---------------------------------------------------------------------------//

#ifndef UTILITY_EQUIPROBABLE_BIN_DISTRIBUTION_DEF_HPP
#define UTILITY_EQUIPROBABLE_BIN_DISTRIBUTION_DEF_HPP

// FRENSIE Includes
#include "Utility_SearchAlgorithms.hpp"
#include "Utility_SortAlgorithms.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_ArrayString.hpp"
#include "Utility_ExceptionTestMacros.hpp"
#include "Utility_ContractException.hpp"

namespace Utility{

// Default constructor
template<typename IndependentUnit, typename DependentUnit>
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::UnitAwareEquiprobableBinDistribution()
{ /* ... */ }

// Basic constructor
template<typename IndependentUnit, typename DependentUnit>
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::UnitAwareEquiprobableBinDistribution( 
				 const Teuchos::Array<double>& bin_boundaries )
  : d_bin_boundaries( bin_boundaries.size() )
{
  this->initializeDistribution( bin_boundaries );
}

// Constructor
template<typename IndependentUnit, typename DependentUnit>
template<typename InputIndepQuantity>
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::UnitAwareEquiprobableBinDistribution( 
	           const Teuchos::Array<InputIndepQuantity>& bin_boundaries )
  : d_bin_boundaries( bin_boundaries.size() )
{
  this->initializeDistribution( bin_boundaries );
}

// Copy constructor
/*! \details Just like boost::units::quantity objects, the unit-aware 
 * distribution can be explicitly cast to a distribution with compatible
 * units. If the units are not compatible, this function will not compile. Note
 * that this allows distributions to be scaled safely (unit conversions 
 * are completely taken care of by boost::units)!
 */
template<typename IndependentUnit, typename DependentUnit>
template<typename InputIndepUnit, typename InputDepUnit>
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::UnitAwareEquiprobableBinDistribution(
  const UnitAwareEquiprobableBinDistribution<InputIndepUnit,InputDepUnit>& dist_instance )
  : d_bin_boundaries()
{ 
  this->initializeDistribution( dist_instance.d_bin_boundaries );
}

// Copy constructor (copying from unitless distribution only)
template<typename IndependentUnit, typename DependentUnit>
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::UnitAwareEquiprobableBinDistribution( 
 const UnitAwareEquiprobableBinDistribution<void,void>& unitless_dist_instance, int )
  : d_bin_boundaries()
{
  this->initializeDistribution( unitless_dist_instance.d_bin_boundaries );
}

// Construct distribution from a unitless dist. (potentially dangerous)
/*! \details Constructing a unit-aware distribution from a unitless 
 * distribution is potentially dangerous. By forcing users to construct objects
 * using this method instead of a standard constructor we are trying to make
 * sure users are aware of the danger. This is designed to mimic the interface 
 * of the boost::units::quantity, which also has to deal with this issue. 
 */
template<typename IndependentUnit, typename DependentUnit>
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit> 
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::fromUnitlessDistribution( 
 const UnitAwareEquiprobableBinDistribution<void,void>& unitless_distribution )
{
  return ThisType( unitless_distribution, 0 );
}

// Assignment operator
template<typename IndependentUnit, typename DependentUnit>
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>& 
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::operator=(
		             const UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>& dist_instance )
{
  if( this != &dist_instance )
    d_bin_boundaries = dist_instance.d_bin_boundaries;

  return *this;
}


// Evaulate the distribution
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::DepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::evaluate( 
  const typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity indep_var_value ) const
{
  return QuantityTraits<DepQuantity>::initializeQuantity(
		      getRawQuantity( this->evaluatePDF( indep_var_value ) ) );
}

// Evaluate the PDF
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::InverseIndepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::evaluatePDF(
    const typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity indep_var_value ) const
{
  if( indep_var_value < d_bin_boundaries.front() )
    return QuantityTraits<InverseIndepQuantity>::zero();
  else if( indep_var_value > d_bin_boundaries.back() )
    return QuantityTraits<InverseIndepQuantity>::zero();
  else
  {
    typename Teuchos::Array<IndepQuantity>::const_iterator lower_bin_boundary,
      upper_bin_boundary;
    
    if( indep_var_value >= d_bin_boundaries.front() &&
	indep_var_value < d_bin_boundaries.back() )
    {
      lower_bin_boundary = 
	Search::binaryLowerBound( d_bin_boundaries.begin(),
				  d_bin_boundaries.end(),
				  indep_var_value );

     upper_bin_boundary = lower_bin_boundary;
     ++upper_bin_boundary;
    }
    else
    {
      upper_bin_boundary = d_bin_boundaries.end();
      --upper_bin_boundary;
      
      lower_bin_boundary = upper_bin_boundary;
      --lower_bin_boundary;
    }
    
    return (1.0/(d_bin_boundaries.size()-1))/
      (*upper_bin_boundary - *lower_bin_boundary);
  }
}

// Evaluate the CDF
template<typename IndependentUnit, typename DependentUnit>
double UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::evaluateCDF( 
   const typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity indep_var_value ) const
{
  if( indep_var_value < d_bin_boundaries.front() )
    return 0.0;
  else if( indep_var_value >= d_bin_boundaries.back() )
    return 1.0;
  else
  {
    unsigned bin_index = 
      Search::binaryLowerBoundIndex( d_bin_boundaries.begin(),
				     d_bin_boundaries.end(),
				     indep_var_value );

    unsigned bins = d_bin_boundaries.size()-1;
    
    double bin_contribution = (indep_var_value - d_bin_boundaries[bin_index])/
      (d_bin_boundaries[bin_index+1] - d_bin_boundaries[bin_index]);

    return (bin_index + bin_contribution)/bins;
  }
}

// Return a random sample from the distribution
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity 
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::sample() const
{
  double random_number = RandomNumberGenerator::getRandomNumber<double>();

  unsigned dummy_index;

  return this->sampleImplementation( random_number, dummy_index );
}

// Return a random sample and record the number of trials
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::sampleAndRecordTrials( unsigned& trials ) const
{
  ++trials;

  return this->sample();
}

// Return a random sample from the distribution and the sampled index 
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity 
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::sampleAndRecordBinIndex( 
					    unsigned& sampled_bin_index ) const
{
  double random_number = RandomNumberGenerator::getRandomNumber<double>();

  return this->sampleImplementation( random_number, sampled_bin_index );
}

// Return a random sample from the distribution at the given CDF value
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::sampleWithRandomNumber( 
					     const double random_number ) const
{
  // Make sure the random number is valid
  testPrecondition( random_number >= 0.0 );
  testPrecondition( random_number <= 1.0 );

  unsigned dummy_index;

  return this->sampleImplementation( random_number, dummy_index );
}

// Return a random sample from the distribution in a subrange
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::sampleInSubrange( 
 const typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity max_indep_var ) const
{
  // Make sure the max independent variable is valid
  testPrecondition( max_indep_var >= d_bin_boundaries.front() );

  double random_number = RandomNumberGenerator::getRandomNumber<double>();

  return this->sampleWithRandomNumberInSubrange( random_number,
						 max_indep_var );
}

// Return a random sample using the random number and record the bin index
template<typename IndependentUnit, typename DependentUnit>
inline typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::sampleImplementation( 
				            double random_number,
				            unsigned& sampled_bin_index ) const
{
  // Make sure the random number is valid
  testPrecondition( random_number >= 0.0 );
  testPrecondition( random_number <= 1.0 );

  double bin_location = random_number*(d_bin_boundaries.size()-1);
  
  sampled_bin_index = (unsigned)floor(bin_location);
  
  return d_bin_boundaries[sampled_bin_index] + 
    (bin_location - sampled_bin_index)*(d_bin_boundaries[sampled_bin_index+1]-
					d_bin_boundaries[sampled_bin_index]);
}

// Return a random sample from the distribution at the given CDF value in a subrange
template<typename IndependentUnit, typename DependentUnit>
inline typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::sampleWithRandomNumberInSubrange( 
     const double random_number,
     const typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity max_indep_var ) const
{
  // Make sure the random number is valid
  testPrecondition( random_number >= 0.0 );
  testPrecondition( random_number <= 1.0 );
  // Make sure the max independent variable is valid
  testPrecondition( max_indep_var >= d_bin_boundaries.front() );

  // Compute the scaled random number
  double scaled_random_number = 
    random_number*this->evaluateCDF( max_indep_var );

  unsigned dummy_index;

  return this->sampleImplementation( scaled_random_number, dummy_index );
}

// Return the upper bound of the distribution independent variable
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::getUpperBoundOfIndepVar() const
{
  return d_bin_boundaries.back();
}

// Return the lower bound of the distribution independent variable
template<typename IndependentUnit, typename DependentUnit>
typename UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::IndepQuantity
UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::getLowerBoundOfIndepVar() const
{
  return d_bin_boundaries.front();
}

// Return the distribution type
template<typename IndependentUnit, typename DependentUnit>
OneDDistributionType UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::getDistributionType() const
{
  return UnitAwareEquiprobableBinDistribution::distribution_type;
}

// Test if the distribution is continuous
template<typename IndependentUnit, typename DependentUnit>
bool UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::isContinuous() const
{
  return true;
}

// Method for placing the object in an output stream
template<typename IndependentUnit, typename DependentUnit>
void UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::toStream( std::ostream& os ) const
{
  Teuchos::Array<double> raw_bin_boundaries( d_bin_boundaries.size() );

  for( unsigned i = 0; i < d_bin_boundaries.size(); ++i )
    raw_bin_boundaries[i] = getRawQuantity( d_bin_boundaries[i] );

  os << raw_bin_boundaries;
}

// Method for initializing the object from an input stream
template<typename IndependentUnit, typename DependentUnit>
void UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::fromStream( std::istream& is )
{
  std::string bin_boundaries_rep;
  std::getline( is, bin_boundaries_rep, '}' );
  bin_boundaries_rep += "}";
  
  // Parse special characters
  try{
    ArrayString::locateAndReplacePi( bin_boundaries_rep );
    ArrayString::locateAndReplaceIntervalOperator( bin_boundaries_rep );
  }
  EXCEPTION_CATCH_RETHROW_AS( std::runtime_error,
			      InvalidDistributionStringRepresentation,
			      "Error: the equiprobable bin distribution "
			      "cannot be constructed because the "
			      "representation is not valid (see details "
			      "below)!\n" );

  try{
    this->initializeDistribution(
		    Teuchos::fromStringToArray<double>( bin_boundaries_rep ) );
  }
  EXCEPTION_CATCH_RETHROW_AS( Teuchos::InvalidArrayStringRepresentation,
			      InvalidDistributionStringRepresentation,
			      "Error: the equiprobable bin distribution "
			      "cannot be constructed because the "
			      "representation is not valid (see details "
			      "below)!\n" );
  
  TEST_FOR_EXCEPTION( d_bin_boundaries.size() <= 1,
		      InvalidDistributionStringRepresentation, 
		      "Error: the equiprobable bin distribution cannot be "
		      "constructed because at least one bin (two boundaries) "
		      "is required!\n" );

  TEST_FOR_EXCEPTION( !Sort::isSortedAscending( d_bin_boundaries.begin(),
						d_bin_boundaries.end() ),
		      InvalidDistributionStringRepresentation, 
		      "Error: the equiprobable bin distribution cannot be "
		      "constructed because the bin boundaries "
		      << bin_boundaries_rep << " are not sorted!" );
}

// Method for testing if two objects are equivalent
template<typename IndependentUnit, typename DependentUnit>
bool UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::isEqual( 
		      const UnitAwareEquiprobableBinDistribution& other ) const
{
  return d_bin_boundaries == other.d_bin_boundaries;
}

// Initialize the distribution
template<typename IndependentUnit, typename DependentUnit>
void UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::initializeDistribution( 
				 const Teuchos::Array<double>& bin_boundaries )
{
  // Make sure there is at least one bin
  testPrecondition( bin_boundaries.size() > 1 );
  // Make sure that the bins are sorted
  testPrecondition( Sort::isSortedAscending( bin_boundaries.begin(),
					     bin_boundaries.end() ) );
  
  // Resize the bin boundaries array
  d_bin_boundaries.resize( bin_boundaries.size() );

  // Copy the bin boundaries 
  for( unsigned i = 0; i < bin_boundaries.size(); ++i )
    setQuantity( d_bin_boundaries[i], bin_boundaries[i] );
}

// Initialize the distribution
template<typename IndependentUnit, typename DependentUnit>
template<typename InputIndepQuantity>
void UnitAwareEquiprobableBinDistribution<IndependentUnit,DependentUnit>::initializeDistribution( 
		     const Teuchos::Array<InputIndepQuantity>& bin_boundaries )
{
  // Make sure there is at least one bin
  testPrecondition( bin_boundaries.size() > 1 );
  // Make sure that the bins are sorted
  testPrecondition( Sort::isSortedAscending( bin_boundaries.begin(),
					     bin_boundaries.end() ) );

  // Resize the bin boundaries array
  d_bin_boundaries.resize( bin_boundaries.size() );

  // Explicitly cast each bin boundary to the desired quantity
  for( unsigned i = 0; i < bin_boundaries.size(); ++i )
    d_bin_boundaries[i] = IndepQuantity( bin_boundaries[i] );
}

} // end Utility namespace

#endif // end UTILITY_EQUIPROBABLE_BIN_DISTRIBUTION_DEF_HPP

//---------------------------------------------------------------------------//
// end Utility_EquiprobableBinDistribution_def.hpp
//---------------------------------------------------------------------------//
