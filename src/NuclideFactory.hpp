//---------------------------------------------------------------------------//
//!
//! \file   NuclideFactory.hpp
//! \author Alex Robinson
//! \brief  The nuclide factory class declaration
//!
//---------------------------------------------------------------------------//

#ifndef NUCLIDE_FACTORY_HPP
#define NUCLIDE_FACTORY_HPP

// Std Lib Includes
#include <string>

// Boost Includes
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

// Trilinos Includes
#include <Teuchos_ParameterList.hpp>
#include <Teuchos_ArrayRCP.hpp>
#include <Teuchos_RCP.hpp>

// FACEMC Includes
#include "Nuclide.hpp"

namespace FACEMC{

//! The nuclide factory class
class NuclideFactory
{
public:

  //! Constructor
  NuclideFactory( const std::string& cross_sections_xml_directory,
		  const Teuchos::ParameterList& cross_section_table_info,
		  const boost::unordered_set<std::string>& nuclide_aliases );

  //! Destructor
  ~NuclideFactory()
  { /* ... */ }

  //! Create the map of nuclides
  void createNuclideMap( 
    boost::unordered_map<unsigned,Teuchos::RCP<Nuclide> >& nuclide_map ) const;

private:

  // The nuclide id map
  boost::unordered_map<unsigned,Teuchos::RCP<Nuclide> > d_nuclide_id_map;
};

} // end FACEMC namespace

#endif // end NUCLIDE_FACTORY_HPP

//---------------------------------------------------------------------------//
// end NuclideFactory.hpp
//---------------------------------------------------------------------------//
