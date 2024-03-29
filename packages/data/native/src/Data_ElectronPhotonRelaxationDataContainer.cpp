//---------------------------------------------------------------------------//
//!
//! \file   Data_ElectronPhotonRelaxationDataContainer.cpp
//! \author Alex Robinson, Luke Kersting
//! \brief  The native electron-photon-relaxation data container class decl.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <algorithm>
#include <fstream>
#include <sstream>
#include <typeinfo>

// Boost Includes
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

// FRENSIE Includes
#include "Data_ElectronPhotonRelaxationDataContainer.hpp"
#include "Utility_SortAlgorithms.hpp"
#include "Utility_ContractException.hpp"

namespace Data{

// Constructor (from saved archive)
ElectronPhotonRelaxationDataContainer::ElectronPhotonRelaxationDataContainer( 
		    const std::string& archive_name,
		    const Utility::ArchivableObject::ArchiveType archive_type )
{
  // Import the data in the archive - no way to use initializer list :(
  this->importData( archive_name, archive_type );
}

//---------------------------------------------------------------------------//
// GET RELAXATION DATA
//---------------------------------------------------------------------------//

// Return the atomic number
unsigned ElectronPhotonRelaxationDataContainer::getAtomicNumber() const
{
  return d_atomic_number;
}

// Return the atomic subshells 
const std::set<unsigned>& 
ElectronPhotonRelaxationDataContainer::getSubshells() const
{
  return d_subshells;
}

// Return the subshell occupancies
double ElectronPhotonRelaxationDataContainer::getSubshellOccupancy(
					        const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != 
		    d_subshells.end() );
  
  return d_subshell_occupancies.find( subshell )->second;
}

// Return the subshell binding energies
double ElectronPhotonRelaxationDataContainer::getSubshellBindingEnergy(
						const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != 
		    d_subshells.end() );
  
  return d_subshell_binding_energies.find( subshell )->second;
}

// Return if there is relaxation data
bool ElectronPhotonRelaxationDataContainer::hasRelaxationData() const
{
  return d_relaxation_transitions.size() > 0;
}

// Return if the subshell has relaxation data
bool ElectronPhotonRelaxationDataContainer::hasSubshellRelaxationData( 
						const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );
  
  return d_relaxation_transitions.find( subshell ) != 
    d_relaxation_transitions.end();
}

// Return the number of transitions that can fill a subshell vacancy
unsigned ElectronPhotonRelaxationDataContainer::getSubshellRelaxationTransitions( 
						const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );

  return d_relaxation_transitions.find( subshell )->second;
}

// Return the relaxation vacancies for a subshell
const std::vector<std::pair<unsigned,unsigned> >&
ElectronPhotonRelaxationDataContainer::getSubshellRelaxationVacancies( 
						const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );

  return d_relaxation_vacancies.find( subshell )->second;
}

// Return the relaxation particle energies for a subshell
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getSubshellRelaxationParticleEnergies(
					        const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );

  return d_relaxation_particle_energies.find( subshell )->second;
}

// Return the relaxation probabilities for a subshell
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getSubshellRelaxationProbabilities(
					        const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );

  return d_relaxation_probabilities.find( subshell )->second;
}

//---------------------------------------------------------------------------//
// GET PHOTON DATA
//---------------------------------------------------------------------------//

// Return the Compton profile momentum grid for a subshell
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getComptonProfileMomentumGrid(
					        const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );
  
  return d_compton_profile_momentum_grids.find( subshell )->second;
}

// Return the Compton profile for a subshell
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getComptonProfile(
						const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != 
		    d_subshells.end() );

  return d_compton_profiles.find( subshell )->second;
}

// Return the occupation number momentum grid for a subshell
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getOccupationNumberMomentumGrid(
						const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != 
		    d_subshells.end() );
  
  return d_occupation_number_momentum_grids.find( subshell )->second;
}
  
// Return the occupation number for a subshell
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getOccupationNumber(
					        const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );
  
  return d_occupation_numbers.find( subshell )->second;
}

// Return the Waller-Hartree scattering function momentum grid
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getWallerHartreeScatteringFunctionMomentumGrid() const
{
  return d_waller_hartree_scattering_function_momentum_grid;
}

// Return the Waller-Hartree scattering function
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getWallerHartreeScatteringFunction() const
{
  return d_waller_hartree_scattering_function;
}
  
// Return the Waller-Hartree atomic form factor momentum grid
const std::vector<double>&
ElectronPhotonRelaxationDataContainer::getWallerHartreeAtomicFormFactorMomentumGrid() const
{
  return d_waller_hartree_atomic_form_factor_momentum_grid;
}

// Return the Waller-Hartree atomic form factor
const std::vector<double>& ElectronPhotonRelaxationDataContainer::getWallerHartreeAtomicFormFactor() const
{
  return d_waller_hartree_atomic_form_factor;
}

// Return the photon energy grid
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getPhotonEnergyGrid() const
{
  return d_photon_energy_grid;
}

// Return the average heating numbers
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getAveragePhotonHeatingNumbers() const
{
  return d_average_photon_heating_numbers;
}

// Return the Waller-Hartree (WH) incoherent photon cross section 
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getWallerHartreeIncoherentCrossSection() const
{
  return d_waller_hartree_incoherent_cross_section;
}

// Return the WH incoherent photon cross section threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getWallerHartreeIncoherentCrossSectionThresholdEnergyIndex() const
{
  return d_waller_hartree_incoherent_cross_section_threshold_index;
}

// Return the impluse approx. (IA) incoherent photon cross section
const std::vector<double>&
ElectronPhotonRelaxationDataContainer::getImpulseApproxIncoherentCrossSection() const
{
  return d_impulse_approx_incoherent_cross_section;
}

// Return the IA incoherent photon cross section threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getImpulseApproxIncoherentCrossSectionThresholdEnergyIndex() const
{
  return d_impulse_approx_incoherent_cross_section_threshold_index;
}

// Return the subshell Impulse approx. incoherent photon cross section
const std::vector<double>&
ElectronPhotonRelaxationDataContainer::getImpulseApproxSubshellIncoherentCrossSection( 
					        const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );

  return d_impulse_approx_subshell_incoherent_cross_sections.find( subshell )->second;
}

// Return the subshell IA incoherent photon cs threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getImpulseApproxSubshellIncoherentCrossSectionThresholdEnergyIndex( 
					        const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );

  return d_impulse_approx_subshell_incoherent_cross_section_theshold_indices.find( subshell )->second;
}

// Return the Waller-Hartree coherent cross section
const std::vector<double>&
ElectronPhotonRelaxationDataContainer::getWallerHartreeCoherentCrossSection() const
{
  return d_waller_hartree_coherent_cross_section;
}

// Return the Waller-Hartree coherent cs threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getWallerHartreeCoherentCrossSectionThresholdEnergyIndex() const
{
  return d_waller_hartree_coherent_cross_section_threshold_index;
}

