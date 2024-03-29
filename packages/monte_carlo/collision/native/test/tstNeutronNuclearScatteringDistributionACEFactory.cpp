//---------------------------------------------------------------------------//
//!
//! \file   tstNeutronNuclearScatteringDistributionACEFactory.cpp
//! \author Alex Robinson
//! \brief  Neutron-neutron scattering distribution factory unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <string>
#include <iostream>

// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_RCP.hpp>

// FRENSIE Includes
#include "MonteCarlo_NeutronNuclearScatteringDistributionACEFactory.hpp"
#include "MonteCarlo_NuclearReactionType.hpp"
#include "Data_ACEFileHandler.hpp"
#include "Data_XSSNeutronDataExtractor.hpp"

//---------------------------------------------------------------------------//
// Testing Structs.
//---------------------------------------------------------------------------//
class TestNeutronNuclearScatteringDistributionACEFactory : public MonteCarlo::NeutronNuclearScatteringDistributionACEFactory
{
public:
  TestNeutronNuclearScatteringDistributionACEFactory(
                         const std::string& table_name,
			 const double atomic_weight_ratio,
			 const Data::XSSNeutronDataExtractor raw_nuclide_data )
    : MonteCarlo::NeutronNuclearScatteringDistributionACEFactory( table_name,
                                                           atomic_weight_ratio,
							   raw_nuclide_data )
  { /* ... */ }
  
  ~TestNeutronNuclearScatteringDistributionACEFactory()
  { /* ... */ }

  using MonteCarlo::NeutronNuclearScatteringDistributionACEFactory::getReactionOrdering;
  using MonteCarlo::NeutronNuclearScatteringDistributionACEFactory::getReactionCMScattering;
  using MonteCarlo::NeutronNuclearScatteringDistributionACEFactory::getReactionsWithIsotropicScatteringOnly;
  using MonteCarlo::NeutronNuclearScatteringDistributionACEFactory::getReactionsWithCoupledEnergyAngleDist;
  using MonteCarlo::NeutronNuclearScatteringDistributionACEFactory::getReactionAngularDist;
  using MonteCarlo::NeutronNuclearScatteringDistributionACEFactory::getReactionAngularDistStartIndex;
  using MonteCarlo::NeutronNuclearScatteringDistributionACEFactory::getReactionEnergyDist;
  using MonteCarlo::NeutronNuclearScatteringDistributionACEFactory::getReactionEnergyDistStartIndex;
};
//---------------------------------------------------------------------------//
// Testing Variables.
//---------------------------------------------------------------------------//
std::string test_basic_h1_ace_file_name;
std::string test_basic_h1_ace_table_name;

Teuchos::RCP<Data::ACEFileHandler> ace_file_handler_h1;
Teuchos::RCP<Data::XSSNeutronDataExtractor> xss_data_extractor_h1;
Teuchos::RCP<TestNeutronNuclearScatteringDistributionACEFactory> neutron_distribution_factory_h1;

std::string test_basic_o16_ace_file_name;
std::string test_basic_o16_ace_table_name;

