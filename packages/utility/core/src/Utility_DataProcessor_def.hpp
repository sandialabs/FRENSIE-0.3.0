//---------------------------------------------------------------------------//
//!
//! \file   Utility_DataProcessor_def.hpp
//! \author Alex Robinson
//! \brief  Data Processor base class template definitions
//!
//---------------------------------------------------------------------------//

#ifndef UTILITY_DATA_PROCESSOR_DEF_HPP
#define UTILITY_DATA_PROCESSOR_DEF_HPP

// Std Lib Includes
#include <cmath>
#include <limits>
#include <iterator>

// FRENSIE Includes
#include "Utility_Tuple.hpp"
#include "Utility_ContractException.hpp"

namespace Utility{

/*! \details This function processes the independent and dependent data points
 * in a table of continuous data using the desired data processing policy (see
 * Utility::LogLogDataProcessingPolicy or 
 * Utility::SquareSquareDataProcessingPolicy). Since this data
 * will be stored in a Teuchos::Array of Tuples (Utility::Pair, Utility::Trip,
 * or Utility::Quad) this function must also know which members of the tuple
 * store the raw independent data values and the raw dependent data values. 
 * This function will only compile if the desired tuple members are actually 
 * available in Tuple.
 * \tparam DataProcessingPolicy A policy class that is used to process the
 * independent and dependent data values in the data table. Only two such
 * policies are provided by the DataProcessor. However, any policy class that
 * adheres to the necessary interface can be used (the static members 
 * processIndependentVar and processDependentVar must be defined or a compile
 * time error will be given).
 * \tparam indepMember A member of the enumeration Utility::TupleMember, 
 * which is used to refer to the member of Tuple that stores the independent 
 * data values.
 * \tparam depMember A member of the enumeration Utility::TupleMember, 
 * which is used to refer to the member of Tuple that stores the dependent 
 * data values.
 * \tparam Tuple A Utility tuple struct (either Utility::Pair, Utility::Trip, or
 * Utility::Quad) that is used to store processed data.
 * \param[in,out] data The array of tuple structs which contains the 
 * raw table data.
 * \pre A valid array, which is any array of tuples with at least one element,
 * must be given to this function.
 * \note Developers: The Utility::TupleMemberTraits is critical to the
 * generality of this function. Review this struct to better understand how
 * this function operates. 
 */
template<typename DataProcessingPolicy,
	 TupleMember indepMember,
	 TupleMember depMember,
	 typename Tuple>
void DataProcessor::processContinuousData( Teuchos::Array<Tuple> &data )
{
  // Make sure that the array is valid
  testPrecondition( data.size() > 0 );

  typename Teuchos::Array<Tuple>::iterator data_point = data.begin();
  typename Teuchos::Array<Tuple>::iterator end = data.end();

  typename TupleMemberTraits<Tuple,indepMember>::tupleMemberType 
    indep_value;
  typename TupleMemberTraits<Tuple,depMember>::tupleMemberType
    dep_value;

  while( data_point != end )
  {
    indep_value = DataProcessingPolicy::processIndependentVar( 
					     get<indepMember>( *data_point ) );
    dep_value = DataProcessingPolicy::processDependentVar( 
					       get<depMember>( *data_point ) );

    set<indepMember>( *data_point, indep_value );
    set<depMember>( *data_point, dep_value );

    ++data_point;
  }
}

/*! \details This function removes elements of an array if the specified tuple
 * member is less than the given value. This function is primarily used to 
 * remove data points from a table with independent values below some
 * threshold. For instance, photon data tables can contain data for energies
 * in the eV range. Typically, one is only interested in photons with energies
 * in the keV range and above. All data below the keV range can therefore be
 * ignored. To allow for accurate interpolation, the data point closest to the
 * given value but below it will be kept. If the tuple member of interest does 
 * not exist in Tuple, a compile time error will be given.
 * \tparam member A member of the enumeration Utility::TupleMember, which
 * is used to refer to the member of Tuple that stores the value that will be
 * tested against the value of interest.
 * \tparam Tuple A Utility tuple struct (either Utility::Pair, Utility::Trip, or
 * Utility::Quad) that is used to store the table data.
 * \param[in,out] data The array of tuple structs which contain the raw table
 * data. 
 * \param[in] value The minimum value that all Tuples in the array will be 
 * compared against. Tuples with Utility::TupleMember <em> member </em> less 
 * than <b> value </b> will be removed from the array.
 * \pre A valid array, which is any array of tuples with at least one element,
 * must be given to this function.
 * \post A valid array, which is any array of tuples with at least one element,
 * must be returned from this function. If the testing value is set too high
 * it is possible for all elements to be eliminated from the array, which is
 * surely not an intended results.
 * \note Developers: The Utility::TupleMemberTraits struct is critical 
 * to the generality of this function. Review this struct to better understand 
 * how this function operates. 
 */
template<TupleMember member,
	 typename Tuple>
void DataProcessor::removeElementsLessThanValue( Teuchos::Array<Tuple> &data,
						 const double value )
{
  // Make sure that the array is valid
  testPrecondition( data.size() > 0 );
  // Make sure that array is sorted (need a check for this)
  
  typename Teuchos::Array<Tuple>::iterator data_point_1 = data.begin();
  typename Teuchos::Array<Tuple>::iterator data_point_2 = data_point_1 + 1;
  typename Teuchos::Array<Tuple>::iterator end = data.end();
  
  // Loop through the array and find the element range to remove
  typename Teuchos::Array<Tuple>::size_type max_index = 0;
  while( data_point_2 != end )
  {
    if( get<member>( *data_point_1 ) < value &&
	get<member>( *data_point_2 ) <= value )
      ++max_index;
    else
      break;

    ++data_point_1;
    ++data_point_2;
  }

  // Remove the elements from the array
  data_point_1 = data.begin();
  data_point_2 = data.begin() + max_index;
  data.erase( data_point_1, data_point_2 );

  // Make sure that the array is still valid
  testPostcondition( data.size() > 0 );
}

/*! \details This function removes elements of an array if the specified tuple
 * member is greater than the given value. This function is primarily used to
 * remove data points from a table with independent values above some threshold.
 * For instance, photon data tables can contain data for energies in the GeV
 * range. Typically, one is only interested in photons with energies in the
 * MeV range and below. All data above a few tens of MeV can therefore be 
 * ignored. To allow for accurate interpolation, the data point closest to the
 * give value but above it will be kept. If the tuple member of interest does
 * not exist in Tuple, a compile time error will be given.
 * \tparam member A member of the enumeration Utility::TupleMember, which
 * is used to refer to the member of Tuple that stores the value that will be
 * tested against the value of interest. 
 * \tparam Tuple A Utility tuple struct (either Utility::Pair, Utility::Trip, or
 * Utility::Quad) that is used to store the table data.
 * \param[in,out] data The array of tuple structs which contain the raw table
 * data.
 * \param[in] The maximum value that all Tuples in the array will be compared
 * against. Tuples with Utility::TupleMember <em> member </em> greater than 
 * <b> value </b> will be removed from the array.
 * \pre A valid array, which is any array of tuples with at least one element,
 * must be given to this function. 
 * \post A valid array, which is any array of tuples with at least one element,
 * must be returned from this function. If the testing value is set too low it
 * is possible for all elements to be eliminated from the array, which is surely
 * not an intended result.
 * \note Developers: The Utility::TupleMemberTraits struct is critical 
 * to the generality of this function. Review this struct to better understand 
 * how this function operates.
 */
template<TupleMember member,
	 typename Tuple>
void DataProcessor::removeElementsGreaterThanValue( Teuchos::Array<Tuple> &data,
						    const double value )
{
  // Make sure that the array is valid
  testPrecondition( data.size() > 0 );
  // Make sure that array is sorted (need a check for this)
  
  // Use bidirectional iterator in reverse to improve performance
  typename Teuchos::Array<Tuple>::iterator data_point_1 = data.end() - 1;
  typename Teuchos::Array<Tuple>::iterator data_point_2 = data_point_1 - 1;
  typename Teuchos::Array<Tuple>::iterator end = data.begin() - 1;
  
  // Loop through the array and find the element range to remove
  typename Teuchos::Array<Tuple>::size_type max_index = 0;
  while( data_point_2 != end )
  {
    if( get<member>( *data_point_1 ) > value &&
        get<member>( *data_point_2 ) >= value )
      ++max_index;
    else
      break;

    --data_point_1;
    --data_point_2;
  }

  // Remove the elements from the array
  data_point_2 = data.end();
  data_point_1 = data_point_2 - max_index;
  data.erase( data_point_1, data_point_2 );

  // Make sure that the array is still valid
  testPostcondition( data.size() > 0 );
}

/*! \details This function removes an adjacent data point if the values of
 * interest are the same (constant region). By eliminating the amount of data
 * points in a table the memory requirements and the search time can be 
 * reduced. If the tuple member of interest does not exist in Tuple, a compile
 * time error will be given.
 * \tparam member A member of the enumeration Utility::TupleMember, which is used
 * to refer to the member of Tuple that stores the value that will be tested.
 * \tparam Tuple A Utility tuple struct (either Utility::Pair, Utility::Trip,
 * Utility::Quad) that is used to store the table data. 
 * \param[in,out] data The array of tuple structs which contain the raw table 
 * data.
 * \pre A valid array, which is any array of tuples with at least <em> three
 * </em> elements, must be given to this function.
 * \post A valid array, which is any array of tuples with at least <em> two
 * </em> elements, must be returned from this function.
 * \note Developers: The Utility::TraitsTupleMemberTraits struct is critical to 
 * the generality of this function. Review this struct to better understand how
 * this function operates. 
 */ 
template<TupleMember member,
	 typename Tuple>
void DataProcessor::coarsenConstantRegions( Teuchos::Array<Tuple> &data )
{
  // Make sure that the array is valid
  testPrecondition( data.size() > 2 );

  // Use bidirectional iterator in reverse to improve performance
  // when the constant region is at the end of the array
  typename Teuchos::Array<Tuple>::iterator data_point_1 = data.end()-1;
  typename Teuchos::Array<Tuple>::iterator data_point_2, data_point_3;
  typename Teuchos::Array<Tuple>::iterator end = data.begin()-1;

  data_point_2 = data_point_1-1;
  data_point_3 = data_point_2-1;
  while( data_point_3 != end )
  {
    if( get<member>( *data_point_1 ) == get<member>( *data_point_2 ) &&
	get<member>( *data_point_1 ) == get<member>( *data_point_3 ) )
    {
      data_point_1 = data.erase( data_point_1 );
    }
    
    --data_point_1;
    data_point_2 = data_point_1-1;
    data_point_3 = data_point_2-1;
  }

  // Make sure that the array is still valid
  testPostcondition( data.size() > 1 );
}    
  
/*! \details This function calculates the slopes between each pair of data
 * points. Since this data will be stored in an Array of Tuples
 * (either Utility::Trip or Utility::Quad) this function must know which
 * members of the tuple store the processed independent data values and the
 * processed dependent data values. This function must also know which member
 * of the tuple will store the slope value that is calculated. The tuple with
 * the smaller independent variable will store the slope value for the pair.
 * This is desirable since conducting a search for a value in the table will
 * always return the lower bin boundary (the tuple with the smaller independent
 * variable). This function will only compile if the desired tuple members are
 * actually available in Tuple.
 * \tparam indepMember A member of the enumeration Utility::TupleMember, which
 * is used to refer to the member of Tuple that stores the independent data
 * values. 
 * \tparam depMember A member of the enumeration Utility::TupleMember, which
 * is used to refer to the member of Tuple that stores the dependent data
 * values. 
 * \tparam slopeMember A member of the enumeration Utility::TupleMember, which
 * is used to refer to the member of Tuple that stores the slope values. 
 * \tparam Array An array class of tuple structs (either Utility::Trip or 
 * Utility::Quad).
 * \param[in,out] data The array of tuple structs which contain the raw table
 * data.
 * \pre A valid array, which is any array of tuples with at least two elements,
 * must be given to this function.
 * \note Developers: 
 * <ul>
 *  <li> The Utility::TupleMemberTraits struct is critical to the
 *       generality of this function. Review this struct to better understand 
 *       how this function operates. 
 * </ul>
 */
template<TupleMember indepMember, 
	 TupleMember depMember,
	 TupleMember slopeMember,
	 typename Array>
void DataProcessor::calculateSlopes( Array &data  )
{
  // Make sure that the array has more than one element
  testPrecondition( (data.size() > 1) );

  typedef typename ArrayTraits<Array>::value_type Tuple;
  
  typename Array::iterator data_point_1, data_point_2;
  typename Array::iterator end = data.end();

  data_point_1 = data.begin();
  data_point_2 = data_point_1 + 1;

  typename TupleMemberTraits<Tuple,slopeMember>::tupleMemberType slope;
  
  while( data_point_2 != end )
  {
    slope = (get<depMember>( *data_point_2 )-get<depMember>( *data_point_1 ))/
      (get<indepMember>( *data_point_2 ) - get<indepMember>( *data_point_1 ));
    
    set<slopeMember>( *data_point_1, slope );
    set<slopeMember>( *data_point_2, 0 );

    ++data_point_1;
    ++data_point_2;
  }
}

/*! \details This function calculates a CDF from an array of continuous
 * data points. Since this data will be stored in an Array of Tuples (only
 * Utility::Quad) this function must know which members of the tuple store the
 * processed independent data values and the processed dependent data values
 * (PDF values). This function must also know which member of the tuple will
 * store the CDF value that is calculated. To calculate the CDF values a 
 * Taylor series expansion to second order is conducted on the function which
 * represents the CDF:
 * \f{align}{
 *   CDF(x) & \approx CDF(x_0) + CDF^{'}(x_0)*(x - x_0) + 
 *   \frac{1}{2}CDF^{''}(x_0)*(x - x_0)^2 \nonumber \\
 *   & = CDF(x_0) + PDF(x_0)*(x - x_0) + \frac{1}{2}\left[\frac{d}{dx} PDF(x)
 *   \right]\Bigg|_{x_0}(x - x_0)^2 \nonumber \\
 *   & = CDF(x_0) + PDF(x_0)*(x - x_0) + \frac{1}{2}\left[
 *   \frac{PDF(x_1) - PDF(x_0)}{x_1 - x_0} \right](x - x_0)^2. \nonumber
 * \f}
 * \tparam indepMember A member of the enumeration Utility::TupleMember, which is
 * used to refer to the member of Tuple that stores the independent data values.
 * \tparam pdfMember A member of the enumeration Utility::TupleMember, which is
 * used to refer to the member of Tuple that stores the PDF data values.
 * \tparam cdfMember A member of the enumeration Utility::TupleMember, which is
 * used to refer to the member of Tuple that will store the CDF data values.
 * \tparam Tuple A Utility tuple struct (only Utility::Quad) that is used to
 * store the processed data.
 * \tparam Array An array class of tuple structs (only Utility::Quad).
 * \param[in,out] data The array of tuple structs which contain the table data.
 * \pre A valid array, which is any array of tuples with at least two
 * elements, must be given to this function.
 * \note Developers:
 * <ul>
 *  <li> The Utility::TupleMemberTraits struct is critical to the
 *       generality of this function. Review this struct to better understand 
 *       how this function operates. 
 * </ul>
 */
template<TupleMember indepMember,
	 TupleMember pdfMember,
	 TupleMember cdfMember,
	 typename Array>
typename QuantityTraits<typename TupleMemberTraits<typename ArrayTraits<Array>::value_type,cdfMember>::tupleMemberType>::template GetQuantityToPowerType<-1>::type
DataProcessor::calculateContinuousCDF( Array &data,
				       const bool normalize )
{
  // Make sure that the array has more than one element
  testPrecondition( (data.size() > 1) );
  
  typedef typename ArrayTraits<Array>::value_type Tuple;
  typedef QuantityTraits<typename TupleMemberTraits<Tuple,cdfMember>::tupleMemberType> CDFQT;
  
  typename Array::iterator data_point_1, data_point_2;
  typename Array::iterator end = data.end();

  data_point_1 = data.begin();
  data_point_2 = data.begin() + 1;

  typename TupleMemberTraits<Tuple,cdfMember>::tupleMemberType 
    cdf_value;

  // Initialize the CDF
  set<cdfMember>( *data_point_1, CDFQT::zero() );
  
  // Calculate the CDF
  // CDF(x) = CDF(x1)+PDF(x1)*(x-x1)+0.5*(PDF(x2)-PDF(x1))/(x2-x1)*(x-x1)^2
  while( data_point_2 != end )
  {
    cdf_value = get<cdfMember>( *data_point_1 ) + 
      get<pdfMember>( *data_point_1 )*
      (get<indepMember>( *data_point_2 ) - get<indepMember>( *data_point_1 )) +
      0.5*(get<pdfMember>( *data_point_2 ) - get<pdfMember>( *data_point_1 ))*
      (get<indepMember>( *data_point_2) - get<indepMember>( *data_point_1 ));
    
    set<cdfMember>( *data_point_2, cdf_value );
    
    ++data_point_1;
    ++data_point_2;
  }

  typename TupleMemberTraits<Tuple,cdfMember>::tupleMemberType cdf_max =
    get<cdfMember>( data.back() );
    
  // Normalize the CDF and PDF
  if( normalize )
  {
    typename TupleMemberTraits<Tuple,cdfMember>::tupleMemberType
      cdf_norm_value;
    
    typename TupleMemberTraits<Tuple,pdfMember>::tupleMemberType 
      pdf_norm_value;
    
    data_point_1 = data.begin();
    
    while( data_point_1 != end )
    {
      cdf_norm_value = 
	get<cdfMember>( *data_point_1 )/getRawQuantity( cdf_max );
      set<cdfMember>( *data_point_1, cdf_norm_value );
      
      pdf_norm_value = 
	get<pdfMember>( *data_point_1 )/getRawQuantity( cdf_max );
      set<pdfMember>( *data_point_1, pdf_norm_value );
      
      ++data_point_1;
    }
  }
   
  // Return the normalization constant
  return 1.0/cdf_max;
}


// Create a pdf from an array of data using a first order approximation
template<TupleMember indepMember,
	   TupleMember pdfMember,
	   TupleMember cdfMember,
	   typename Array>
void DataProcessor::calculateContinuousPDF( Array &data )
{
  // Make sure that the array has more than one element
  testPrecondition( (data.size() > 1) );
  
  typedef typename ArrayTraits<Array>::value_type Tuple;
  
  typename Array::iterator data_point_1, data_point_2;
  typename Array::iterator end = data.end();

  data_point_1 = data.begin();
  data_point_2 = data.begin() + 1;

  // Calculate the slope of the cdf
  typename TupleMemberTraits<Tuple,pdfMember>::tupleMemberType 
    pdf_value = (get<cdfMember>( *data_point_2 ) - 
		 get<cdfMember>( *data_point_1 ) )/
    (get<indepMember>( *data_point_2 ) - get<indepMember>( *data_point_1 ) );

  // Initialize the PDF
  set<pdfMember>( *data_point_1, pdf_value );
  
  // Calculate the PDF (slope of cdf)
  while( data_point_2 != end )
  {
    pdf_value = (get<cdfMember>( *data_point_2 ) - 
		 get<cdfMember>( *data_point_1 ) )/
    (get<indepMember>( *data_point_2 ) - get<indepMember>( *data_point_1 ) );
    
    set<pdfMember>( *data_point_2, pdf_value );
    
    ++data_point_1;
    ++data_point_2;
  }
}

/*! \details This function calculates a discrete CDF from an array of discrete
 * data points. Since this data will be stored in a Teuchos::Array of Tuples
 * (either Utility::Pair, Utility::Trip or Utility::Quad) this function must know 
 * which member of the tuple stores the processed PDF value and which member of
 * the tuple will store the CDF value that is calculated. This function will 
 * only compile if the desired tuple members are actually available in Tuple.
 * \tparam pdfMember A member of the enumeration Utility::TupleMember, which is
 * used to refer to the member of Tuple that stores the PDF data values.
 * \tparam cdfMember A member of the enumeration Utility::TupleMember, which is
 * used to refer to the member of Tuple that will store the CDF data values.
 * \tparam Tuple A Utility tuple struct (either Utility::Pair, Utility::Trip or
 * Utility::Quad) that is used to store the processed data. 
 * \param[in,out] data The array of tuple structs which contain the table data.
 * \pre A valid array, which is any array of tuples with at least two elements,
 * must be given to this function. 
 * \note Developers: The Utility::TupleMemberTraits struct is critical 
 * to the generality of this function. Review this struct to better understand 
 * how this function operates. 
 */
template<TupleMember pdfMember,
	 TupleMember cdfMember,
	 typename Tuple>
void DataProcessor::calculateDiscreteCDF( Teuchos::Array<Tuple> &data )
{
  // Make sure that the array has at least one element
  testPrecondition( data.size() > 0 );
  
  typename Teuchos::Array<Tuple>::iterator data_point;
  typename Teuchos::Array<Tuple>::iterator end = data.end();

  data_point = data.begin();

  typename TupleMemberTraits<Tuple,cdfMember>::tupleMemberType 
    cdf_value = 0;

  // Create the discrete CDF
  while( data_point != end )
  {
    cdf_value += get<pdfMember>( *data_point );
    set<cdfMember>( *data_point, cdf_value );
    
    ++data_point;
  }

  // Normalize the CDF
  data_point = data.begin();
  while( data_point != end )
  {
    cdf_value = get<cdfMember>( *data_point )/get<cdfMember>( data.back() );
    set<cdfMember>( *data_point, cdf_value );
    
    ++data_point;
  }
}

/*! \details This function copies the data in the desired tuple member of one
 * tuple to the desired tuple member of another tuple. The two arrays of 
 * tuples must be distinct (no in place copying).
 * \tparam origMember A member of the enumeration Utility::TupleMember, which is
 * used to refer to the member of the tuple whose value will be copied.
 * \tparam copyMember A member of the enumeration Utility::TupleMember, which is
 * used to refer to the member of the new tuple that will store the copy.
 * \tparam OrigArray An array class of tuple structs (either Utility::Pair, 
 * Utility::Trip or Utility::Quad) that will have a member value copied.
 * \tparam CopyArray An array class of tuple structs (either Utility::Pair, 
 * Utility::Trip or Utility::Quad) that will store a copy in one of its members. 
 * The tuple type contained in the array does not need to be the same as the 
 * one contained in the <em> OrigArray </em>.
 * \param[in] orig_data The array of tuple structs which contain the data that
 * will be copied.
 * \param[in,out] copy_data The array of tuple structs that will store the
 * copied data.
 * \pre
 * <ul>
 *  <li> A valid array, which is any array of tuples with at least two 
 *       elements, must be given to this function.
 *  <li> The two arrays must have the same number of elements.
 *  <li> The two arrays must be distinct.
 * </ul>
 * \note Developers:
 * <ul>
 *  <li> The Utility::TupleMemberTraits struct is critical to the
 *       generality of this function. Review this struct to better understand 
 *       how this function operates. 
 * </ul>
 */
template<TupleMember origMember, 
	 TupleMember copyMember,
	 typename OrigArray,
	 typename CopyArray>
void DataProcessor::copyTupleMemberData( const OrigArray &orig_data,
					 CopyArray &copy_data )
{
  // Make sure that the arrays are valid
  testPrecondition( (orig_data.size() > 0) );
  testPrecondition( (orig_data.size() == copy_data.size()) );
  remember(typedef typename ArrayTraits<OrigArray>::pointer 
	   origTuplePointer);
  testPrecondition( &orig_data[0] != 
		    reinterpret_cast<origTuplePointer>( &copy_data[0] ) );

  typename OrigArray::const_iterator orig_data_point, end;
  typename CopyArray::iterator copy_data_point;
  orig_data_point = orig_data.begin();
  copy_data_point = copy_data.begin();
  end = orig_data.end();
  
  while( orig_data_point != end )
  {
    set<copyMember>( *copy_data_point, get<origMember>( *orig_data_point ) );
    
    ++orig_data_point;
    ++copy_data_point;
  }
}

/*! \details This function swaps the data in two tuple members. It can be used
 * to reorganize the processed data in a table.
 * \tparam member1 A member of the enumeration Utility::TupleMember, which is
 * used to refer to the member of the tuple whose value will be swapped.
 * \tparam member2 A member of the enumeration Utility::TupleMember, which is 
 * used to refer to the member of the tuple whose value will be swapped.
 * \tparam Array An array class of tuple structs (either Utility::Pair, 
 * Utility::Trip or Utility::Quad).
 * \param[in,out] data The array of tuple structs which contain the table data.
 * \pre 
 * <ul>
 *  <li> The tuple members that will swap data must be distinct.
 *  <li> A valid array, which is any array of tuples with at least one element,
 *       must be given to this function.
 * </ul>
 * \note Developers:
 * <ul>
 *  <li> The Utility::TupleMemberTraits struct is critical to the 
 *       generality of this function. Review this struct to better understand 
 *       how this function operates. 
 * </ul>
 */
template<TupleMember member1,
	 TupleMember member2,
	 typename Array>
void DataProcessor::swapTupleMemberData( Array &data )
{
  // Make sure that the members are valid
  testPrecondition( member1 != member2 );
  // Make sure that the arrays are valid
  testPrecondition( (data.size() > 0) );
  
  typedef typename ArrayTraits<Array>::value_type Tuple;
  
  typename Array::iterator data_point, end;
  data_point = data.begin();
  end = data.end();

  typename TupleMemberTraits<Tuple,member1>::tupleMemberType copy;

  while( data_point != end )
  {
    copy = get<member1>( *data_point );
    set<member1>( *data_point, get<member2>( *data_point ) );
    set<member2>( *data_point, copy );

    ++data_point;
  }
}

} // end Utility namespace

#endif // end UTILITY_DATA_PROCESSOR_DEF_HPP

//---------------------------------------------------------------------------//
// end Utility_DataProcessor_def.hpp
//---------------------------------------------------------------------------//

