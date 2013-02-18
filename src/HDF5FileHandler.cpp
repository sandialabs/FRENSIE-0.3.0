//---------------------------------------------------------------------------//
// \file HDF5FileHandler.cpp
// \author Alex Robinson
// \brief HDF5 file handler non-template member function definitions
//---------------------------------------------------------------------------//

// FACEMC Includes
#include "HDF5FileHandler.hpp"
#include "HDF5ExcpetionCatchMacro.hpp"

namespace FACEMC{

//! Default Constructor
HDF5FileHandler::HDF5FileHandler()
{
  // Turn off HDF5 error auto printing: All errors will be handled through
  // exceptions
  try
  {
    H5::Exception::dontPrint();
  }

  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

//! Open an HDF5 file and overwrite any existing data
hid_t HDF5FileHandler::openHDF5FileAndOverwrite( const std::string &file_name )
{
  // The H5File contructor can throw a H5::FileIException exception
  try
  {
    d_hdf5_file.reset( new H5File( file_name, HF5_ACC_TRUNC ) );
  }
  
  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

//! Open an HDF5 file and append to any existing data
hid_t HDF5FileHandler::openHDF5FileAndAppend( const std::string &file_name )
{
  // The H5File constructor can throw a H5::FileIException exception
  try
  {
    d_hdf5_file.reset( new H5File( file_name, HF5_ACC_RDWR ) );
  }
  
  HDF5_EXCEPTION_CATCH_AND_EXIT();
}

//! Close an HDF5 file
void HDF5FileHandler::closeHDF5File( hid_t file_id )
{
  d_hdf5_file_reset();
}

} // end FACEMC namespace

//---------------------------------------------------------------------------//
// end HDF5FileHandler.cpp
//---------------------------------------------------------------------------//