Teuchos::RCP<Data::ACEFileHandler> ace_file_handler_o16;
Teuchos::RCP<Data::XSSNeutronDataExtractor> xss_data_extractor_o16;
Teuchos::RCP<TestNeutronNuclearScatteringDistributionACEFactory> neutron_distribution_factory_o16;
//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionOrdering_h1 )
{


  ace_file_handler_h1.reset(new Data::ACEFileHandler( test_basic_h1_ace_file_name,
                                                   test_basic_h1_ace_table_name,
                                                   1u ) );

  xss_data_extractor_h1.reset(
   new Data::XSSNeutronDataExtractor( ace_file_handler_h1->getTableNXSArray(),
                                      ace_file_handler_h1->getTableJXSArray(),
                                      ace_file_handler_h1->getTableXSSArray()));
   
//  MonteCarlo::NeutronNuclearScatteringDistributionACEFactory
  neutron_distribution_factory_h1.reset( 
    new TestNeutronNuclearScatteringDistributionACEFactory(
                             test_basic_h1_ace_table_name,
                             ace_file_handler_h1->getTableAtomicWeightRatio(),
			     *xss_data_extractor_h1 ) );

  TEST_COMPARE( neutron_distribution_factory_h1->getReactionOrdering().size() ,==, 0);
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionOrdering_o16 )
{


  ace_file_handler_o16.reset(new Data::ACEFileHandler( test_basic_o16_ace_file_name,
                                                       test_basic_o16_ace_table_name,
                                                       1u ) );

  xss_data_extractor_o16.reset(
   new Data::XSSNeutronDataExtractor( ace_file_handler_o16->getTableNXSArray(),
                                      ace_file_handler_o16->getTableJXSArray(),
                                      ace_file_handler_o16->getTableXSSArray()));
   
//  MonteCarlo::NeutronNuclearScatteringDistributionACEFactory
  neutron_distribution_factory_o16.reset( 
    new TestNeutronNuclearScatteringDistributionACEFactory(
			     test_basic_o16_ace_table_name,
                             ace_file_handler_o16->getTableAtomicWeightRatio(),
			     *xss_data_extractor_o16 ) );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionOrdering().size() ,==, 17);
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionOrdering().find(MonteCarlo::N__ANYTHING_REACTION)->second ,==, 0 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionOrdering().find(MonteCarlo::N__N_CONTINUUM_REACTION)->second ,==, 16 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionCMScattering_h1 )
{
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionCMScattering().size() ,==, 1);
  TEST_ASSERT( neutron_distribution_factory_h1->getReactionCMScattering().find(MonteCarlo::N__N_ELASTIC_REACTION)->second );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionCMScattering_o16 )
{
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionCMScattering().size() ,==, 18);
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionCMScattering().find(MonteCarlo::N__N_ELASTIC_REACTION)->second );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionCMScattering().find(MonteCarlo::N__ANYTHING_REACTION)->second );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionCMScattering().find(MonteCarlo::N__N_CONTINUUM_REACTION)->second );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionsWithIsotropicScatteringOnly_h1 )
{
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionsWithIsotropicScatteringOnly().size() ,==, 0 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionsWithIsotropicScatteringOnly_o16 )
{
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionsWithIsotropicScatteringOnly().size() ,==, 2 );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithIsotropicScatteringOnly().find(MonteCarlo::N__N_EXCITED_STATE_6_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithIsotropicScatteringOnly().end() )
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithIsotropicScatteringOnly().find(MonteCarlo::N__N_EXCITED_STATE_7_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithIsotropicScatteringOnly().end() )
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionWithCoupledEnergyAngleDist_h1 )
{
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionsWithCoupledEnergyAngleDist().size() ,==, 0 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionWithCoupledEnergyAngleDist_o16 )
{
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().size() ,==, 10 );

  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__ANYTHING_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__2N_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__N_ALPHA_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__N_3ALPHA_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__N_P_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__N_D_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__2N_P_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__N_2P_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__N_P_ALPHA_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().find(MonteCarlo::N__N_CONTINUUM_REACTION) !=
               neutron_distribution_factory_o16->getReactionsWithCoupledEnergyAngleDist().end() );

}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionAngularDist_h1 )
{
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionAngularDist().size() ,==, 1 );
  TEST_ASSERT( neutron_distribution_factory_h1->getReactionAngularDist().find(MonteCarlo::N__N_ELASTIC_REACTION) !=
               neutron_distribution_factory_h1->getReactionAngularDist().end() );
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionAngularDist().find(MonteCarlo::N__N_ELASTIC_REACTION)->second.size() ,==, 1291 );
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionAngularDist().find(MonteCarlo::N__N_ELASTIC_REACTION)->second.front() ,==, 96 );
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionAngularDist().find(MonteCarlo::N__N_ELASTIC_REACTION)->second.back() ,==, 1 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionAngularDist_o16 )
{
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().size() ,==, 6 );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_ELASTIC_REACTION) !=
               neutron_distribution_factory_o16->getReactionAngularDist().end() );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION) !=
               neutron_distribution_factory_o16->getReactionAngularDist().end() );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION) !=
               neutron_distribution_factory_o16->getReactionAngularDist().end() );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION) !=
               neutron_distribution_factory_o16->getReactionAngularDist().end() );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION) !=
               neutron_distribution_factory_o16->getReactionAngularDist().end() );
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION) !=
               neutron_distribution_factory_o16->getReactionAngularDist().end() );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_ELASTIC_REACTION)->second.size() ,==, 101748 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_ELASTIC_REACTION)->second.front() ,==, 1175 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_ELASTIC_REACTION)->second.back() ,==, 1 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION)->second.size() ,==, 1475 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION)->second.front() ,==, 40 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION)->second.back() ,==, 1 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION)->second.size() ,==, 1475 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION)->second.front() ,==, 40 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION)->second.back() ,==, 1 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION)->second.size() ,==, 1097 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION)->second.front() ,==, 31 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION)->second.back() ,==, 1 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION)->second.size() ,==, 1084 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION)->second.front() ,==, 30 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION)->second.back() ,==, 1 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION)->second.size() ,==, 1098 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION)->second.front() ,==, 23 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDist().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION)->second.back() ,==, 1 );

}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionAngularDistStartIndex_h1 )
{
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionAngularDistStartIndex().size() ,==, 1 );
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionAngularDistStartIndex().find(MonteCarlo::N__N_ELASTIC_REACTION)->second ,==, 0 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionAngularDistStartIndex_o16 )
{
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDistStartIndex().size() ,==, 6 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDistStartIndex().find(MonteCarlo::N__N_ELASTIC_REACTION)->second ,==, 0 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION)->second ,==, 101748 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION)->second ,==, 103223 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION)->second ,==, 104698 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION)->second ,==, 105795 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionAngularDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION)->second ,==, 106879 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionEnergyDist_h1 )
{
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionEnergyDist().size() ,==, 0 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionEnergyDist_o16 )
{
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().size() ,==, 17 );

  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__ANYTHING_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__2N_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_ALPHA_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_3ALPHA_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_P_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_D_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__2N_P_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_2P_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_P_ALPHA_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_6_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_7_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() ); 
  TEST_ASSERT( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_CONTINUUM_REACTION) !=
               neutron_distribution_factory_o16->getReactionEnergyDist().end() );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__ANYTHING_REACTION)->second.size() ,==, 5438 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__ANYTHING_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__ANYTHING_REACTION)->second.back(), 9.333427, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__2N_REACTION)->second.size() ,==, 3861 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__2N_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__2N_REACTION)->second.back(), 0.7874268, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_ALPHA_REACTION)->second.size() ,==, 10597 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_ALPHA_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_ALPHA_REACTION)->second.back(), 1.052822, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_3ALPHA_REACTION)->second.size() ,==, 3692 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_3ALPHA_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_3ALPHA_REACTION)->second.back(), 0.7801906, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_D_REACTION)->second.size() ,==, 1718 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_D_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_D_REACTION)->second.back(), 0.6012564 , 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__2N_P_REACTION)->second.size() ,==, 966 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__2N_P_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__2N_P_REACTION)->second.back(), 0.5165388, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_2P_REACTION)->second.size() ,==, 971 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_2P_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_2P_REACTION)->second.back(), 0.5165388, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_P_ALPHA_REACTION)->second.size() ,==, 916 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_P_ALPHA_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_P_ALPHA_REACTION)->second.back(), 0.502482, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION)->second.size() ,==, 11 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION)->second.back(), 0.8848775, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION)->second.size() ,==, 11 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION)->second.back(), 0.8848775, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION)->second.size() ,==, 11 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION)->second.back(), 0.8848775, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION)->second.size() ,==, 11 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION)->second.back(), 0.8848775, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION)->second.size() ,==, 11 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION)->second.back(), 0.8848775, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_6_REACTION)->second.size() ,==, 11 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_6_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_6_REACTION)->second.back(), 0.8848775, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_7_REACTION)->second.size() ,==, 11 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_7_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_EXCITED_STATE_7_REACTION)->second.back(), 0.8848775, 1e-15 );

  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_CONTINUUM_REACTION)->second.size() ,==, 8892 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_CONTINUUM_REACTION)->second.front() ,==, 0 );
  TEST_FLOATING_EQUALITY( neutron_distribution_factory_o16->getReactionEnergyDist().find(MonteCarlo::N__N_CONTINUUM_REACTION)->second.back(), 1.007868, 1e-15 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionEnergyDistStartIndex_h1 )
{
  TEST_COMPARE( neutron_distribution_factory_h1->getReactionEnergyDistStartIndex().size() ,==, 0 );
}

