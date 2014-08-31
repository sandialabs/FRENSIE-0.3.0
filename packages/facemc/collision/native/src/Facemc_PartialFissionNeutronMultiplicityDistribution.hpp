//---------------------------------------------------------------------------//
//!
//! \file   Facemc_PartialFissionNeutronMultiplicityDistribution.hpp
//! \author Alex Robinson
//! \brief  The partial fission neutron multiplicity distribution class decl.
//!
//---------------------------------------------------------------------------//

#ifndef FACEMC_PARTIAL_FISSION_NEUTRON_MULTIPLICITY_DISTRIBUTION_HPP
#define FACEMC_PARTIAL_FISSION_NEUTRON_MULTIPLICITY_DISTRIBUTION_HPP

// FRENSIE Includes
#include "Facemc_FissionNeutronMultiplicityDistribution.hpp"
#include "Utility_OneDDistribution.hpp"

namespace Facemc{

//! The partial fission neutron multiplicity distribution class
template<typename FissionNeutronMultiplicityDistributionPolicy>
class PartialFissionNeutronMultiplicityDistribution : public FissionNeutronMultiplicityDistribution
{

public:

  //! Constructor (prompt and total)
  PartialFissionNeutronMultiplicityDistribution(
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 first_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 second_multiplicity_distribution );

  //! Destructor
  ~PartialFissionNeutronMultiplicityDistribution()
  { /* ... */ }

  //! Return the average number of neutrons emitted
  double getAverageNumberOfEmittedNeutrons( const double energy) const;

  //! Return the average number of prompt neutrons emitted
  double getAverageNumberOfPromptNeutrons( const double energy ) const;

  //! Return the average number of delayed neutrons emitted
  double getAverageNumberOfDelayedNeutrons( const double energy) const;

private:

  // The first multiplicity distribution
  Teuchos::RCP<Utility::OneDDistribution> d_first_multiplicity_distribution;

  // The second multiplicity distribution
  Teuchos::RCP<Utility::OneDDistribution> d_second_multiplicity_distribution;
};

//! The prompt-total policy
struct PromptTotalFissionNeutronMultiplicityPolicy
{
  //! Return the total nu-bar
  static inline double getTotalNuBar( 
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_prompt_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_total_multiplicity_distribution,
				 const double energy )
  {
    return d_total_multiplicity_distribution->evaluate( energy );
  }

  //! Return the prompt nu-bar
  static inline double getPromptNuBar(
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_prompt_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_total_multiplicity_distribution,
				 const double energy )
  {
    return d_prompt_multiplicity_distribution->evaluate( energy );
  }

  //! Return the delayed nu-bar
  static inline double getDelayedNuBar(
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_prompt_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_total_multiplicity_distribution,
				 const double energy )
  {
    return d_total_multiplicity_distribution->evaluate( energy ) - 
      d_prompt_multiplicity_distribution->evaluate( energy );
  }
};

//! The delayed-total policy
struct DelayedTotalFissionNeutronMultiplicityPolicy
{
  //! Return the total nu-bar
  static inline double getTotalNuBar( 
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_delayed_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_total_multiplicity_distribution,
				 const double energy )
  {
    return d_total_multiplicity_distribution->evaluate( energy );
  }

  //! Return the prompt nu-bar
  static inline double getPromptNuBar(
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_delayed_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_total_multiplicity_distribution,
				 const double energy )
  {
    return d_total_multiplicity_distribution->evaluate( energy ) - 
      d_delayed_multiplicity_distribution->evaluate( energy );
  }

  //! Return the delayed nu-bar
  static inline double getDelayedNuBar(
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_delayed_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_total_multiplicity_distribution,
				 const double energy )
  {
    return d_delayed_multiplicity_distribution->evaluate( energy );
  }
};

//! The delayed-prompt policy
struct DelayedPromptFissionNeutronMultiplicityPolicy
{
  //! Return the total nu-bar
  static inline double getTotalNuBar( 
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_delayed_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_prompt_multiplicity_distribution,
				 const double energy )
  {
    return d_delayed_multiplicity_distribution->evaluate( energy ) +
      d_prompt_multiplicity_distribution->evaluate( energy );
  }

  //! Return the prompt nu-bar
  static inline double getPromptNuBar(
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_delayed_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_prompt_multiplicity_distribution,
				 const double energy )
  {
    return d_prompt_multiplicity_distribution->evaluate( energy );
  }

  //! Return the delayed nu-bar
  static inline double getDelayedNuBar(
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_delayed_multiplicity_distribution,
				 const Teuchos::RCP<Utility::OneDDistribution>&
				 d_prompt_multiplicity_distribution,
				 const double energy )
  {
    return d_delayed_multiplicity_distribution->evaluate( energy );
  }
};

} // end Facemc namespace

//---------------------------------------------------------------------------//
// Template Includes
//---------------------------------------------------------------------------//

#include "Facemc_PartialFissionNeutronMultiplicityDistribution_def.hpp"

//---------------------------------------------------------------------------//

#endif // end FACEMC_PARTIAL_FISSION_NEUTRON_MULTIPLICITY_DISTRIBUTION_HPP

//---------------------------------------------------------------------------//
// end Facemc_PartialFissionNeutronMultiplicityDistribution.hpp
//---------------------------------------------------------------------------//