// Return the pair production cross section
const std::vector<double>&
ElectronPhotonRelaxationDataContainer::getPairProductionCrossSection() const
{
  return d_pair_production_cross_section;
}

// Return the pair production cross section threshold energy bin index
unsigned ElectronPhotonRelaxationDataContainer::getPairProductionCrossSectionThresholdEnergyIndex() const
{
  return d_pair_production_cross_section_threshold_index;
}

// Return the Photoelectric effect cross section
const std::vector<double>& ElectronPhotonRelaxationDataContainer::getPhotoelectricCrossSection() const
{
  return d_photoelectric_cross_section;
}

// Return the Photoelectric effect cross section theshold energy bin index
unsigned ElectronPhotonRelaxationDataContainer::getPhotoelectricCrossSectionThresholdEnergyIndex() const
{
  return d_photoelectric_cross_section_threshold_index;
}

// Return the Photoelectric effect cross section for a subshell
const std::vector<double>&
ElectronPhotonRelaxationDataContainer::getSubshellPhotoelectricCrossSection( 
						const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );

  return d_subshell_photoelectric_cross_sections.find( subshell )->second;
}

// Return the subshell Photoelectric effect cross section threshold index
unsigned
ElectronPhotonRelaxationDataContainer::getSubshellPhotoelectricCrossSectionThresholdEnergyIndex( 
					        const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) !=
		    d_subshells.end() );

  return d_subshell_photoelectric_cross_section_threshold_indices.find( subshell )->second;
}

// Return the Waller-Hartree total cross section
const std::vector<double>& ElectronPhotonRelaxationDataContainer::getWallerHartreeTotalCrossSection() const
{
  return d_waller_hartree_total_cross_section;
}

// Return the impulse approx. total cross section
const std::vector<double>& ElectronPhotonRelaxationDataContainer::getImpulseApproxTotalCrossSection() const
{
  return d_impulse_approx_total_cross_section;
}


//---------------------------------------------------------------------------//
// GET ELECTRON DATA 
//---------------------------------------------------------------------------//

// Return the elastic cutoff angle
double ElectronPhotonRelaxationDataContainer::getCutoffAngle() const
{
  return d_cutoff_angle;
}

// Return the elastic angular energy grid
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getElasticAngularEnergyGrid() const
{
  return d_angular_energy_grid;
}

// Return the elastic angles for an incoming energy
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getAnalogElasticAngles(
					        const double incoming_energy ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy >= d_angular_energy_grid.front() );
  testPrecondition( incoming_energy <= d_angular_energy_grid.back() );

  return d_analog_elastic_angles.find( incoming_energy )->second;
}

// Return the elastic pdf for an incoming energy
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getAnalogElasticPDF(
					        const double incoming_energy ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy >= d_angular_energy_grid.front() );
  testPrecondition( incoming_energy <= d_angular_energy_grid.back() );

  return d_analog_elastic_pdf.find( incoming_energy )->second;
}

// Return the screened Rutherford elastic normalization constant 
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getScreenedRutherfordNormalizationConstant() const
{
  return d_screened_rutherford_normalization_constant;
}

// Return Moliere's screening constant 
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getMoliereScreeningConstant() const
{
  return d_moliere_screening_constant;
}

// Return the moment preserving elastic discrete angles for an incoming energy
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getMomentPreservingElasticDiscreteAngles(
					        const double incoming_energy ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy >= d_angular_energy_grid.front() );
  testPrecondition( incoming_energy <= d_angular_energy_grid.back() );

  return d_moment_preserving_elastic_discrete_angles.find( incoming_energy )->second;
}

// Return the moment preserving elastic weights for an incoming energy
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getMomentPreservingElasticWeights(
					        const double incoming_energy ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy >= d_angular_energy_grid.front() );
  testPrecondition( incoming_energy <= d_angular_energy_grid.back() );

  return d_moment_preserving_elastic_weights.find( incoming_energy )->second;
}

// Return the electroionization energy grid for a subshell
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getElectroionizationEnergyGrid( 
                            const unsigned subshell ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );

  return d_electroionization_energy_grid.find( subshell )->second;
}

// Return the electroionization recoil energy for a subshell and energy bin
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getElectroionizationRecoilEnergy( 
                           const unsigned subshell,
					       const double incoming_energy ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the incoming energy is valid
  testPrecondition( 
            incoming_energy >= 
            d_electroionization_energy_grid.find( subshell )->second.front() );
  testPrecondition( 
            incoming_energy <= 
            d_electroionization_energy_grid.find( subshell )->second.back() );

  return d_electroionization_recoil_energy.find( subshell )->second.find( incoming_energy )->second;
}

// Return the electroionization recoil energy pdf for a subshell and energy bin
const std::vector<double>&  
ElectronPhotonRelaxationDataContainer::getElectroionizationRecoilPDF( 
                           const unsigned subshell,
					       const double incoming_energy ) const
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the incoming energy is valid
  testPrecondition( 
            incoming_energy >= 
            d_electroionization_energy_grid.find( subshell )->second.front() );
  testPrecondition( 
            incoming_energy <= 
            d_electroionization_energy_grid.find( subshell )->second.back() );

  return d_electroionization_recoil_pdf.find( subshell )->second.find( incoming_energy )->second;
}

// Return the bremsstrahlung energy grid
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getBremsstrahlungEnergyGrid() const
{
  return d_bremsstrahlung_energy_grid;
}

// Return the bremsstrahlung for an incoming energy
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getBremsstrahlungPhotonEnergy(
					        const double incoming_energy ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy >= d_bremsstrahlung_energy_grid.front() );
  testPrecondition( incoming_energy <= d_bremsstrahlung_energy_grid.back() );

  return d_bremsstrahlung_photon_energy.find( incoming_energy )->second;
}

// Return the bremsstrahlung photon pdf for an incoming energy
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getBremsstrahlungPhotonPDF(
					        const double incoming_energy ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy >= d_bremsstrahlung_energy_grid.front() );
  testPrecondition( incoming_energy <= d_bremsstrahlung_energy_grid.back() );

  return d_bremsstrahlung_photon_pdf.find( incoming_energy )->second;
}

// Return the atomic excitation energy grid
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getAtomicExcitationEnergyGrid() const
{
  return d_atomic_excitation_energy_grid;
}

// Return the atomic excitation energy loss
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getAtomicExcitationEnergyLoss() const
{
  return d_atomic_excitation_energy_loss;
}

// Return the electron energy grid
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getElectronEnergyGrid() const
{
  return d_electron_energy_grid;
}
// Return the cutoff elastic electron cross section
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getCutoffElasticCrossSection() const
{
  return d_cutoff_elastic_cross_section;
}

// Return the cutoff elastic cross section threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getCutoffElasticCrossSectionThresholdEnergyIndex() const
{
  return d_cutoff_elastic_cross_section_threshold_index;
}
// Return the screened Rutherford elastic electron cross section
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getScreenedRutherfordElasticCrossSection() const
{
  return d_screened_rutherford_elastic_cross_section;
}

