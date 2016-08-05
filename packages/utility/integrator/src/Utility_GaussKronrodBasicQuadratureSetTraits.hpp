//---------------------------------------------------------------------------//
//!
//! \file   Utility_GaussKronrodBasicQuadratureSetTraits.hpp
//! \author Luke Kersting
//! \brief  Gauss-Kronrod quadrature set traits
//!
//---------------------------------------------------------------------------//

#ifndef UTILITY_GAUSS_KRONROD_BASIC_QUADRATURE_SET_TRAITS_HPP
#define UTILITY_GAUSS_KRONROD_BASIC_QUADRATURE_SET_TRAITS_HPP

// FRENSIE Includes
#include "Utility_GaussKronrodBasicQuadratureSetTraitsDecl.hpp"

namespace Utility{

  // Gauss-Kronrod quadrature set traits 15 point rule
  template<>
  struct GaussKronrodBasicQuadratureSetTraits<15>
  {
    // Valid rule
    static const bool valid_rule = true;

    // Gauss quadrature weights 
    static const Teuchos::Array<double> gauss_weights;
    
    // Kronrad quadrature weights 
    static const Teuchos::Array<double> kronrod_weights;

    // Kronrad quadrature abscissae
    static const Teuchos::Array<double> kronrod_abscissae;

    private:

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeGaussWeights();

    // Initialize the kronrod weight array
    static Teuchos::Array<double> initializeKronrodWeights();

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeKronrodAbscissae();
  };

  // Gauss-Kronrod quadrature set traits 21 point rule
  template<>
  struct GaussKronrodBasicQuadratureSetTraits<21>
  {
    // Valid rule
    static const bool valid_rule = true;

    // Gauss quadrature weights 
    static const Teuchos::Array<double> gauss_weights;
    
    // Kronrad quadrature weights 
    static const Teuchos::Array<double> kronrod_weights;

    // Kronrad quadrature abscissae
    static const Teuchos::Array<double> kronrod_abscissae;

    private:

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeGaussWeights();

    // Initialize the kronrod weight array
    static Teuchos::Array<double> initializeKronrodWeights();

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeKronrodAbscissae();
  };

  // Gauss-Kronrod quadrature set traits 31 point rule
  template<>
  struct GaussKronrodBasicQuadratureSetTraits<31>
  {
    // Valid rule
    static const bool valid_rule = true;

    // Gauss quadrature weights 
    static const Teuchos::Array<double> gauss_weights;
    
    // Kronrad quadrature weights 
    static const Teuchos::Array<double> kronrod_weights;

    // Kronrad quadrature abscissae
    static const Teuchos::Array<double> kronrod_abscissae;

    private:

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeGaussWeights();

    // Initialize the kronrod weight array
    static Teuchos::Array<double> initializeKronrodWeights();

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeKronrodAbscissae();
  };

  // Gauss-Kronrod quadrature set traits 41 point rule
  template<>
  struct GaussKronrodBasicQuadratureSetTraits<41>
  {
    // Valid rule
    static const bool valid_rule = true;

    // Gauss quadrature weights 
    static const Teuchos::Array<double> gauss_weights;
    
    // Kronrad quadrature weights 
    static const Teuchos::Array<double> kronrod_weights;

    // Kronrad quadrature abscissae
    static const Teuchos::Array<double> kronrod_abscissae;

    private:

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeGaussWeights();

    // Initialize the kronrod weight array
    static Teuchos::Array<double> initializeKronrodWeights();

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeKronrodAbscissae();
  };

  // Gauss-Kronrod quadrature set traits 51 point rule
  template<>
  struct GaussKronrodBasicQuadratureSetTraits<51>
  {
    // Valid rule
    static const bool valid_rule = true;

    // Gauss quadrature weights 
    static const Teuchos::Array<double> gauss_weights;
    
    // Kronrad quadrature weights 
    static const Teuchos::Array<double> kronrod_weights;

    // Kronrad quadrature abscissae
    static const Teuchos::Array<double> kronrod_abscissae;

    private:

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeGaussWeights();

    // Initialize the kronrod weight array
    static Teuchos::Array<double> initializeKronrodWeights();

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeKronrodAbscissae();
  };

  // Gauss-Kronrod quadrature set traits 61 point rule
  template<>
  struct GaussKronrodBasicQuadratureSetTraits<61>
  {
    // Valid rule
    static const bool valid_rule = true;

    // Gauss quadrature weights 
    static const Teuchos::Array<double> gauss_weights;
    
    // Kronrad quadrature weights 
    static const Teuchos::Array<double> kronrod_weights;

    // Kronrad quadrature abscissae
    static const Teuchos::Array<double> kronrod_abscissae;

    private:

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeGaussWeights();

    // Initialize the kronrod weight array
    static Teuchos::Array<double> initializeKronrodWeights();

    // Initialize the gauss weight array
    static Teuchos::Array<double> initializeKronrodAbscissae();
  };

} // end Utility namespace

#endif // end UTILITY_GAUSS_KRONROD_BASIC_QUADRATURE_SET_TRAITS_HPP

//---------------------------------------------------------------------------//
// end Utility_GaussKronrodBasicQuadratureSetTraits.hpp
//---------------------------------------------------------------------------//

