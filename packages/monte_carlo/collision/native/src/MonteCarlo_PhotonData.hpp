//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_PhotonData.hpp
//! \author Alex Robinson
//! \brief  PhotonData class declaration
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_PHOTON_DATA_HPP
#define MONTE_CARLO_PHOTON_DATA_HPP

// FRENSIE Includes
#include "MonteCarlo_PhotonDataBasic.hpp"
#include "MonteCarlo_PhotonDataFluorescence.hpp"
#include "MonteCarlo_PhotonDataDopplerBroadening.hpp"

namespace MonteCarlo{

/*!
 * \brief This is the default PhotonData class. All photon physics is
 * available.
 */
template<bool fluorescence_physics = true,
	 bool doppler_physics = true>
class PhotonData : public PhotonDataBasic, 
		   public PhotonDataFluorescence,
		   public PhotonDataDopplerBroadening
{

public:
  
  //! Constructor
  PhotonData( unsigned int atomic_number,
	      double energy_min,
	      double energy_max )
    : PhotonDataBasic( atomic_number, energy_min, energy_max ),
      PhotonDataFluorescence( atomic_number, energy_min, energy_max ),
      PhotonDataDopplerBroadening( atomic_number, energy_min, energy_max )
  { /* ... */ }

  //! Destructor
  virtual ~PhotonData()
  { /* ... */ }

  using PhotonDataBasic::getAtomicNumber;
  using PhotonDataBasic::getAtomicWeight;
  using PhotonDataBasic::getTotalCrossSection;
  using PhotonDataBasic::getNonabsorptionProbability;
};

/*!
 * \brief No doppler broadening physics available.
 */
template<>
class PhotonData<true,false> : public PhotonDataBasic, 
			       public PhotonDataFluorescence
{

public:

  //! Constructor
  PhotonData( unsigned int atomic_number,
	      double energy_min,
	      double energy_max )
    : PhotonDataBasic( atomic_number, energy_min, energy_max ),
      PhotonDataFluorescence( atomic_number, energy_min, energy_max )
  { /* ... */ }

  //! Destructor
  virtual ~PhotonData()
  { /* ... */ }

  using PhotonDataBasic::getAtomicNumber;
  using PhotonDataBasic::getAtomicWeight;
  using PhotonDataBasic::getTotalCrossSection;
  using PhotonDataBasic::getNonabsorptionProbability;
};

/*!
 * \brief No fluorescence physics available.
 */
template<>
class PhotonData<false,true> : public PhotonDataBasic, 
			       public PhotonDataDopplerBroadening
{

public:

  //! Constructor
  PhotonData( unsigned int atomic_number,
	      double energy_min,
	      double energy_max )
    : PhotonDataBasic( atomic_number, energy_min, energy_max ),
      PhotonDataDopplerBroadening( atomic_number, energy_min, energy_max )
  { /* ... */ }

  //! Destructor
  virtual ~PhotonData()
  { /* ... */ }

  using PhotonDataBasic::getAtomicNumber;
  using PhotonDataBasic::getAtomicWeight;
  using PhotonDataBasic::getTotalCrossSection;
  using PhotonDataBasic::getNonabsorptionProbability;
};

/*!
 * \brief Only basic physics available.
 */
template<>
class PhotonData<false,false> : public PhotonDataBasic
{

public:

  //! Constructor
  PhotonData( unsigned int atomic_number,
	      double energy_min,
	      double energy_max )
    : PhotonDataBasic( atomic_number, energy_min, energy_max )
  { /* ... */ }

  //! Destructor
  virtual ~PhotonData()
  { /* ... */ }

  using PhotonDataBasic::getAtomicNumber;
  using PhotonDataBasic::getAtomicWeight;
  using PhotonDataBasic::getTotalCrossSection;
  using PhotonDataBasic::getNonabsorptionProbability;
};

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_PHOTON_DATA_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_PhotonData.hpp
//---------------------------------------------------------------------------//