// Return the screened Rutherford elastic cross section threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getScreenedRutherfordElasticCrossSectionThresholdEnergyIndex() const
{
  return d_screened_rutherford_elastic_cross_section_threshold_index;
}
// Return the total elastic electron cross section
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getTotalElasticCrossSection() const
{
  return d_total_elastic_cross_section;
}

// Return the total elastic cross section threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getTotalElasticCrossSectionThresholdEnergyIndex() const
{
  return d_total_elastic_cross_section_threshold_index;
}

// Return the Moment Preserving (MP) elastic electron cross section
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getMomentPreservingCrossSection() const
{
  return d_moment_preserving_elastic_cross_section;
}

// Return the MP elastic cross section threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getMomentPreservingCrossSectionThresholdEnergyIndex() const
{
  return d_moment_preserving_elastic_cross_section_threshold_index;
}

// Return the electroionization electron cross section for a subshell
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getElectroionizationCrossSection( 
    const unsigned subshell ) const
{
  return d_electroionization_subshell_cross_section.find( subshell )->second;
}

// Return the electroionization cross section threshold energy bin index for a subshell
unsigned
ElectronPhotonRelaxationDataContainer::getElectroionizationCrossSectionThresholdEnergyIndex( 
    const unsigned subshell ) const
{
  return d_electroionization_subshell_cross_section_threshold_index.find( subshell )->second;
}

// Return the bremsstrahlung electron cross section
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getBremsstrahlungCrossSection() const
{
  return d_bremsstrahlung_cross_section;
}

// Return the bremsstrahlung cross section threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getBremsstrahlungCrossSectionThresholdEnergyIndex() const
{
  return d_bremsstrahlung_cross_section_threshold_index;
}

// Return the atomic excitation electron cross section
const std::vector<double>& 
ElectronPhotonRelaxationDataContainer::getAtomicExcitationCrossSection() const
{
  return d_atomic_excitation_cross_section;
}

// Return the atomic excitation cross section threshold energy bin index
unsigned
ElectronPhotonRelaxationDataContainer::getAtomicExcitationCrossSectionThresholdEnergyIndex() const
{
  return d_atomic_excitation_cross_section_threshold_index;
}

//---------------------------------------------------------------------------//
// SET RELAXATION DATA
//---------------------------------------------------------------------------//

// Set the atomic number
void ElectronPhotonRelaxationDataContainer::setAtomicNumber( 
						 const unsigned atomic_number )
{
  // Make sure the atomic number is valid
  testPrecondition( atomic_number > 0 );
  testPrecondition( atomic_number <= 100 );

  d_atomic_number = atomic_number;
}
  
// Set the atomic subshells
void ElectronPhotonRelaxationDataContainer::setSubshells( 
				       const std::set<unsigned>& subshells )
{
  // Make sure the subshells are valid
  testPrecondition( subshells.size() > 0 );
  testPrecondition( std::find_if( subshells.begin(),
				  subshells.end(),
				  isValueLessThanOrEqualToZero ) ==
		    subshells.end() );

  d_subshells = subshells;
}

// Set the subshell occupancy
void ElectronPhotonRelaxationDataContainer::setSubshellOccupancy( 
						       const unsigned subshell,
						       const double occupancy )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the subshell occupancy is valid
  testPrecondition( occupancy > 0.0 );

  d_subshell_occupancies[subshell] = occupancy;
}

// Set the subshell binding energy
void ElectronPhotonRelaxationDataContainer::setSubshellBindingEnergy(
						  const unsigned subshell,
						  const double binding_energy )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the subshell binding energy is valid
  testPrecondition( binding_energy > 0.0 );

  d_subshell_binding_energies[subshell] = binding_energy;
}

// Set the number of transitions that can fill a subshell vacancy
void ElectronPhotonRelaxationDataContainer::setSubshellRelaxationTransitions( 
						   const unsigned subshell,
						   const unsigned transitions )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the number of transitions is valid
  testPrecondition( transitions > 0 );
  
  d_relaxation_transitions[subshell] = transitions;
}

// Set the relaxation vacancies for a subshell
void ElectronPhotonRelaxationDataContainer::setSubshellRelaxationVacancies( 
       const unsigned subshell,
       const std::vector<std::pair<unsigned,unsigned> >& relaxation_vacancies )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the relaxation vacancies are valid
  testPrecondition( relaxation_vacancies.size() ==
		    d_relaxation_transitions.find( subshell )->second );

  d_relaxation_vacancies[subshell] = relaxation_vacancies;
}

// Set the relaxation particle energies for a subshell
void ElectronPhotonRelaxationDataContainer::setSubshellRelaxationParticleEnergies(
		      const unsigned subshell,
		      const std::vector<double>& relaxation_particle_energies )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the relaxation particle energies are valid
  testPrecondition( relaxation_particle_energies.size() ==
		    d_relaxation_transitions.find( subshell )->second );

  d_relaxation_particle_energies[subshell] = relaxation_particle_energies;
}

// Set the relaxation probabilities for a subshell
void ElectronPhotonRelaxationDataContainer::setSubshellRelaxationProbabilities( 
			  const unsigned subshell,
			  const std::vector<double>& relaxation_probabilities )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the relaxation cdf is valid
  testPrecondition( relaxation_probabilities.size() ==
		    d_relaxation_transitions.find( subshell )->second );
  testPrecondition( std::find_if( relaxation_probabilities.begin(),
				  relaxation_probabilities.end(),
				  isValueLessThanOrEqualToZero ) ==
		    relaxation_probabilities.end() );
  
  d_relaxation_probabilities[subshell] = relaxation_probabilities;
}
  
//---------------------------------------------------------------------------//
// SET PHOTON DATA
//---------------------------------------------------------------------------//

// Set the Compton profile momentum grid for a subshell
void ElectronPhotonRelaxationDataContainer::setComptonProfileMomentumGrid(
		     const unsigned subshell,
		     const std::vector<double>& compton_profile_momentum_grid )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the momentum grid is valid
  testPrecondition( compton_profile_momentum_grid.size() > 1 );
  testPrecondition( Utility::Sort::isSortedAscending( 
				       compton_profile_momentum_grid.begin(),
				       compton_profile_momentum_grid.end() ) );
  testPrecondition( compton_profile_momentum_grid.front() == -1.0 );

  d_compton_profile_momentum_grids[subshell] = compton_profile_momentum_grid;
}
  
// Set the Compton profile for a subshell
void ElectronPhotonRelaxationDataContainer::setComptonProfile( 
				   const unsigned subshell,
				   const std::vector<double>& compton_profile )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the compton_profile is valid
  testPrecondition( compton_profile.size() ==
		    d_compton_profile_momentum_grids.find( subshell )->second.size() );
  testPrecondition( std::find_if( compton_profile.begin(),
				  compton_profile.end(),
				  isValueLessThanOrEqualToZero ) ==
		    compton_profile.end() );

  d_compton_profiles[subshell] = compton_profile;
}
  
