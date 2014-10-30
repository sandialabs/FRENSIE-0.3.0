//---------------------------------------------------------------------------//
//!
//! \file   Utility_TetrahedronHelpers.cpp
//! \author Alex Robinson, Eli Moll
//! \brief  Tetrahedron helper functions
//! 
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <math.h>

// Trilinos Includes
#include <Teuchos_ScalarTraits.hpp>

// FRENSIE Includes
#include "Utility_TetrahedronHelpers.hpp"
#include "Utility_ContractException.hpp"
#include <moab/Matrix3.hpp>

namespace Utility{

// Calculate the volume of a tetrahedron
double calculateTetrahedronVolume( const double vertex_a[3],
				   const double vertex_b[3],
				   const double vertex_c[3],
				   const double vertex_d[3] )
{
  double a1 = vertex_a[0] - vertex_d[0];
  double a2 = vertex_a[1] - vertex_d[1];
  double a3 = vertex_a[2] - vertex_d[2];
  double b1 = vertex_b[0] - vertex_d[0];
  double b2 = vertex_b[1] - vertex_d[1];
  double b3 = vertex_b[2] - vertex_d[2];
  double c1 = vertex_c[0] - vertex_d[0];
  double c2 = vertex_c[1] - vertex_d[1];
  double c3 = vertex_c[2] - vertex_d[2];
  
  double volume =
    fabs( a1*(b2*c3-b3*c2) + a2*(b3*c1-b1*c3) + a3*(b1*c2-b2*c1) )/6.0;

  testPostcondition( !Teuchos::ScalarTraits<double>::isnaninf( volume ) );
  testPostcondition( volume > 0.0 );

  return volume;
}

// Calculate the Barycentric Transform Matrix
double* calculateBarycentricTransformMatrix( const double vertex_a[3],
				             const double vertex_b[3],
				             const double vertex_c[3],
				             const double vertex_d[3] )
{				            
  double t1 = vertex_a[0] - vertex_d[0]; 
  double t2 = vertex_b[0] - vertex_d[0]; 
  double t3 = vertex_c[0] - vertex_d[0];
  double t4 = vertex_a[1] - vertex_d[1];
  double t5 = vertex_b[1] - vertex_d[1];
  double t6 = vertex_c[1] - vertex_d[1];
  double t7 = vertex_a[2] - vertex_d[2];
  double t8 = vertex_b[2] - vertex_d[2];
  double t9 = vertex_c[2] - vertex_d[2];
  
  moab::Matrix3 T( t1, t2, t3, t4, t5, t6, t7, t8, t9 );
  T = T.inverse();
  
  return T.array();
}

} // end Utility namespace

//---------------------------------------------------------------------------//
// end Utility_TetrahedronHelpers.cpp
//---------------------------------------------------------------------------//
