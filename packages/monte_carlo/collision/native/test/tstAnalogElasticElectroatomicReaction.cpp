//---------------------------------------------------------------------------//
//!
//! \file   tstAnalogElasticElectroatomicReaction.cpp
//! \author Luke Kersting
//! \brief  Analog Elastic electroatomic reaction unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_VerboseObject.hpp>
#include <Teuchos_RCP.hpp>
#include <Teuchos_ArrayRCP.hpp>

// FRENSIE Includes
#include "Data_ElectronPhotonRelaxationDataContainer.hpp"
#include "MonteCarlo_AnalogElasticElectroatomicReaction.hpp"
#include "MonteCarlo_AnalogElasticElectronScatteringDistribution.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_HistogramDistribution.hpp"
#include "Utility_UnitTestHarnessExtensions.hpp"
#include "Utility_TabularOneDDistribution.hpp"

//---------------------------------------------------------------------------//
// Testing Variables.
//---------------------------------------------------------------------------//

Teuchos::RCP<MonteCarlo::AnalogElasticElectroatomicReaction<Utility::LinLin> > analog_elastic_reaction;

//---------------------------------------------------------------------------//
// Testing Functions.
//---------------------------------------------------------------------------//
bool notEqualZero( double value )
{
  return value != 0.0;
}

//---------------------------------------------------------------------------//
// Tests
//---------------------------------------------------------------------------//
// Check that the reaction type can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction, getReactionType )
{
  TEST_EQUALITY_CONST( analog_elastic_reaction->getReactionType(),
		       MonteCarlo::ANALOG_ELASTIC_ELECTROATOMIC_REACTION );
}

//---------------------------------------------------------------------------//
// Check that the cutoff threshold energy can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction, getCutoffThresholdEnergy )
{
  TEST_EQUALITY_CONST( analog_elastic_reaction->getCutoffThresholdEnergy(),
                       1.0e-5 );
}

//---------------------------------------------------------------------------//
// Check that the screened Rutherford threshold energy can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction, getScreenedRutherfordThresholdEnergy )
{
  TEST_EQUALITY_CONST( analog_elastic_reaction->getScreenedRutherfordThresholdEnergy(),
                       2.5902400000E-01 );
}

//---------------------------------------------------------------------------//
// Check that the threshold energy can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction, getThresholdEnergy )
{
  TEST_EQUALITY_CONST( analog_elastic_reaction->getThresholdEnergy(),
                       1.0e-5 );
}

//---------------------------------------------------------------------------//
// Check that the number of electrons emitted from the rxn can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction, getNumberOfEmittedElectrons )
{
  TEST_EQUALITY_CONST( analog_elastic_reaction->getNumberOfEmittedElectrons(1e-3),
		       0u );

  TEST_EQUALITY_CONST( analog_elastic_reaction->getNumberOfEmittedElectrons(20.0),
		       0u );
}

//---------------------------------------------------------------------------//
// Check that the number of photons emitted from the rxn can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction, getNumberOfEmittedPhotons )
{
  TEST_EQUALITY_CONST( analog_elastic_reaction->getNumberOfEmittedPhotons(1e-3),
		       0u );

  TEST_EQUALITY_CONST( analog_elastic_reaction->getNumberOfEmittedPhotons(20.0),
		       0u );
}