//---------------------------------------------------------------------------//
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   getReactionEnergyDistStartIndex_o16 )
{
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__ANYTHING_REACTION)->second ,==, 34 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__2N_REACTION)->second ,==, 5472 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_ALPHA_REACTION)->second ,==, 9333 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_3ALPHA_REACTION)->second ,==, 19930 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_P_REACTION)->second ,==, 23622 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_D_REACTION)->second ,==, 29905 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__2N_P_REACTION)->second ,==, 31623 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_2P_REACTION)->second ,==, 32589 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_P_ALPHA_REACTION)->second ,==, 33560 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_1_REACTION)->second ,==, 34476 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_2_REACTION)->second ,==, 34487 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_3_REACTION)->second ,==, 34498 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_4_REACTION)->second ,==, 34509 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_5_REACTION)->second ,==, 34520 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_6_REACTION)->second ,==, 34531 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_EXCITED_STATE_7_REACTION)->second ,==, 34542 );
  TEST_COMPARE( neutron_distribution_factory_o16->getReactionEnergyDistStartIndex().find(MonteCarlo::N__N_CONTINUUM_REACTION)->second ,==, 34553 );
}

//---------------------------------------------------------------------------//
// Check that an elastic scattering distribution can be constructed
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   createScatteringDistribution_elastic )
{
  Teuchos::RCP<MonteCarlo::NuclearScatteringDistribution<MonteCarlo::NeutronState,MonteCarlo::NeutronState> > scattering_dist;

  // Sample the hydrogen elastic scattering distribution
  neutron_distribution_factory_h1->createScatteringDistribution(
						 MonteCarlo::N__N_ELASTIC_REACTION,
						 scattering_dist );

  MonteCarlo::NeutronState neutron( 0ull );
  neutron.setDirection( 0.0, 0.0, 1.0 );
  neutron.setEnergy( 1.0 );

  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
				   ace_file_handler_h1->getTableTemperature() );
  
  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
  				   ace_file_handler_h1->getTableTemperature() );

  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
  				   ace_file_handler_h1->getTableTemperature() );

  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
  				   ace_file_handler_h1->getTableTemperature() );

  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
  				   ace_file_handler_h1->getTableTemperature() );

  std::cout << neutron << std::endl;

  // Create a the elastic scattering distribution for oxygen
  neutron_distribution_factory_o16->createScatteringDistribution(
						 MonteCarlo::N__N_ELASTIC_REACTION,
						 scattering_dist );

  neutron.setDirection( 0.0, 0.0, 1.0 );
  neutron.setEnergy( 1.0 );

  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
				   ace_file_handler_o16->getTableTemperature() );
  
  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
  				   ace_file_handler_o16->getTableTemperature() );

  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
  				   ace_file_handler_o16->getTableTemperature() );

  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
  				   ace_file_handler_o16->getTableTemperature() );

  std::cout << neutron << std::endl;
  scattering_dist->scatterParticle( neutron, 
  				   ace_file_handler_o16->getTableTemperature() );

  std::cout << neutron << std::endl;
}

