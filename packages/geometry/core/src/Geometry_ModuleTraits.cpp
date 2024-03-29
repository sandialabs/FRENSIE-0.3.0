//---------------------------------------------------------------------------//
//!
//! \file   Geometry_ModuleTraits.cpp
//! \author Alex Robinson
//! \brief  Geometry module traits class definition
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <limits>

// FRENSIE Includes
#include "Geometry_ModuleTraits.hpp"

namespace Geometry{

// Initialize static member data
const ModuleTraits::InternalCellHandle 
ModuleTraits::invalid_internal_cell_handle = 0;

const ModuleTraits::InternalSurfaceHandle
ModuleTraits::invalid_internal_surface_handle = 0;

} // end Geometry namespace

//---------------------------------------------------------------------------//
// end Geometry_ModuleTraits.cpp
//---------------------------------------------------------------------------//