// Set the occupation number momentum grid for a subshell
void ElectronPhotonRelaxationDataContainer::setOccupationNumberMomentumGrid( 
		    const unsigned subshell,
		    const std::vector<double>& occupation_number_momentum_grid )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the occupation number momentum grid is valid
  testPrecondition( occupation_number_momentum_grid.size() > 1 );
  testPrecondition( Utility::Sort::isSortedAscending( 
					occupation_number_momentum_grid.begin(),
					occupation_number_momentum_grid.end()));
  testPrecondition( occupation_number_momentum_grid.front() == -1.0 );

  d_occupation_number_momentum_grids[subshell] = occupation_number_momentum_grid;
}
  
// Set the occupation number for a subshell
void ElectronPhotonRelaxationDataContainer::setOccupationNumber( 
				  const unsigned subshell,
				  const std::vector<double>& occupation_number )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the occupation number is valid
  testPrecondition( occupation_number.size() ==
		    d_occupation_number_momentum_grids.find( subshell )->second.size() );
  testPrecondition( occupation_number.front() == 0.0 );
  testPrecondition( occupation_number.back() <= 1.0 );

  d_occupation_numbers[subshell] = occupation_number;
}

// Set the Waller-Hartree scattering function momentum grid
void ElectronPhotonRelaxationDataContainer::setWallerHartreeScatteringFunctionMomentumGrid(
				     const std::vector<double>& momentum_grid )
{
  // Make sure the momentum grid is valid
  testPrecondition( momentum_grid.size() > 1 );
  testPrecondition( Utility::Sort::isSortedAscending( momentum_grid.begin(),
						      momentum_grid.end() ) );
  testPrecondition( std::find_if( momentum_grid.begin(),
				  momentum_grid.end(),
				  isValueLessThanZero ) ==
		    momentum_grid.end() );
  
  d_waller_hartree_scattering_function_momentum_grid = momentum_grid;
}
  
// Set the Waller-Hartree scattering function 
void ElectronPhotonRelaxationDataContainer::setWallerHartreeScatteringFunction(
			       const std::vector<double>& scattering_function )
{
  // Make sure the scattering function is valid
  testPrecondition( scattering_function.size() == 
		    d_waller_hartree_scattering_function_momentum_grid.size());
  testPrecondition( scattering_function.front() >= 0.0 );
  testPrecondition( scattering_function.back() == d_atomic_number );

  d_waller_hartree_scattering_function = scattering_function;
}

// Set the Waller-Hartree atomic form factor momentum grid
void ElectronPhotonRelaxationDataContainer::setWallerHartreeAtomicFormFactorMomentumGrid(
				     const std::vector<double>& momentum_grid )
{
  // Make sure the momentum grid is valid
  testPrecondition( momentum_grid.size() > 1 );
  testPrecondition( Utility::Sort::isSortedAscending( momentum_grid.begin(),
						      momentum_grid.end() ) );
  testPrecondition( std::find_if( momentum_grid.begin(),
				  momentum_grid.end(),
				  isValueLessThanZero ) ==
		    momentum_grid.end() );
  
  d_waller_hartree_atomic_form_factor_momentum_grid = momentum_grid;
}
  
// Set the Waller-Hartree atomic form factor 
void ElectronPhotonRelaxationDataContainer::setWallerHartreeAtomicFormFactor(
			        const std::vector<double>& atomic_form_factor )
{
  // Make sure the atomic form factor is valid
  testPrecondition( atomic_form_factor.size() ==
		    d_waller_hartree_atomic_form_factor_momentum_grid.size() );
  testPrecondition( Utility::Sort::isSortedAscending( 
						 atomic_form_factor.rbegin(),
						 atomic_form_factor.rend() ) );
  testPrecondition( atomic_form_factor.front() == d_atomic_number );
  testPrecondition( atomic_form_factor.back() >= 0.0 );

  d_waller_hartree_atomic_form_factor = atomic_form_factor;
}
  
// Set the photon energy grid
void ElectronPhotonRelaxationDataContainer::setPhotonEnergyGrid( 
				       const std::vector<double>& energy_grid )
{
  // Make sure the energy grid is valid
  testPrecondition( energy_grid.size() > 1 );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
						      energy_grid.end() ) );
  testPrecondition( energy_grid.front() > 0.0 );

  d_photon_energy_grid = energy_grid;
}

// Set the average photon heating numbers
void ElectronPhotonRelaxationDataContainer::setAveragePhotonHeatingNumbers( 
				   const std::vector<double>& heating_numbers )
{
  // Make sure the heating numbers are valid
  testPrecondition( heating_numbers.size() == d_photon_energy_grid.size() );
  testPrecondition( std::find_if( heating_numbers.begin(),
				  heating_numbers.end(),
				  isValueLessThanOrEqualToZero ) ==
		    heating_numbers.end() );

  d_average_photon_heating_numbers = heating_numbers;
}
  
// Set the incoherent photon cross section using Waller-Hartree (WH) theory
void ElectronPhotonRelaxationDataContainer::setWallerHartreeIncoherentCrossSection(
			  const std::vector<double>& incoherent_cross_section )
{
  // Make sure the incoherent cross section is valid
  testPrecondition( incoherent_cross_section.size() <= 
		    d_photon_energy_grid.size() );
  testPrecondition( std::find_if( incoherent_cross_section.begin(),
				  incoherent_cross_section.end(),
				  isValueLessThanOrEqualToZero ) ==
		    incoherent_cross_section.end() );
  
  d_waller_hartree_incoherent_cross_section = incoherent_cross_section;
}

// Set the WH incoherent cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setWallerHartreeIncoherentCrossSectionThresholdEnergyIndex(
						         const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( d_waller_hartree_incoherent_cross_section.size() + index ==
		    d_photon_energy_grid.size() );
  
  d_waller_hartree_incoherent_cross_section_threshold_index = index;
}
  
// Set the incoherent photon cross section using the impulse approx. (IA)
void ElectronPhotonRelaxationDataContainer::setImpulseApproxIncoherentCrossSection(
			  const std::vector<double>& incoherent_cross_section )
{
  // Make sure the incoherent cross section is valid
  testPrecondition( incoherent_cross_section.size() <= 
		    d_photon_energy_grid.size() );
  testPrecondition( std::find_if( incoherent_cross_section.begin(),
				  incoherent_cross_section.end(),
				  isValueLessThanOrEqualToZero ) ==
		    incoherent_cross_section.end() );
  
  d_impulse_approx_incoherent_cross_section = incoherent_cross_section;
}

// Set the IA incoherent photon cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setImpulseApproxIncoherentCrossSectionThresholdEnergyIndex(
							 const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( d_impulse_approx_incoherent_cross_section.size() + index ==
		    d_photon_energy_grid.size() );
  
  d_impulse_approx_incoherent_cross_section_threshold_index = index;
}