//---------------------------------------------------------------------------//
// Check that all other scattering distributions can be constructed
TEUCHOS_UNIT_TEST( NeutronNuclearScatteringDistributionACEFactory, 
		   createScatteringDistribution_all )
{
  Teuchos::RCP<MonteCarlo::NuclearScatteringDistribution<MonteCarlo::NeutronState,MonteCarlo::NeutronState> > scattering_dist;
  
  neutron_distribution_factory_o16->createScatteringDistribution( 
					       MonteCarlo::N__ANYTHING_REACTION,
					       scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					      MonteCarlo::N__2N_REACTION,
					      scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					       MonteCarlo::N__N_ALPHA_REACTION,
					       scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					       MonteCarlo::N__N_3ALPHA_REACTION,
					       scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					       MonteCarlo::N__N_D_REACTION,
					       scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					       MonteCarlo::N__2N_P_REACTION,
					       scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					       MonteCarlo::N__N_2P_REACTION,
					       scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					       MonteCarlo::N__N_P_ALPHA_REACTION,
					       scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					 MonteCarlo::N__N_EXCITED_STATE_1_REACTION,
					 scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					 MonteCarlo::N__N_EXCITED_STATE_2_REACTION,
					 scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					 MonteCarlo::N__N_EXCITED_STATE_3_REACTION,
					 scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					 MonteCarlo::N__N_EXCITED_STATE_4_REACTION,
					 scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					 MonteCarlo::N__N_EXCITED_STATE_5_REACTION,
					 scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					 MonteCarlo::N__N_EXCITED_STATE_6_REACTION,
					 scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					 MonteCarlo::N__N_EXCITED_STATE_7_REACTION,
					 scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );

  neutron_distribution_factory_o16->createScatteringDistribution( 
					      MonteCarlo::N__N_CONTINUUM_REACTION,
					      scattering_dist );
						
  TEST_ASSERT( !scattering_dist.is_null() );



//  boost::unordered_map<MonteCarlo::NuclearReactionType,unsigned>::const_iterator i;
//
//  for(i = neutron_distribution_factory_o16->getReactionOrdering().begin(); i != neutron_distribution_factory_o16->getReactionOrdering().end(); ++i)
//  {
//    std::cout << "MT number " << i->first << std::endl;
//
//    Teuchos::RCP<MonteCarlo::NeutronNuclearScatteringDistribution> dist;
//
//    neutron_distribution_factory_o16->createScatteringDistribution( i->first, dist );
//
//  }


}

