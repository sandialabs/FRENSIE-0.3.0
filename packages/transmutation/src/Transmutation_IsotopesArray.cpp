//---------------------------------------------------------------------------//
//!
//! \file   Transmutation_IsotopesArray.cpp
//! \author Alex Bennett
//! \brief  Creates and read the Isotopes Array
//!
//---------------------------------------------------------------------------//

// Trilinos Includes
#include <Teuchos_Array.hpp>

// Boost Includes
#include <boost/unordered_map.hpp>

// FRENSIE Includes
#include "Transmutation_IsotopesArray.hpp"
#include "Transmutation_IsotopesForDepletion.hpp"
#include "Utility_ContractException.hpp"

namespace Transmutation {

void IsotopesArray::getOrderedIsotopesArray(const boost::unordered_map<int,double>& number_densities,
                                            Teuchos::Array<double>& number_densities_array) 
{
     // Set up the zaids array
     Teuchos::Array<int> zaids;
     IsotopesForDepletion::getIsotopes( zaids ); 

     // Set up the number densities array
     number_densities_array.resize( zaids.length() , 0.0 );

     // Iterator for the zaids array
     int j = 0;

     // Loop through given unordered map of zaids and number densities
     for(boost::unordered_map<int,double>::const_iterator i = number_densities.begin(); 
         i != number_densities.end(); 
         ++i)
     {
          // Loop through the zaids to find the location of the zaid
          for(j = 0; j != zaids.length(); j++)
          {
                if( i->first == zaids[j] )
                {
                       // Add the number to the number density array
                       number_densities_array[j] = i->second;

                       break;
                }
           }
           // Check if the isotope was found
           if( j == zaids.length() )
           {
                 std::cerr << std::endl;
                 std::cerr << "Warning: ZAID number " << i->first << 
                              " was not found in the list of isotopes for depletion." << std::endl;
           } 
     }
}

bool IsotopesArray::isIsotopeInList(int isotope)
{
     // Make sure isotope is a number greater then 0
     testPrecondition( isotope > 0 );

     // Set up the zaids array
     Teuchos::Array<int> zaids;
     IsotopesForDepletion::getIsotopes( zaids ); 

     // Loop through the list of zaids
     for(int i = 0; i != zaids.length(); i++)
     {
           // Check if element equals the desired element
           if( zaids[i] == isotope )
           {
                 // return true if element is found
                 return true;
           }
     }
     
     // return false if element is not found
     return false;
}

void IsotopesArray::getIsotopesNumberDensityMap(boost::unordered_map<int,double>& number_densities,
                                                const Teuchos::Array<double>& number_densities_array)
{
     // Clear the number densities map before filling it
     number_densities.clear();

     // Set up the zaids array
     Teuchos::Array<int> zaids;
     IsotopesForDepletion::getIsotopes( zaids ); 

     // Loop through the number densities array
     for(int i = 0; i != number_densities_array.length(); i++)
     {
           // Check if isotope is present
           if( number_densities_array[i] > 0.0 )
           {
                  // Add the number density of the element to the unordered map
                  number_densities[ zaids[i] ] = number_densities_array[i];
           }
     } 
}

void IsotopesArray::getNumberDensityMapFromUnorderedArray(boost::unordered_map<int,double>& number_densities,
                                                          const Teuchos::Array<std::pair<int,double> >& number_densities_array)
{
   // Clear the map
   number_densities.clear();

   // Loop through the array and fill the map
   for(Teuchos::Array<std::pair<int,double> >::const_iterator i = number_densities_array.begin(); 
       i != number_densities_array.end(); 
       ++i)
   {
      number_densities[ i->first ] = i->second;
   }
}

void IsotopesArray::getUnorderedArray(Teuchos::Array<std::pair<int,double> >& unordered_number_densities,
                                        const Teuchos::Array<double>& ordered_number_densities,
                                        const double lower_bound)
{
   // Clear the array
   unordered_number_densities.clear();

   // Set up zaids array
   Teuchos::Array<int> zaids;
   IsotopesForDepletion::getIsotopes( zaids );

   // Loop through the ordered array
   for(int i = 0; i != ordered_number_densities.length(); i++)
   {
      // Check if isotope is present
      if( ordered_number_densities[i] > lower_bound )
      {
         // Add the isotope to the unordered array
         unordered_number_densities.push_back( std::pair<int,double>(zaids[i],ordered_number_densities[i]) );
      }
   }
}

} // End namespace transmutation

//---------------------------------------------------------------------------//
// end Transmutation_IsotopesArray.cpp
//---------------------------------------------------------------------------//