// Set the IA subshell incoherent photon cross section
void ElectronPhotonRelaxationDataContainer::setImpulseApproxSubshellIncoherentCrossSection(
			  const unsigned subshell,
			  const std::vector<double>& incoherent_cross_section )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the incoherent cross section is valid
  testPrecondition( incoherent_cross_section.size() <= 
		    d_photon_energy_grid.size() );
  testPrecondition( std::find_if( incoherent_cross_section.begin(),
				  incoherent_cross_section.end(),
				  isValueLessThanOrEqualToZero ) ==
		    incoherent_cross_section.end() );
  
  d_impulse_approx_subshell_incoherent_cross_sections[subshell] = 
    incoherent_cross_section;
}

// Set the IA subshell incoherent photon cs threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setImpulseApproxSubshellIncoherentCrossSectionThresholdEnergyIndex(
						       const unsigned subshell,
						       const unsigned index )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the threshold index is valid
  testPrecondition( d_impulse_approx_subshell_incoherent_cross_sections.find( subshell ) !=
		    d_impulse_approx_subshell_incoherent_cross_sections.end());
  remember( const std::vector<double>& incoherent_cross_section = 
	    d_impulse_approx_subshell_incoherent_cross_sections.find( subshell )->second );
  testPrecondition( incoherent_cross_section.size() + index ==
		    d_photon_energy_grid.size() );
  
  d_impulse_approx_subshell_incoherent_cross_section_theshold_indices[subshell] =
    index;
}
  
// Set the WH coherent cross section 
void ElectronPhotonRelaxationDataContainer::setWallerHartreeCoherentCrossSection(
			    const std::vector<double>& coherent_cross_section )
{
  // Make sure the coherent cross section is valid
  testPrecondition( coherent_cross_section.size() <= 
		    d_photon_energy_grid.size() );
  testPrecondition( std::find_if( coherent_cross_section.begin(),
				  coherent_cross_section.end(),
				  isValueLessThanOrEqualToZero ) ==
		    coherent_cross_section.end() );
  
  d_waller_hartree_coherent_cross_section = coherent_cross_section;
}

// Set the WH coherent cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setWallerHartreeCoherentCrossSectionThresholdEnergyIndex(
							 const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( d_waller_hartree_coherent_cross_section.size() + index ==
		    d_photon_energy_grid.size() );
  
  d_waller_hartree_coherent_cross_section_threshold_index = index;
}
  
// Set the pair production cross section
void ElectronPhotonRelaxationDataContainer::setPairProductionCrossSection(
		     const std::vector<double>& pair_production_cross_section )
{
  // Make sure the pair production cross section is valid
  testPrecondition( pair_production_cross_section.size() <=
		    d_photon_energy_grid.size() );
  testPrecondition( std::find_if( pair_production_cross_section.begin(),
				  pair_production_cross_section.end(),
				  isValueLessThanOrEqualToZero ) ==
		    pair_production_cross_section.end() );

  d_pair_production_cross_section = pair_production_cross_section;
}

// Set the pair production cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setPairProductionCrossSectionThresholdEnergyIndex( 
							 const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( d_pair_production_cross_section.size() + index ==
		    d_photon_energy_grid.size() );
  
  d_pair_production_cross_section_threshold_index = index;
}

// Set the Photoelectric effect cross section
void ElectronPhotonRelaxationDataContainer::setPhotoelectricCrossSection(
		       const std::vector<double>& photoelectric_cross_section )
{
  // Make sure the photoelectric cross section is valid
  testPrecondition( photoelectric_cross_section.size() <=
		    d_photon_energy_grid.size() );
  
  d_photoelectric_cross_section = photoelectric_cross_section;
}

// Set the Photoelectric effect cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setPhotoelectricCrossSectionThresholdEnergyIndex(
							 const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( d_photoelectric_cross_section.size() + index ==
		    d_photon_energy_grid.size() );
  
  d_photoelectric_cross_section_threshold_index = index;
}
  
// Set the Photoelectric effect cross section for a subshell
void ElectronPhotonRelaxationDataContainer::setSubshellPhotoelectricCrossSection( 
		       const unsigned subshell,
		       const std::vector<double>& photoelectric_cross_section )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the photoelectric cross section is valid
  testPrecondition( photoelectric_cross_section.size() <= 
		    d_photon_energy_grid.size() );
  testPrecondition( std::find_if( photoelectric_cross_section.begin(),
				  photoelectric_cross_section.end(),
				  isValueLessThanOrEqualToZero ) ==
		    photoelectric_cross_section.end() );
  
  d_subshell_photoelectric_cross_sections[subshell] = 
    photoelectric_cross_section;
}
  
// Set the subshell Photoelectric effect cross section threshold index
void ElectronPhotonRelaxationDataContainer::setSubshellPhotoelectricCrossSectionThresholdEnergyIndex(
						       const unsigned subshell,
						       const unsigned index )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the index is valid
  testPrecondition( d_subshell_photoelectric_cross_sections.find( subshell ) !=
		    d_subshell_photoelectric_cross_sections.end() );
  remember( const std::vector<double> photoelectric_cross_section = 
	    d_subshell_photoelectric_cross_sections.find( subshell )->second );
  testPrecondition( photoelectric_cross_section.size() + index ==
		    d_photon_energy_grid.size() );
  
  d_subshell_photoelectric_cross_section_threshold_indices[subshell] = index;
}

// Set the Waller-Hartree total cross section
void ElectronPhotonRelaxationDataContainer::setWallerHartreeTotalCrossSection( 
			       const std::vector<double>& total_cross_section )
{
  // Make sure the total cross section is valid
  testPrecondition( total_cross_section.size() == d_photon_energy_grid.size());
  testPrecondition( std::find_if( total_cross_section.begin(),
				  total_cross_section.end(),
				  isValueLessThanOrEqualToZero ) ==
		    total_cross_section.end() );
  
  d_waller_hartree_total_cross_section = total_cross_section;
}

// Set the impulse approx. total cross section
void ElectronPhotonRelaxationDataContainer::setImpulseApproxTotalCrossSection(
			       const std::vector<double>& total_cross_section )
{
  // Make sure the total cross section is valid
  testPrecondition( total_cross_section.size() == d_photon_energy_grid.size());
  testPrecondition( std::find_if( total_cross_section.begin(),
				  total_cross_section.end(),
				  isValueLessThanOrEqualToZero ) ==
		    total_cross_section.end() );
  
  d_impulse_approx_total_cross_section = total_cross_section;
}


//---------------------------------------------------------------------------//
// SET ELECTRON DATA 
//---------------------------------------------------------------------------//

// Set the elastic cutoff angle
void ElectronPhotonRelaxationDataContainer::setCutoffAngle( 
                         const double cutoff_angle )
{
  // Make sure the elastic cutoff angle is valid
  testPrecondition( cutoff_angle >= 0.0 );
  testPrecondition( cutoff_angle < 2.0 );

  d_cutoff_angle = cutoff_angle;
}

