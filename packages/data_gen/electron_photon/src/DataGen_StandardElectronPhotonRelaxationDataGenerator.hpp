//---------------------------------------------------------------------------//
//!
//! \file   DataGen_StandardElectronPhotonRelaxationDataGenerator.hpp
//! \author Alex Robinson
//! \brief  The standard electron-photon-relaxation data generator class decl.
//!
//---------------------------------------------------------------------------//

#ifndef DATA_GEN_STANDARD_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_HPP
#define DATA_GEN_STANDARD_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_HPP

// Std Lib Includes
#include <utility>

// Trilinos Includes
#include <Teuchos_RCP.hpp>
#include <Teuchos_Array.hpp>

// FRENSIE Includes
#include "DataGen_ElectronPhotonRelaxationDataGenerator.hpp"
#include "MonteCarlo_SubshellIncoherentPhotonScatteringDistribution.hpp"
#include "Data_ENDLFileHandler.hpp"
#include "Data_XSSEPRDataExtractor.hpp"
#include "Utility_OneDDistribution.hpp"

namespace DataGen{

//! The standard electron-photon-relaxation data generator class
class StandardElectronPhotonRelaxationDataGenerator : public ElectronPhotonRelaxationDataGenerator
{

public:

  //! Constructor
  StandardElectronPhotonRelaxationDataGenerator( 
	   const unsigned atomic_number,
	   const Teuchos::RCP<const Data::XSSEPRDataExtractor>& ace_epr_data,
       const Teuchos::RCP<Data::ENDLFileHandler>& eedl_file_handler,
	   const double min_photon_energy,
	   const double max_photon_energy,
       const double min_electron_energy,
       const double max_electron_energy,
       const double cutoff_angle,
	   const double occupation_number_evaluation_tolerance,
	   const double subshell_incoherent_evaluation_tolerance,
	   const double grid_convergence_tol = 0.001,
	   const double grid_absolute_diff_tol = 1e-13,
	   const double grid_distance_tol = 1e-13 );

  //! Destructor
  ~StandardElectronPhotonRelaxationDataGenerator()
  { /* ... */ }

  //! Populate the electron-photon-relaxation data container
  void populateEPRDataContainer(
			   Data::ElectronPhotonRelaxationVolatileDataContainer&
			   data_container ) const;

protected:

  // Set the atomic data
  void setRelaxationData( Data::ElectronPhotonRelaxationVolatileDataContainer&
			  data_container ) const;

  // Set the Compton profile data
  void setComptonProfileData( 
			   Data::ElectronPhotonRelaxationVolatileDataContainer&
			   data_container ) const;

  // Set the occupation number data
  void setOccupationNumberData(
			   Data::ElectronPhotonRelaxationVolatileDataContainer&
			   data_container ) const;

  // Set the Waller-Hartree scattering function data
  void setWallerHartreeScatteringFunctionData(
			   Data::ElectronPhotonRelaxationVolatileDataContainer&
			   data_container ) const;

  // Set the Waller-Hartree atomic form factor data
  void setWallerHartreeAtomicFormFactorData(
			   Data::ElectronPhotonRelaxationVolatileDataContainer&
			   data_container ) const;

  // Set the photon data
  void setPhotonData( Data::ElectronPhotonRelaxationVolatileDataContainer&
		      data_container ) const;

  // Set the electron data
  void setElectronData( Data::ElectronPhotonRelaxationVolatileDataContainer&
			data_container ) const;

private:

  // Test if a value is greater than or equal to one
  static bool greaterThanOrEqualToOne( const double value );

  // Test if a value is greater than one
  static bool greaterThanOne( const double value );

  // The if a value is not equal to zero
  static bool notEqualZero( const double value );

  // Set the transition data
  void setTransitionData( const unsigned subshell,
			  const unsigned transitions,
			  const unsigned subshell_data_start_index,
			  Data::ElectronPhotonRelaxationVolatileDataContainer&
			  data_container ) const;

  // Extract the half Compton profile from the ACE table
  void extractHalfComptonProfile( 
			   const unsigned subshell,
			   std::vector<double>& half_momentum_grid,
			   std::vector<double>& half_profile ) const;

  // Set the screened rutherford data
  void setScreenedRutherfordData( 
    const Teuchos::RCP<const Utility::OneDDistribution>& 
        cutoff_elastic_cross_section, 
    const Teuchos::RCP<const Utility::OneDDistribution>& 
        total_elastic_cross_section,
    const std::vector<double>& elastic_energy_grid,
    const std::map<double,std::vector<double> >& elastic_pdf,
    Data::ElectronPhotonRelaxationVolatileDataContainer& data_container ) const;

  // Extract the average photon heating numbers
  template<typename InterpPolicy>
  void extractCrossSection(
	  Teuchos::ArrayView<const double> raw_energy_grid,
	  Teuchos::ArrayView<const double> raw_cross_section,
	  Teuchos::RCP<const Utility::OneDDistribution>& cross_section ) const;