//---------------------------------------------------------------------------//
// Custom main function
//---------------------------------------------------------------------------//
int main( int argc, char** argv )
{
  Teuchos::CommandLineProcessor& clp = Teuchos::UnitTestRepository::getCLP();

  clp.setOption( "test_basic_h1_ace_file",
		 &test_basic_h1_ace_file_name,
		 "Test basic H1 ACE file name" );
  clp.setOption( "test_basic_h1_ace_table",
		 &test_basic_h1_ace_table_name,
		 "Test basic H1 ACE table name in basic ACE file" );

  clp.setOption( "test_basic_o16_ace_file",
		 &test_basic_o16_ace_file_name,
		 "Test basic O16 ACE file name" );
  clp.setOption( "test_basic_o16_ace_table",
		 &test_basic_o16_ace_table_name,
		 "Test basic O16 ACE table name in basic ACE file" );

  // Initialize the random number generator
  Utility::RandomNumberGenerator::createStreams();

  Teuchos::GlobalMPISession mpiSession( &argc, &argv );
  return Teuchos::UnitTestRepository::runUnitTestsFromMain( argc, argv );
}

//---------------------------------------------------------------------------//
// end tstNeutronNuclearScatteringDistributionACEFactory.cpp
//---------------------------------------------------------------------------//