// Set the elastic angular energy grid
void ElectronPhotonRelaxationDataContainer::setElasticAngularEnergyGrid( 
				       const std::vector<double>& angular_energy_grid )
{
  // Make sure the angular energy grid is valid
  testPrecondition( angular_energy_grid.back() > 0 );
  testPrecondition( 
        Utility::Sort::isSortedAscending( angular_energy_grid.begin(),
			                              angular_energy_grid.end() ) );

  testPrecondition( std::find_if( angular_energy_grid.begin(),
                                  angular_energy_grid.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    angular_energy_grid.end() );

  d_angular_energy_grid = angular_energy_grid;
}

// Set the total elastic angles for an incoming energy
void ElectronPhotonRelaxationDataContainer::setAnalogElasticAnglesAtEnergy(
    const double incoming_energy,
    const std::vector<double>& analog_elastic_angles )
{
  // Make sure the incoming_energy is valid
  testPrecondition( incoming_energy >= d_angular_energy_grid.front() );
  testPrecondition( incoming_energy <= d_angular_energy_grid.back() );
  // Make sure the elastic angles are valid
  testPrecondition( std::find_if( analog_elastic_angles.begin(),
                                  analog_elastic_angles.end(),
                                  isValueLessThanMinusOne ) ==
                    analog_elastic_angles.end() );
  testPrecondition( std::find_if( analog_elastic_angles.begin(),
                                  analog_elastic_angles.end(),
                                  isValueGreaterThanOne ) ==
                    analog_elastic_angles.end() );

  d_analog_elastic_angles[incoming_energy] = analog_elastic_angles;
}

// Set the total elastic pdf for an incoming energy
void ElectronPhotonRelaxationDataContainer::setAnalogElasticPDFAtEnergy( 
    const double incoming_energy,
    const std::vector<double>& analog_elastic_pdf )
{
  // Make sure the incoming_energy is valid
  testPrecondition( incoming_energy >= d_angular_energy_grid.front() );
  testPrecondition( incoming_energy <= d_angular_energy_grid.back() );
  // Make sure the weight is valid
  testPrecondition( std::find_if( analog_elastic_pdf.begin(),
                                  analog_elastic_pdf.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    analog_elastic_pdf.end() );
  
  d_analog_elastic_pdf[incoming_energy] = analog_elastic_pdf;
}
// Set the total elastic angles
void ElectronPhotonRelaxationDataContainer::setAnalogElasticAngles(
    const std::map<double,std::vector<double> >& analog_elastic_angles )
{
  d_analog_elastic_angles = analog_elastic_angles;
}

// Set the total elastic pdf 
void ElectronPhotonRelaxationDataContainer::setAnalogElasticPDF( 
    const std::map<double,std::vector<double> >& analog_elastic_pdf )
{
  d_analog_elastic_pdf = analog_elastic_pdf;
}

// Set the screened Rutherford elastic normalization constant 
void ElectronPhotonRelaxationDataContainer::setScreenedRutherfordNormalizationConstant(
		     const std::vector<double>& screened_rutherford_normalization_constant )
{
  // Make sure the screened_rutherford_normalization_constants are valid
  testPrecondition( std::find_if( screened_rutherford_normalization_constant.begin(),
                                  screened_rutherford_normalization_constant.end(),
                                  isValueLessThanZero ) ==
                    screened_rutherford_normalization_constant.end() );

  d_screened_rutherford_normalization_constant = 
    screened_rutherford_normalization_constant;
}

// Set Moliere's screening constant 
void ElectronPhotonRelaxationDataContainer::setMoliereScreeningConstant( 
			 const std::vector<double>& moliere_screening_constant )
{
  d_moliere_screening_constant = moliere_screening_constant;
}

// Set the moment preserving elastic discrete angles for an incoming energy
void ElectronPhotonRelaxationDataContainer::setMomentPreservingElasticDiscreteAngles(
		     const double incoming_energy,
		     const std::vector<double>& moment_preserving_elastic_discrete_angles )
{
  // Make sure the incoming_energy is valid
  testPrecondition( incoming_energy >= d_angular_energy_grid.front() );
  testPrecondition( incoming_energy <= d_angular_energy_grid.back() );
  // Make sure the moment preserving elastic discrete angles are valid
 /* testPrecondition( moment_preserving_elastic_discrete_angles.size() ==
               d_number_of_discrete_angles.find( incoming_energy )->second );*/
  testPrecondition( std::find_if( moment_preserving_elastic_discrete_angles.begin(),
                                  moment_preserving_elastic_discrete_angles.end(),
                                  isValueLessThanMinusOne ) ==
                    moment_preserving_elastic_discrete_angles.end() );
  testPrecondition( std::find_if( moment_preserving_elastic_discrete_angles.begin(),
                                  moment_preserving_elastic_discrete_angles.end(),
                                  isValueGreaterThanOne ) ==
                    moment_preserving_elastic_discrete_angles.end() );

  d_moment_preserving_elastic_discrete_angles[incoming_energy] = 
        moment_preserving_elastic_discrete_angles;
}

// Set the moment preserving elastic weights for an incoming energy
void ElectronPhotonRelaxationDataContainer::setMomentPreservingElasticWeights( 
			 const double incoming_energy,
			 const std::vector<double>& moment_preserving_elastic_weights )
{
  // Make sure the incoming_energy is valid
  testPrecondition( incoming_energy >= d_angular_energy_grid.front() );
  testPrecondition( incoming_energy <= d_angular_energy_grid.back() );
  // Make sure the weight is valid
  /*testPrecondition( moment_preserving_elastic_weights.size() ==
               d_number_of_discrete_angles.find( incoming_energy )->second );*/
  testPrecondition( std::find_if( moment_preserving_elastic_weights.begin(),
                                  moment_preserving_elastic_weights.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    moment_preserving_elastic_weights.end() );
  testPrecondition( std::find_if( moment_preserving_elastic_weights.begin(),
                                  moment_preserving_elastic_weights.end(),
                                  isValueGreaterThanOne ) ==
                    moment_preserving_elastic_weights.end() );
  
  d_moment_preserving_elastic_weights[incoming_energy] = moment_preserving_elastic_weights;
}

// Set the electroionization energy grid for a subshell
void ElectronPhotonRelaxationDataContainer::setElectroionizationEnergyGrid(
            const unsigned subshell, 
            const std::vector<double>& electroionization_energy_grid )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  testPrecondition( Utility::Sort::isSortedAscending( 
                        electroionization_energy_grid.begin(),
                        electroionization_energy_grid.end() ) );

  d_electroionization_energy_grid[subshell]=electroionization_energy_grid;
}

// Set the electroionization recoil energy for a subshell and energy bin
void ElectronPhotonRelaxationDataContainer::setElectroionizationRecoilEnergyAtIncomingEnergy( 
            const unsigned subshell, 
            const double incoming_energy,
            const std::vector<double>& electroionization_recoil_energy )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy >= d_electroionization_energy_grid[subshell].front() );
  testPrecondition( incoming_energy <= d_electroionization_energy_grid[subshell].back() );
  // Make sure the electroionization recoil energy is valid
  testPrecondition( std::find_if( electroionization_recoil_energy.begin(),
                                  electroionization_recoil_energy.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    electroionization_recoil_energy.end() );

  d_electroionization_recoil_energy[subshell][ incoming_energy] =
    electroionization_recoil_energy;
}

// Set the electroionization recoil energy pdf for a subshell and energy bin
void ElectronPhotonRelaxationDataContainer::setElectroionizationRecoilPDFAtIncomingEnergy(
            const unsigned subshell,
            const double incoming_energy,
            const std::vector<double>& electroionization_recoil_pdf )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy >= d_electroionization_energy_grid[subshell].front() );
  testPrecondition( incoming_energy <= d_electroionization_energy_grid[subshell].back() );
  // Make sure the electroionization recoil pdf is valid
  testPrecondition( std::find_if( electroionization_recoil_pdf.begin(),
                                  electroionization_recoil_pdf.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    electroionization_recoil_pdf.end() );

  d_electroionization_recoil_pdf[subshell][ incoming_energy] =
    electroionization_recoil_pdf;
}