//---------------------------------------------------------------------------//
// Check that the cutoff cross section can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction,
                   getCutoffCrossSection )
{

  double cross_section =
    analog_elastic_reaction->getCutoffCrossSection( 1.0E-05 );

  TEST_FLOATING_EQUALITY( cross_section, 2.74896E+08, 1e-12 );

  cross_section =
    analog_elastic_reaction->getCutoffCrossSection( 1.0E-03 );

  TEST_FLOATING_EQUALITY( cross_section, 2.80423E+06, 1e-12 );

  cross_section =
    analog_elastic_reaction->getCutoffCrossSection( 1.0E+05 );

  TEST_FLOATING_EQUALITY( cross_section, 1.31176E-05, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the screened Rutherford cross section can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction,
                   getScreenedRutherfordCrossSection )
{

  double cross_section =
    analog_elastic_reaction->getScreenedRutherfordCrossSection( 1.0E-05 );

  TEST_FLOATING_EQUALITY( cross_section, 0.0, 1e-12 );

  cross_section =
    analog_elastic_reaction->getScreenedRutherfordCrossSection( 1.0E-03 );

  TEST_FLOATING_EQUALITY( cross_section, 0.0, 1e-12 );

  cross_section =
    analog_elastic_reaction->getScreenedRutherfordCrossSection( 2.59024E-01 );

  TEST_FLOATING_EQUALITY( cross_section, 2.574552047073660E+00, 1e-12 );

  cross_section =
    analog_elastic_reaction->getScreenedRutherfordCrossSection( 1.0E+05 );

  TEST_FLOATING_EQUALITY( cross_section, 1.298709998688240E+04, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the analog cross section can be returned
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction,
                   getCrossSection )
{

  double cross_section =
    analog_elastic_reaction->getCrossSection( 1.0E-05 );

  TEST_FLOATING_EQUALITY( cross_section, 2.74896E+08, 1e-12 );

  cross_section =
    analog_elastic_reaction->getCrossSection( 1.0E-03 );

  TEST_FLOATING_EQUALITY( cross_section, 2.80423E+06, 1e-12 );

  cross_section =
    analog_elastic_reaction->getCrossSection( 1.0E+05 );

  TEST_FLOATING_EQUALITY( cross_section, 1.29871E+04, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the elastic reaction can be simulated
TEUCHOS_UNIT_TEST( AnalogElasticElectroatomicReaction, react )
{
  MonteCarlo::ElectronState electron( 0 );
  electron.setEnergy( 20.0 );
  electron.setDirection( 0.0, 0.0, 1.0 );

  MonteCarlo::ParticleBank bank;

  Data::SubshellType shell_of_interaction;

  analog_elastic_reaction->react( electron, bank, shell_of_interaction );

  TEST_EQUALITY_CONST( electron.getEnergy(), 20.0 );
  TEST_ASSERT( electron.getZDirection() < 2.0 );
  TEST_ASSERT( electron.getZDirection() > 0.0 );
  TEST_ASSERT( bank.isEmpty() );
  TEST_EQUALITY_CONST( shell_of_interaction, Data::UNKNOWN_SUBSHELL );
}

//---------------------------------------------------------------------------//
// Custom main function
//---------------------------------------------------------------------------//
int main( int argc, char** argv )
{
  std::string test_native_file_name;

  Teuchos::CommandLineProcessor& clp = Teuchos::UnitTestRepository::getCLP();

  clp.setOption( "test_native_file",
		 &test_native_file_name,
		 "Test Native file name" );

  const Teuchos::RCP<Teuchos::FancyOStream> out =
    Teuchos::VerboseObjectBase::getDefaultOStream();

  Teuchos::CommandLineProcessor::EParseCommandLineReturn parse_return =
    clp.parse(argc,argv);

  if ( parse_return != Teuchos::CommandLineProcessor::PARSE_SUCCESSFUL ) {
    *out << "\nEnd Result: TEST FAILED" << std::endl;
    return parse_return;
  }

  // Create reaction
  {
    // Get native data container
    Data::ElectronPhotonRelaxationDataContainer data_container =
        Data::ElectronPhotonRelaxationDataContainer( test_native_file_name );

    // Get the energy grid
    std::vector<double> angular_energy_grid =
        data_container.getElasticAngularEnergyGrid();

    // Get size of paramters
    int size = angular_energy_grid.size();

    // Create the scattering function
    MonteCarlo::AnalogElasticElectronScatteringDistribution::CutoffDistribution
        scattering_function(size);

    for( unsigned n = 0; n < angular_energy_grid.size(); ++n )
    {
    scattering_function[n].first = angular_energy_grid[n];

    // Get the cutoff elastic scattering angles at the energy
    Teuchos::Array<double> angles(
        data_container.getCutoffElasticAngles( angular_energy_grid[n] ) );

    // Get the cutoff elastic scatering pdf at the energy
    Teuchos::Array<double> pdf(
        data_container.getCutoffElasticPDF( angular_energy_grid[n] ) );

    scattering_function[n].second.reset(
      new const Utility::TabularDistribution<Utility::LinLin>( angles, pdf ) );
    }

    double atomic_number = data_container.getAtomicNumber();

    // Create cutoff distribution
    Teuchos::RCP<const MonteCarlo::AnalogElasticElectronScatteringDistribution>
        analog_elastic_distribution(
            new MonteCarlo::AnalogElasticElectronScatteringDistribution(
                scattering_function,
                atomic_number ) );

    Teuchos::ArrayRCP<double> energy_grid;
    energy_grid.assign(
        data_container.getElectronEnergyGrid().begin(),
        data_container.getElectronEnergyGrid().end() );

    Teuchos::ArrayRCP<double> cutoff_cross_section;
    cutoff_cross_section.assign(
        data_container.getCutoffElasticCrossSection().begin(),
        data_container.getCutoffElasticCrossSection().end() );

    Teuchos::ArrayRCP<double> sr_cross_section;
    sr_cross_section.assign(
        data_container.getScreenedRutherfordElasticCrossSection().begin(),
        data_container.getScreenedRutherfordElasticCrossSection().end() );

    unsigned cutoff_threshold_index(
        data_container.getCutoffElasticCrossSectionThresholdEnergyIndex() );

    unsigned sr_threshold_index(
        data_container.getScreenedRutherfordElasticCrossSectionThresholdEnergyIndex() );

    // Create the reaction
    analog_elastic_reaction.reset(
      new MonteCarlo::AnalogElasticElectroatomicReaction<Utility::LinLin>(
                energy_grid,
                cutoff_cross_section,
                sr_cross_section,
                cutoff_threshold_index,
                sr_threshold_index,
                analog_elastic_distribution ) );
  }

  // Initialize the random number generator
  Utility::RandomNumberGenerator::createStreams();

  // Run the unit tests
  Teuchos::GlobalMPISession mpiSession( &argc, &argv );

  const bool success = Teuchos::UnitTestRepository::runUnitTests( *out );

  if (success)
    *out << "\nEnd Result: TEST PASSED" << std::endl;
  else
    *out << "\nEnd Result: TEST FAILED" << std::endl;

  clp.printFinalTimerSummary(out.ptr());

  return (success ? 0 : 1);
}

//---------------------------------------------------------------------------//
// end tstAnalogElasticElectroatomicReaction.cpp
//---------------------------------------------------------------------------//