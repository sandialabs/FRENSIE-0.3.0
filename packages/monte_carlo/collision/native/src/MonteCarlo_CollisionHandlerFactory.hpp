//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_CollisionHandlerFactory.hpp
//! \author Alex Robinson
//! \brief  Collision handler factory class declaration.
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_COLLISION_HANDLER_FACTORY_HPP
#define MONTE_CARLO_COLLISION_HANDLER_FACTORY_HPP

// Std Lib Includes
#include <stdexcept>

// Trilinos Includes
#include <Teuchos_RCP.hpp>
#include <Teuchos_ParameterList.hpp>

// FRENSIE Includes
#include "MonteCarlo_CollisionHandler.hpp"
#include "MonteCarlo_AtomicRelaxationModelFactory.hpp"
#include "MonteCarlo_IncoherentModelType.hpp"
#include "MonteCarlo_BremsstrahlungAngularDistributionType.hpp"

namespace MonteCarlo{

//! The collision handler factory
class CollisionHandlerFactory
{

public:

  //! Initialize the collision handler
  virtual void initializeHandler( 
		     const Teuchos::ParameterList& material_reps,
		     const Teuchos::ParameterList& cross_sections_table_info,
		     const std::string& cross_sections_xml_directory );

  // Validate a material representation
  virtual void validateMaterialRep( 
	      const Teuchos::ParameterList& material_rep,
	      boost::unordered_set<Geometry::ModuleTraits::InternalCellHandle>&
	      material_ids );

  // Create the set of all nuclides/atoms needed to construct materials
  virtual void createAliasSet( 
		       const Teuchos::ParameterList& material_reps,
		       const Teuchos::ParameterList& cross_sections_alias_map,
		       boost::unordered_set<std::string>& nuclides );

  // Create the material id data maps
  virtual void createMaterialIdDataMaps( 
    const Teuchos::ParameterList& material_reps,
    boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                         Teuchos::Array<double> >& material_id_fraction_map,
    boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                    Teuchos::Array<std::string> >& material_id_component_map );

  // Create the neutron materials
  virtual void createNeutronMaterials( 
   const Teuchos::ParameterList& cross_sections_table_info,
   const std::string& cross_sections_xml_directory,
   const boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                            Teuchos::Array<double> >& material_id_fraction_map,
   const boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                      Teuchos::Array<std::string> >& material_id_component_map,
   const boost::unordered_set<std::string>& nuclide_aliases,
   const boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                              std::vector<std::string> >& cell_id_mat_id_map,
   const boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                               std::vector<std::string> >& cell_id_density_map,
   const bool use_unresolved_resonance_data,
   const bool use_photon_production_data );
   
  // Create the photon materials
  virtual void createPhotonMaterials(
   const Teuchos::ParameterList& cross_sections_table_info,
   const std::string& cross_sections_xml_directory,
   const boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                            Teuchos::Array<double> >& material_id_fraction_map,
   const boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                      Teuchos::Array<std::string> >& material_id_component_map,
   const boost::unordered_set<std::string>& photoatom_aliases,
   const boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                              std::vector<std::string> >& cell_id_mat_id_map,
   const boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                               std::vector<std::string> >& cell_id_density_map,
   const Teuchos::RCP<AtomicRelaxationModelFactory>& 
   atomic_relaxation_model_factory,
   const unsigned hash_grid_bins,
   const IncoherentModelType incoherent_model,
   const double kahn_sampling_cutoff_energy,
   const bool use_detailed_pair_production_data,
   const bool use_atomic_relaxation_data,
   const bool use_photonuclear_data );

  // Create the electron materials
  virtual void createElectronMaterials(
   const Teuchos::ParameterList& cross_sections_table_info,
   const std::string& cross_sections_xml_directory,
   const boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                            Teuchos::Array<double> >& material_id_fraction_map,
   const boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                      Teuchos::Array<std::string> >& material_id_component_map,
   const boost::unordered_set<std::string>& electroatom_aliases,
   const boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                              std::vector<std::string> >& cell_id_mat_id_map,
   const boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                               std::vector<std::string> >& cell_id_density_map,
   const Teuchos::RCP<AtomicRelaxationModelFactory>& 
   atomic_relaxation_model_factory,
   const BremsstrahlungAngularDistributionType photon_distribution_function,
   const bool use_atomic_relaxation_data );

  // Create the material name data maps
  template<typename ScatteringCenterType, typename MaterialType>
  static void createMaterialNameDataMaps(
   const boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                            Teuchos::Array<double> >& material_id_fraction_map,
   const boost::unordered_map<ModuleTraits::InternalMaterialHandle,
                      Teuchos::Array<std::string> >& material_id_component_map,
   const boost::unordered_map<std::string,Teuchos::RCP<ScatteringCenterType> >&
   scattering_center_map,
   const boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                              std::vector<std::string> >& cell_id_mat_id_map,
   const boost::unordered_map<Geometry::ModuleTraits::InternalCellHandle,
                               std::vector<std::string> >& cell_id_density_map,
   boost::unordered_map<std::string,Teuchos::RCP<MaterialType> >&
   material_name_pointer_map,
   boost::unordered_map<std::string,
                  Teuchos::Array<Geometry::ModuleTraits::InternalCellHandle> >&
   material_name_cell_ids_map );  

  // Register materials with the collision handler
  template<typename MaterialType>
  static void registerMaterials(
   const boost::unordered_map<std::string,Teuchos::RCP<MaterialType> >&
   material_name_pointer_map,
   const boost::unordered_map<std::string,
                  Teuchos::Array<Geometry::ModuleTraits::InternalCellHandle> >&
   material_name_cell_ids_map );
   
private:

  // Constructor
  CollisionHandlerFactory();
  
};

//! The invalid material representation error
class InvalidMaterialRepresentation : public std::logic_error
{
  
public:

  InvalidMaterialRepresentation( const std::string& what_arg )
    : std::logic_error( what_arg )
  { /* ... */ }
};

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// Template includes
//---------------------------------------------------------------------------//

#include "MonteCarlo_CollisionHandlerFactory_def.hpp"

//---------------------------------------------------------------------------//

#endif // end MONTE_CARLO_COLLISION_HANDLER_FACTORY_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_CollisionHandlerFactory.hpp
//---------------------------------------------------------------------------//