// Set electroionization recoil energy for all incoming energies in a subshell
void ElectronPhotonRelaxationDataContainer::setElectroionizationRecoilEnergy( 
    const unsigned subshell, 
    const std::map<double,std::vector<double> >& electroionization_recoil_energy )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );

  d_electroionization_recoil_energy[subshell] =
    electroionization_recoil_energy;
}

// Set electroionization recoil energy pdf for all incoming energies in a subshell
void ElectronPhotonRelaxationDataContainer::setElectroionizationRecoilPDF(
    const unsigned subshell,
    const std::map<double,std::vector<double> >& electroionization_recoil_pdf )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );

  d_electroionization_recoil_pdf[subshell] =
    electroionization_recoil_pdf;
}

// Set the bremsstrahlung energy grid
void ElectronPhotonRelaxationDataContainer::setBremsstrahlungEnergyGrid( 
				       const std::vector<double>& bremsstrahlung_energy_grid )
{
  // Make sure the energy grid is valid
  testPrecondition( 
        Utility::Sort::isSortedAscending( bremsstrahlung_energy_grid.begin(),
			                              bremsstrahlung_energy_grid.end() ) );

  testPrecondition( bremsstrahlung_energy_grid.front() > 0.0 );

  d_bremsstrahlung_energy_grid = bremsstrahlung_energy_grid;
}

// Set the bremsstrahlung photon energy for an incoming energy
void ElectronPhotonRelaxationDataContainer::setBremsstrahlungPhotonEnergyAtIncomingEnergy(
		     const double incoming_energy,
		     const std::vector<double>&  bremsstrahlung_photon_energy )
{
  // Make sure the incoming_energy is valid
  testPrecondition( incoming_energy >= d_bremsstrahlung_energy_grid.front() );
  testPrecondition( incoming_energy <= d_bremsstrahlung_energy_grid.back() );
  // Make sure the bremsstrahlung photon energies are valid
  testPrecondition( std::find_if( bremsstrahlung_photon_energy.begin(),
                                  bremsstrahlung_photon_energy.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    bremsstrahlung_photon_energy.end() );

  d_bremsstrahlung_photon_energy[incoming_energy] = 
    bremsstrahlung_photon_energy;
}

// Set the bremsstrahlung photon pdf for an incoming energy
void ElectronPhotonRelaxationDataContainer::setBremsstrahlungPhotonPDFAtIncomingEnergy( 
			 const double incoming_energy,
			 const std::vector<double>& bremsstrahlung_photon_pdf )
{
  // Make sure the incoming_energy is valid
  testPrecondition( incoming_energy >= d_bremsstrahlung_energy_grid.front() );
  testPrecondition( incoming_energy <= d_bremsstrahlung_energy_grid.back() );
  // Make sure the pdf is valid
  testPrecondition( std::find_if( bremsstrahlung_photon_pdf.begin(),
                                  bremsstrahlung_photon_pdf.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    bremsstrahlung_photon_pdf.end() );
  
  d_bremsstrahlung_photon_pdf[incoming_energy] = bremsstrahlung_photon_pdf;
}

// Set all the bremsstrahlung photon energy
void ElectronPhotonRelaxationDataContainer::setBremsstrahlungPhotonEnergy(
    const std::map<double,std::vector<double> >&  bremsstrahlung_photon_energy )
{
  d_bremsstrahlung_photon_energy = bremsstrahlung_photon_energy;
}

// Set all the bremsstrahlung photon pdf
void ElectronPhotonRelaxationDataContainer::setBremsstrahlungPhotonPDF( 
    const std::map<double,std::vector<double> >& bremsstrahlung_photon_pdf )
{
  d_bremsstrahlung_photon_pdf = bremsstrahlung_photon_pdf;
}

// Set the atomic excitation energy grid
void ElectronPhotonRelaxationDataContainer::setAtomicExcitationEnergyGrid( 
				       const std::vector<double>& atomic_excitation_energy_grid )
{
  // Make sure the energy grid is valid
  testPrecondition( Utility::Sort::isSortedAscending( 
                        atomic_excitation_energy_grid.begin(),
                        atomic_excitation_energy_grid.end() ) );

  testPrecondition( atomic_excitation_energy_grid.front() > 0.0 );

  d_atomic_excitation_energy_grid = atomic_excitation_energy_grid;
}

// Set the atomic excitation energy loss for an incoming energy
void ElectronPhotonRelaxationDataContainer::setAtomicExcitationEnergyLoss(
		     const std::vector<double>&  atomic_excitation_energy_loss )
{
  // Make sure the atomic excitation energy loss are valid
  testPrecondition( std::find_if( atomic_excitation_energy_loss.begin(),
                                  atomic_excitation_energy_loss.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    atomic_excitation_energy_loss.end() );

  d_atomic_excitation_energy_loss = 
    atomic_excitation_energy_loss;
}

// Set the electron energy grid
void ElectronPhotonRelaxationDataContainer::setElectronEnergyGrid( 
				       const std::vector<double>& energy_grid )
{
  // Make sure the energy grid is valid
  testPrecondition( energy_grid.back() > 1 );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
						                              energy_grid.end() ) );
  testPrecondition( energy_grid.front() > 0.0 );

  d_electron_energy_grid = energy_grid;
}

// Set the cutoff elastic electron cross section 
void ElectronPhotonRelaxationDataContainer::setCutoffElasticCrossSection(
			 const std::vector<double>& cutoff_elastic_cross_section )
{
  // Make sure the cutoff elastic cross section is valid
  testPrecondition( cutoff_elastic_cross_section.size() <= 
                    d_electron_energy_grid.size() );
  testPrecondition( std::find_if( cutoff_elastic_cross_section.begin(),
                                  cutoff_elastic_cross_section.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    cutoff_elastic_cross_section.end() );
  
  d_cutoff_elastic_cross_section = cutoff_elastic_cross_section;
}

// Set the cutoff elastic cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setCutoffElasticCrossSectionThresholdEnergyIndex(
						        const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( 
        d_cutoff_elastic_cross_section.size() + index ==
        d_electron_energy_grid.size() );
  
 d_cutoff_elastic_cross_section_threshold_index = index;
}

// Set the screened rutherford elastic electron cross section 
void ElectronPhotonRelaxationDataContainer::setScreenedRutherfordElasticCrossSection(
			 const std::vector<double>& screened_rutherford_elastic_cross_section )
{
  // Make sure the screened rutherford elastic cross section is valid
  testPrecondition( screened_rutherford_elastic_cross_section.size() <= 
                    d_electron_energy_grid.size() );
  testPrecondition( std::find_if( screened_rutherford_elastic_cross_section.begin(),
                                  screened_rutherford_elastic_cross_section.end(),
                                  isValueLessThanZero ) ==
                    screened_rutherford_elastic_cross_section.end() );
  
  d_screened_rutherford_elastic_cross_section = screened_rutherford_elastic_cross_section;
}

// Set the screened rutherford elastic cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setScreenedRutherfordElasticCrossSectionThresholdEnergyIndex(
						        const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( 
        d_screened_rutherford_elastic_cross_section.size() + index ==
        d_electron_energy_grid.size() );
  
 d_screened_rutherford_elastic_cross_section_threshold_index = index;
}