  // Extract electron cross sections
  template<typename InterpPolicy>
  void extractElectronCrossSection(
        std::vector<double>& raw_energy_grid,
        std::vector<double>& raw_cross_section,
        Teuchos::RCP<const Utility::OneDDistribution>& cross_section ) const;

  // Extract the subshell photoelectric cross sections
  void extractSubshellPhotoelectricCrossSections( Teuchos::Array<std::pair<unsigned,Teuchos::RCP<const Utility::OneDDistribution> > >& cross_sections ) const;

  // Create the subshell impulse approx incoherent cross section evaluators
  void createSubshellImpulseApproxIncoherentCrossSectionEvaluators(
     const Data::ElectronPhotonRelaxationVolatileDataContainer& data_container,
     Teuchos::Array<std::pair<unsigned,Teuchos::RCP<const MonteCarlo::SubshellIncoherentPhotonScatteringDistribution> > >& evaluators ) const;

  // Initialize the photon union energy grid
  void initializePhotonUnionEnergyGrid( 
     const Data::ElectronPhotonRelaxationVolatileDataContainer& data_container,
     std::list<double>& union_energy_grid ) const;

  // Create the cross section on the union energy grid
  void createCrossSectionOnUnionEnergyGrid(
   const std::list<double>& union_energy_grid,
   const Teuchos::RCP<const Utility::OneDDistribution>& original_cross_section,
   std::vector<double>& cross_section,
   unsigned& threshold_index ) const;

  // Create the cross section on the union energy grid
  void createCrossSectionOnUnionEnergyGrid(
	     const std::list<double>& union_energy_grid,
	     const Teuchos::RCP<const MonteCarlo::SubshellIncoherentPhotonScatteringDistribution>&
	     original_cross_section,
	     std::vector<double>& cross_section,
	     unsigned& threshold_index ) const;

  // Merge the electron union energy grid
  void mergeElectronUnionEnergyGrid( 
    const std::vector<double>& energy_grid,
    std::list<double>& union_energy_grid ) const;

  // Calculate the total photoelectric cross section
  void calculateTotalPhotoelectricCrossSection( 
	                   Data::ElectronPhotonRelaxationVolatileDataContainer&
			   data_container ) const;

  // Calculate the total impulse approx. incoherent cross section
  void calculateImpulseApproxTotalIncoherentCrossSection(
		           Data::ElectronPhotonRelaxationVolatileDataContainer&
			   data_container ) const;

  // Calculate the Waller-Hartree total cross section
  void calculateWallerHartreeTotalCrossSection(
		          Data::ElectronPhotonRelaxationVolatileDataContainer&
			  data_container ) const;

  // Calculate the impulse approx total cross section
  void calculateImpulseApproxTotalCrossSection(
		          Data::ElectronPhotonRelaxationVolatileDataContainer&
			  data_container ) const;

  // The threshold energy nudge factor
  static const double s_threshold_energy_nudge_factor;

  // The ACE data
  Teuchos::RCP<const Data::XSSEPRDataExtractor> d_ace_epr_data;

  // The EEDL data
  Teuchos::RCP<Data::ENDLFileHandler> d_eedl_file_handler;

  // The min photon energy
  double d_min_photon_energy;

  // The max photon energy
  double d_max_photon_energy;

  // The min electron energy
  double d_min_electron_energy;

  // The max electron energy
  double d_max_electron_energy;

  // The cutoff angle below which screened rutherford is used
  double d_cutoff_angle;

  // The occupation number evaluation tolerance
  double d_occupation_number_evaluation_tolerance;

  // The subshell incoherent evaluation tolerance
  double d_subshell_incoherent_evaluation_tolerance;
  
  // The grid convergence tolerance
  double d_grid_convergence_tol;

  // The grid absolute difference tolerance
  double d_grid_absolute_diff_tol;

  // The grid distance tolerance
  double d_grid_distance_tol;
};

// Test if a value is greater than or equal to one
inline bool 
StandardElectronPhotonRelaxationDataGenerator::greaterThanOrEqualToOne( 
							   const double value )
{
  return value >= 1.0;
}

// Test if a value is greater than one
inline bool 
StandardElectronPhotonRelaxationDataGenerator::greaterThanOne( 
							   const double value )
{
  return value > 1.0;
}

// The if a value is not equal to zero
inline bool StandardElectronPhotonRelaxationDataGenerator::notEqualZero( 
							   const double value )
{
  return value != 0.0;
}

} // end DataGen namespace

//---------------------------------------------------------------------------//
// Template Includes
//---------------------------------------------------------------------------//

#include "DataGen_StandardElectronPhotonRelaxationDataGenerator_def.hpp"

//---------------------------------------------------------------------------//

#endif // end DATA_GEN_STANDARD_ELECTRON_PHOTON_RELAXATION_DATA_GENERATOR_HPP

//---------------------------------------------------------------------------//
// end DataGen_StandardElectronPhotonRelaxationDataGenerator.hpp
//---------------------------------------------------------------------------//
