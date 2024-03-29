//---------------------------------------------------------------------------//
//!
//! \file   Utility_PrintableObject.hpp
//! \author Alex Robinson
//! \brief  Printable object base class declaration
//!
//---------------------------------------------------------------------------//

#ifndef UTILITY_PRINTABLE_OBJECT_HPP
#define UTILITY_PRINTABLE_OBJECT_HPP

// Std Lib Includes
#include <string>
#include <ostream>

namespace Utility{

//! The base class for printable objects
class PrintableObject
{

public:

  //! Constructor
  PrintableObject()
  { /* ... */ }

  //! Destructor
  virtual ~PrintableObject()
  { /* ... */ }

  //! Print method for placing the printable object in an output stream.
  virtual void print( std::ostream &os ) const = 0;

  //! Get the printable object label
  std::string getLabel() const;

  //! Check if the label will be printed
  bool isLabelPrinted() const;
};

//! Stream operator for printing all printable objects
inline std::ostream& operator<<( std::ostream &os, 
				 const Utility::PrintableObject &obj )
{ 
  obj.print( os );

  return os;
}

} // end UTILITY namespace

#endif // end UTILITY_PRINTABLE_OBJECT_HPP

//---------------------------------------------------------------------------//
// end Utility_PrintableObject.hpp
//---------------------------------------------------------------------------//