// Set the total elastic electron cross section 
void ElectronPhotonRelaxationDataContainer::setTotalElasticCrossSection(
			 const std::vector<double>& total_elastic_cross_section )
{
  // Make sure the total elastic cross section is valid
  testPrecondition( total_elastic_cross_section.size() <= 
                    d_electron_energy_grid.size() );
  testPrecondition( std::find_if( total_elastic_cross_section.begin(),
                                  total_elastic_cross_section.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    total_elastic_cross_section.end() );
  
  d_total_elastic_cross_section = total_elastic_cross_section;
}

// Set the total elastic cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setTotalElasticCrossSectionThresholdEnergyIndex(
						        const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( 
        d_total_elastic_cross_section.size() + index ==
        d_electron_energy_grid.size() );
  
 d_total_elastic_cross_section_threshold_index = index;
}

// Set the elastic electron cross section using Moment Preserving (MP) theory
void ElectronPhotonRelaxationDataContainer::setMomentPreservingCrossSection(
			 const std::vector<double>& moment_preserving_elastic_cross_section )
{
  // Make sure the moment preserving elastic cross section is valid
  testPrecondition( moment_preserving_elastic_cross_section.size() <= 
                    d_electron_energy_grid.size() );
  testPrecondition( std::find_if( moment_preserving_elastic_cross_section.begin(),
                                  moment_preserving_elastic_cross_section.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    moment_preserving_elastic_cross_section.end() );
  
  d_moment_preserving_elastic_cross_section = 
        moment_preserving_elastic_cross_section;
}

// Set the MP elastic cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setMomentPreservingCrossSectionThresholdEnergyIndex(
						        const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( 
        d_moment_preserving_elastic_cross_section.size() + index ==
        d_electron_energy_grid.size() );
  
 d_moment_preserving_elastic_cross_section_threshold_index= index;
}

// Set the electroionization electron cross section 
void ElectronPhotonRelaxationDataContainer::setElectroionizationCrossSection(
            const unsigned subshell,
            const std::vector<double>& electroionization_cross_section )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the electroionization cross section is valid
  testPrecondition( electroionization_cross_section.size() <= 
                    d_electron_energy_grid.size() );
  testPrecondition( std::find_if( electroionization_cross_section.begin(),
                                  electroionization_cross_section.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    electroionization_cross_section.end() );
  
  d_electroionization_subshell_cross_section[subshell] = 
    electroionization_cross_section;
}

// Set the electroionization cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setElectroionizationCrossSectionThresholdEnergyIndex(
            const unsigned subshell,
	        const unsigned index )
{
  // Make sure the subshell is valid
  testPrecondition( d_subshells.find( subshell ) != d_subshells.end() );
  // Make sure the threshold index is valid
  testPrecondition( 
        d_electroionization_subshell_cross_section[subshell].size() + index ==
        d_electron_energy_grid.size() );
  
 d_electroionization_subshell_cross_section_threshold_index[subshell] = index;
}

// Set the bremsstrahlung electron cross section 
void ElectronPhotonRelaxationDataContainer::setBremsstrahlungCrossSection(
			 const std::vector<double>& bremsstrahlung_cross_section )
{
  // Make sure the bremsstrahlung cross section is valid
  testPrecondition( bremsstrahlung_cross_section.size() <= 
                    d_electron_energy_grid.size() );
  testPrecondition( std::find_if( bremsstrahlung_cross_section.begin(),
                                  bremsstrahlung_cross_section.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    bremsstrahlung_cross_section.end() );
  
  d_bremsstrahlung_cross_section = bremsstrahlung_cross_section;
}

// Set the bremsstrahlung cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setBremsstrahlungCrossSectionThresholdEnergyIndex(
						        const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( 
        d_bremsstrahlung_cross_section.size() + index ==
        d_electron_energy_grid.size() );
  
 d_bremsstrahlung_cross_section_threshold_index = index;
}

// Set the atomic excitation electron cross section 
void ElectronPhotonRelaxationDataContainer::setAtomicExcitationCrossSection(
			 const std::vector<double>& atomic_excitation_cross_section )
{
  // Make sure the atomic excitation cross section is valid
  testPrecondition( atomic_excitation_cross_section.size() <= 
                    d_electron_energy_grid.size() );
  testPrecondition( std::find_if( atomic_excitation_cross_section.begin(),
                                  atomic_excitation_cross_section.end(),
                                  isValueLessThanOrEqualToZero ) ==
                    atomic_excitation_cross_section.end() );
  
  d_atomic_excitation_cross_section = atomic_excitation_cross_section;
}

// Set the atomic excitation cross section threshold energy bin index
void ElectronPhotonRelaxationDataContainer::setAtomicExcitationCrossSectionThresholdEnergyIndex(
						        const unsigned index )
{
  // Make sure the threshold index is valid
  testPrecondition( 
        d_atomic_excitation_cross_section.size() + index ==
        d_electron_energy_grid.size() );
  
 d_atomic_excitation_cross_section_threshold_index = index;
}

// Test if a value is less than or equal to zero
bool ElectronPhotonRelaxationDataContainer::isValueLessThanOrEqualToZero( 
							   const double value )
{
  return value <= 0.0;
}

// Test if a value is less than zero
bool ElectronPhotonRelaxationDataContainer::isValueLessThanZero( 
							   const double value )
{
  return value < 0.0;
}

// Test if a value is greater than one
bool ElectronPhotonRelaxationDataContainer::isValueGreaterThanOne( 
							   const double value )
{
  return value > 1.0;
}

// Test if a value is less than -1.0
bool ElectronPhotonRelaxationDataContainer::isValueLessThanMinusOne( 
							   const double value )
{
  return value < -1.0;
}

} // end Data namespace

//---------------------------------------------------------------------------//
// end Data_ElectronPhotonRelaxationDataContainer.cpp
//---------------------------------------------------------------------------//
