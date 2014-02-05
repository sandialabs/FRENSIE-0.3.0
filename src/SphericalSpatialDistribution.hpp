//---------------------------------------------------------------------------//
//!
//! \file   SphericalSpatialDistribution.hpp
//! \author Alex Robinson
//! \brief  Spherical spatial distribution class declaration.
//!
//---------------------------------------------------------------------------//

#ifndef SPHERICAL_SPATIAL_DISTRIBUTION_HPP
#define SPHERICAL_SPATIAL_DISTRIBUTION_HPP

// Trilinos Includes
#include <Teuchos_RCP.hpp>
#include <Teuchos_ScalarTraits.hpp>

// FACEMC Includes
#include "OneDDistribution.hpp"
#include "SpatialDistribution.hpp"

namespace FACEMC{

/*! Spherical spatial distribution
 * \details the r, theta, and mu variables are assumed to be independent
 */
class SphericalSpatialDistribution : public SpatialDistribution
{

private:

  // Typedef for Teuchos::ScalarTraits
  typedef Teuchos::ScalarTraits<double> ST;

public:

  //! Constructor
  SphericalSpatialDistribution(
		      const Teuchos::RCP<OneDDistribution>& r_distribution,
		      const Teuchos::RCP<OneDDistribution>& theta_distribution,
		      const Teuchos::RCP<OneDDistribution>& mu_distribution,
		      const double center_x_position,
		      const double center_y_position,
		      const double center_z_position,
		      const Axis axis = Z_AXIS );

  //! Destructor
  ~SphericalSpatialDistribution()
  { /* ... */ }

  //! Return a random (cartesian) sample from the distribution (x, y, z)
  void sample( double sampled_point[3] );

private:

  // r dimension distribution
  Teuchos::RCP<OneDDistribution> d_r_distribution;

  // theta dimension distribution
  Teuchos::RCP<OneDDistribution> d_theta_distribution;

  // mu dimension distribution
  Teuchos::RCP<OneDDistribution> d_mu_distribution;

  // The center position of the sphere
  double d_center_x_position;
  double d_center_y_position;
  double d_center_z_position;

  // The spherical axis (direction of the mu distribution)
  Axis d_axis;
};

} // end FACEMC namespace

#endif // end SPHERICAL_SPATIAL_DISTRIBUTION_HPP

//---------------------------------------------------------------------------//
// end SphericalSpatialDistribution.cpp
//---------------------------------------------------------------------------//
