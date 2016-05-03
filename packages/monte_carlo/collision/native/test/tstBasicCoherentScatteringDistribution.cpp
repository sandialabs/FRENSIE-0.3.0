//---------------------------------------------------------------------------//
//!
//! \file   tstBasicCoherentScatteringDistribution.cpp
//! \author Alex Robinson
//! \brief  Basic coherent scattering distribution unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <limits>
  
// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_RCP.hpp>
#include <Teuchos_VerboseObject.hpp>
#include <Teuchos_Array.hpp>

// FRENSIE Includes
#include "MonteCarlo_UnitTestHarnessExtensions.hpp"
#include "MonteCarlo_BasicCoherentScatteringDistribution.hpp"
#include "Data_ACEFileHandler.hpp"
#include "Data_XSSEPRDataExtractor.hpp"
#include "Utility_DiscreteDistribution.hpp"
#include "Utility_TabularDistribution.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_DirectionHelpers.hpp"

//---------------------------------------------------------------------------//
// Testing Variable
//---------------------------------------------------------------------------//

Teuchos::RCP<MonteCarlo::PhotonScatteringDistribution>
  distribution;

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that the distribution can be evaluated
TEUCHOS_UNIT_TEST( BasicCoherentScatteringDistribution, evaluate )
{
  double dist_value = distribution->evaluate( 0.1, 1.0 );

  TEST_FLOATING_EQUALITY( dist_value, 3.354834939813898e3, 1e-15 );
  
  dist_value = distribution->evaluate( 0.1, 0.0 );
  
  TEST_FLOATING_EQUALITY( dist_value, 4.17273487105470142, 1e-15 );
  
  dist_value = distribution->evaluate( 0.1, -1.0 );
  
  TEST_FLOATING_EQUALITY( dist_value, 3.59244179705391486, 1e-15 );

  dist_value = distribution->evaluate( 1.0, 1.0 );
  
  TEST_FLOATING_EQUALITY( dist_value, 3354.83493981389802, 1e-15 );

  dist_value = distribution->evaluate( 1.0, 0.0 );
  
  TEST_FLOATING_EQUALITY( dist_value, 0.00346135656968035591, 1e-15 );

  dist_value = distribution->evaluate( 1.0, -1.0 );
  
  TEST_FLOATING_EQUALITY( dist_value, 0.00114806389085036088, 1e-15 );
}

//---------------------------------------------------------------------------//
// Check that the distribution pdf can be evaluated
TEUCHOS_UNIT_TEST( BasicCoherentScatteringDistribution, evaluatePDF )
{
  double pdf_value = distribution->evaluatePDF( 0.1, 1.0 );
  
  TEST_FLOATING_EQUALITY( pdf_value, 49.4688663359142353, 1e-15 );

  pdf_value = distribution->evaluatePDF( 0.1, 0.0 );
  
  TEST_FLOATING_EQUALITY( pdf_value, 0.0615292457884274099, 1e-15 );

  pdf_value = distribution->evaluatePDF( 0.1, -1.0 );
  
  TEST_FLOATING_EQUALITY( pdf_value, 0.0529725087124166966, 1e-15 );

  pdf_value = distribution->evaluatePDF( 1.0, 1.0 );
  
  TEST_FLOATING_EQUALITY( pdf_value, 3673.12567843006855, 1e-15 );

  pdf_value = distribution->evaluatePDF( 1.0, 0.0 );
  
  TEST_FLOATING_EQUALITY( pdf_value, 0.00378975357249642037, 1e-15 );

  pdf_value = distribution->evaluatePDF( 1.0, -1.0 );
  
  TEST_FLOATING_EQUALITY( pdf_value, 0.00125698671726446362, 1e-15 );
}

