//---------------------------------------------------------------------------//
//!
//! \file   tstFortranStringHelpers.cpp
//! \author Alex Robinson, Alex Bennett
//! \brief  String Conversion unit test
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <string>

// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>

// Fortran String Conversion Helpers
extern "C" {
  void testStringConversion( const char* string_in, const int string_size, char* string_out);
} 

//---------------------------------------------------------------------------//
// Tests 
//---------------------------------------------------------------------------//
//Check that C Strings can be converted to Fortran.
TEUCHOS_UNIT_TEST( FortranStringHelpers, testStringConversion )
{
  std::string input_string( "test" ), output_string( "alex" );
  
  testStringConversion( input_string.c_str(), input_string.size(), &output_string[0] );

  TEST_ASSERT( input_string.compare( output_string ) == 0 );

  std::cout << input_string << " " << output_string << std::endl;

}




//---------------------------------------------------------------------------//
// end tstFortranStringHelpers.cpp
//---------------------------------------------------------------------------//
 