//---------------------------------------------------------------------------//
// Check that the integrated cross section can be evaluated
TEUCHOS_UNIT_TEST( BasicCoherentScatteringDistribution, 
		   evaluateIntegratedCrossSection )
{
  double cross_section = 
    distribution->evaluateIntegratedCrossSection( 0.1, 1e-3 );
  
  TEST_FLOATING_EQUALITY( cross_section, 6.78170976677162821e1, 1e-15 );

  cross_section = 
    distribution->evaluateIntegratedCrossSection( 1.0, 1e-3 );
  
  TEST_FLOATING_EQUALITY( cross_section, 9.13346080019725521e-1, 1e-15 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be sampled from
TEUCHOS_UNIT_TEST( BasicCoherentScatteringDistribution, sample )
{
  double outgoing_energy, scattering_angle_cosine;
  
  // Set up the random number stream
  std::vector<double> fake_stream( 9 );
  fake_stream[0] = 0.75;
  fake_stream[1] = 0.5;
  fake_stream[2] = 0.003; // reject
  fake_stream[3] = 0.7;
  fake_stream[4] = 0.8;
  fake_stream[5] = 0.006; // end first branch
  fake_stream[6] = 0.751;
  fake_stream[7] = 0.1;
  fake_stream[8] = 0.001; // end second branch

  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  // Sample from the 1st branch
  distribution->sample( 0.1,
			outgoing_energy,
			scattering_angle_cosine );
  
  TEST_EQUALITY_CONST( outgoing_energy, 0.1 );
  TEST_FLOATING_EQUALITY( scattering_angle_cosine, 0.6, 1e-15 );

  // Sample from the second branch
  distribution->sample( 0.1,
			outgoing_energy,
			scattering_angle_cosine );

  TEST_EQUALITY_CONST( outgoing_energy, 0.1 );
  TEST_FLOATING_EQUALITY( scattering_angle_cosine, -0.9283177667225558, 1e-15 );

  Utility::RandomNumberGenerator::unsetFakeStream();
}

//---------------------------------------------------------------------------//
// Check that the distribution can be sampled from
TEUCHOS_UNIT_TEST( BasicCoherentScatteringDistribution, 
		   sampleAndRecordTrials )
{
  double outgoing_energy, scattering_angle_cosine;
  unsigned trials = 0;
  
  // Set up the random number stream
  std::vector<double> fake_stream( 9 );
  fake_stream[0] = 0.75;
  fake_stream[1] = 0.5;
  fake_stream[2] = 0.003; // reject
  fake_stream[3] = 0.7;
  fake_stream[4] = 0.8;
  fake_stream[5] = 0.006; // end first branch
  fake_stream[6] = 0.751;
  fake_stream[7] = 0.1;
  fake_stream[8] = 0.001; // end second branch

  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  // Sample from the 1st branch
  distribution->sampleAndRecordTrials( 0.1,
				       outgoing_energy,
				       scattering_angle_cosine,
				       trials );
  
  TEST_EQUALITY_CONST( outgoing_energy, 0.1 );
  TEST_FLOATING_EQUALITY( scattering_angle_cosine, 0.6, 1e-15 );
  TEST_EQUALITY_CONST( 1.0/trials, 0.5 );

  // Sample from the second branch
  distribution->sampleAndRecordTrials( 0.1,
				       outgoing_energy,
				       scattering_angle_cosine,
				       trials );

  TEST_EQUALITY_CONST( outgoing_energy, 0.1 );
  TEST_FLOATING_EQUALITY( scattering_angle_cosine, -0.9283177667225558, 1e-15 )
  TEST_EQUALITY_CONST( 2.0/trials, 2.0/3.0 );

  Utility::RandomNumberGenerator::unsetFakeStream();
}

//---------------------------------------------------------------------------//
// Check that a photon can be scattered coherently
TEUCHOS_UNIT_TEST( BasicCoherentScatteringDistribution, 
		   scatterPhoton )
{
  MonteCarlo::ParticleBank bank;
  
  MonteCarlo::PhotonState photon( 0 );
  photon.setEnergy( 0.1 );
  photon.setDirection( 0.0, 0.0, 1.0 );
  
  Data::SubshellType shell_of_interaction;

  // Set up the random number stream
  std::vector<double> fake_stream( 11 );
  fake_stream[0] = 0.75;
  fake_stream[1] = 0.5;
  fake_stream[2] = 0.003; // reject
  fake_stream[3] = 0.7;
  fake_stream[4] = 0.8;
  fake_stream[5] = 0.006; // end first branch
  fake_stream[6] = 0.0;
  fake_stream[7] = 0.751;
  fake_stream[8] = 0.1;
  fake_stream[9] = 0.001; // end second branch
  fake_stream[10] = 0.0;

  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  // Sample from the 1st branch
  distribution->scatterPhoton( photon,
			       bank,
			       shell_of_interaction );
  
  TEST_FLOATING_EQUALITY( photon.getEnergy(), 0.1, 1e-15  );
  TEST_FLOATING_EQUALITY( photon.getZDirection(), 0.6, 1e-15 );
  TEST_EQUALITY_CONST( shell_of_interaction, Data::UNKNOWN_SUBSHELL );

  photon.setDirection( 0.0, 0.0, 1.0 );

  // Sample from the second branch
  distribution->scatterPhoton( photon,
			       bank,
			       shell_of_interaction );

  TEST_FLOATING_EQUALITY( photon.getEnergy(), 0.1, 1e-15  );
  TEST_FLOATING_EQUALITY( photon.getZDirection(), -0.9283177667225558, 1e-15 );
  TEST_EQUALITY_CONST( shell_of_interaction, Data::UNKNOWN_SUBSHELL );

  Utility::RandomNumberGenerator::unsetFakeStream();
}

//---------------------------------------------------------------------------//
// Custom main function
//---------------------------------------------------------------------------//
int main( int argc, char** argv )
{
  std::string test_ace_file_name, test_ace_table_name;
  int atomic_number;
  
  Teuchos::CommandLineProcessor& clp = Teuchos::UnitTestRepository::getCLP();
  
  clp.setOption( "test_ace_file",
		 &test_ace_file_name,
		 "Test ACE file name" );
  clp.setOption( "test_ace_table",
		 &test_ace_table_name,
		 "Test ACE table name" );

  const Teuchos::RCP<Teuchos::FancyOStream> out = 
    Teuchos::VerboseObjectBase::getDefaultOStream();

  Teuchos::CommandLineProcessor::EParseCommandLineReturn parse_return = 
    clp.parse(argc,argv);

  if ( parse_return != Teuchos::CommandLineProcessor::PARSE_SUCCESSFUL ) {
    *out << "\nEnd Result: TEST FAILED" << std::endl;
    return parse_return;
  }

  {
    // Create a file handler and data extractor
    Teuchos::RCP<Data::ACEFileHandler> ace_file_handler( 
				 new Data::ACEFileHandler( test_ace_file_name,
							   test_ace_table_name,
							   1u ) );
    Teuchos::RCP<Data::XSSEPRDataExtractor> xss_data_extractor(
                            new Data::XSSEPRDataExtractor( 
				      ace_file_handler->getTableNXSArray(),
				      ace_file_handler->getTableJXSArray(),
				      ace_file_handler->getTableXSSArray() ) );

    // Extract the form factor
    Teuchos::ArrayView<const double> jcohe_block = 
      xss_data_extractor->extractJCOHEBlock();

    unsigned form_factor_size = jcohe_block.size()/3;
    
    Teuchos::Array<double> recoil_momentum_squared(
					  jcohe_block( 0, form_factor_size ) );

    Teuchos::Array<double> form_factor_squared(
			 jcohe_block( 2*form_factor_size, form_factor_size ) );

    for( unsigned i = 0; i < form_factor_size; ++i )
    {
      recoil_momentum_squared[i] *=
	recoil_momentum_squared[i]*1e16; // conver from A^-2 to cm^-2
      
      form_factor_squared[i] *= form_factor_squared[i];
    }
  
    Teuchos::RCP<Utility::TabularOneDDistribution> form_factor_function_squared(
			    new Utility::TabularDistribution<Utility::LinLin>( 
						       recoil_momentum_squared,
				                       form_factor_squared ) );

    // Create the scattering distribution
    distribution.reset( 
		new MonteCarlo::BasicCoherentScatteringDistribution( 
					      form_factor_function_squared ) );
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
// end tstBasicCoherentScatteringDistribution.cpp
//---------------------------------------------------------------------------//
