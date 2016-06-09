//---------------------------------------------------------------------------//
//!
//! \file   Utility_OneDDistributionHelpers.i
//! \author Alex Robinson
//! \brief  The distribution helper macros
//!
//---------------------------------------------------------------------------//

%{
#include "PyFrensie_DistributionHelpers.hpp"
%}

// Include std::string support
%include <std_string.i>

// Import the PyFrensie utility
%import "PyFrensie_DistributionHelpers.hpp"

//---------------------------------------------------------------------------//
// Helper macros for getting the distribution name
//---------------------------------------------------------------------------//
// Get the name of a distribution that has 3 template parameters
// It must be included in both the interface file and the wrapper file since
// the preprocessor will have to parse it separately in both files
%{
#define _DIST_NAME_3_ARGS( dist_base_name, arg_1, arg_2, arg_3 ) Utility::UnitAware ## dist_base_name<arg_1,arg_2,arg_3>
%}
#define _DIST_NAME_3_ARGS( dist_base_name, arg_1, arg_2, arg_3 ) Utility::UnitAware ## dist_base_name<arg_1,arg_2,arg_3>

// Get the name of a distribution that has 2 template parameters
#define _DIST_NAME_2_ARGS( dist_base_name, arg_1, arg_2 ) Utility::UnitAware ## dist_base_name<arg_1,arg_2>

// Use Preprocessor Metaprogramming to get the distribution name
#define _GET_DIST_NAME_MACRO( _1, _2, _3, _4, NAME, ... ) NAME
#define DIST_NAME( ... ) _GET_DIST_NAME_MACRO(__VA_ARGS__, _DIST_NAME_3_ARGS, _DIST_NAME_2_ARGS)(__VA_ARGS__)

//---------------------------------------------------------------------------//
// Helper macro for setting up a basic OneDDistribution class python interface
//---------------------------------------------------------------------------//
%define %basic_distribution_interface_setup_helper( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMS... )

%feature("docstring") DIST_NAME( DISTRIBUTION, PARAMS )
"The RENAMED_DISTRIBUTION proxy class. This class can be evaluated, sampled
from, exported to an XML file (using the PyTrilinos.Teuchos.ParameterList and
the PyTrilinos.Teuchos.XMLParameterListWriter), and imported from an XML file
(using the PyTrilinos.Teuchos.ParameterList and the
PyTrilinos.Teuchos.XMLParameterListReader). Before sampling, make sure to
initialize the Frensie Pseudo-Random Number Generator
(PyFrensie.Utility.initFrensiePrng())"

%feature("autodoc",
"evaluate(RENAMED_DISTRIBUTION self, double indep_var_value ) -> double" )
DIST_NAME( DISTRIBUTION, PARAMS )::evaluate;

%feature("autodoc",
"evaluatePDF(RENAMED_DISTRIBUTION self, double indep_var_value ) -> double" )
DIST_NAME( DISTRIBUTION, PARAMS )::evaluatePDF;

%feature("autodoc",
"sample(RENAMED_DISTRIBUTION self) -> double" )
DIST_NAME( DISTRIBUTION, PARAMS )::sample;

%feature("autodoc",
"sampleAndRecordTrials(RENAMED_DISTRIBUTION self, unsigned int & trials ) -> double, unsigned int

Sample from the RENAMED_DISTRIBUTION and record the number of trials
(iterations) that were required to generate the sample. The first element of
the returned tuple is the sample. The second element of the returned tuple is
the trials counter. The trials counter can be reused as such:

  PyFrensie.Utility.initFrensiePrng()

  u = PyFrensie.Utility.RENAMED_DISTRIBUTION( ... )
  trials_counter = 0

  sample,trials_counter = u.sampleAndRecordTrials( trials_counter )
  sample,trials_counter = u.sampleAndRecordTrials( trials_counter )

  print trials_counter
  2")
DIST_NAME( DISTRIBUTION, PARAMS )::sampleAndRecordTrials;

%feature("autodoc",
"getUpperBoundOfIndepVar(RENAMED_DISTRIBUTION self) -> double" )
DIST_NAME( DISTRIBUTION, PARAMS )::getUpperBoundOfIndepVar;

%feature("autodoc",
"getLowerBoundOfIndepVar(RENAMED_DISTRIBUTION self) -> double" )
DIST_NAME( DISTRIBUTION, PARAMS )::getLowerBoundOfIndepVar;

// SWIG will not parse typedefs. Create some typemaps that map the
// typedefs to their true type (double)
%typemap(in) DIST_NAME( DISTRIBUTION, PARAMS )::IndepQuantity
{
  $1 = PyFloat_AsDouble($input);
}

%typemap(out) DIST_NAME( DISTRIBUTION, PARAMS )::IndepQuantity
{
  $result = PyFloat_FromDouble($1);
}

%typemap(out) DIST_NAME( DISTRIBUTION, PARAMS )::DepQuantity
{
  $result = PyFloat_FromDouble($1);
}

%typemap(out) DIST_NAME( DISTRIBUTION, PARAMS )::InverseIndepQuantity
{
  $result = PyFloat_FromDouble($1);
}

%template(RENAMED_DISTRIBUTION) DIST_NAME( DISTRIBUTION, PARAMS );

%enddef

//---------------------------------------------------------------------------//
// Helper macro for setting up a basic TabularOneDDistribution class py. int.
//---------------------------------------------------------------------------//
%define %basic_tab_distribution_interface_setup_helper( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMS... )

%feature("autodoc",
"evaluate(RENAMED_DISTRIBUTION self, double indep_var_value ) -> double" )
DIST_NAME( DISTRIBUTION, PARAMS )::evaluateCDF;

%feature("autodoc",
"sampleAndRecordBinIndex(RENAMED_DISTRIBUTION self) -> double, unsigned int

Sample from the RENAMED_DISTRIBUTION and record the bin index corresponding to
the sample. The first element of the returned tuple is the sample. The second
element of the returned tuple is the bin index. This method can be called
as follows:

  PyFrensie.Utility.initFrensiePrng()

  u = PyFrensie.Utility.RENAMED_DISTRIBUTION( ... )

  sample,bin_index = u.sampleAndRecordBinIndex()")
DIST_NAME( DISTRIBUTION, PARAMS )::sampleAndRecordBinIndex;

%feature("autodoc",
"sampleWithRandomNumber(RENAMED_DISTRIBUTION self, const double random_number) -> double

Sample from the RENAMED_DISTRIBUTION using the supplied random number instead
of using the hidden Utility::RandomNumberGenerator.")
DIST_NAME( DISTRIBUTION, PARAMS )::sampleWithRandomNumber;

%feature("autodoc",
"sampleInSubrange(RENAMED_DISTRIBUTION self, const double max_indep_var ) -> double

Sample from the RENAMED_DISTRIBUTION in the subrange
[self.getLowerBoundOfIndepVar(),max_indep_var]" )
DIST_NAME( DISTRIBUTION, PARAMS )::sampleInSubrange;

%feature("autodoc",
"sampleWithRandomNumberInSubrange(RENAMED_DISTRIBUTION self, const double random_number, const double max_indep_var ) -> double

Sample from the RENAMED_DISTRIBUTION using the supplied random number in the
subrange [self.getLowerBoundOfIndepVar(),max_indep_var]" )
DIST_NAME( DISTRIBUTION, PARAMS )::sampleWithRandomNumberInSubrange;

%basic_distribution_interface_setup_helper( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMS )

%enddef

//---------------------------------------------------------------------------//
// Helper macros for extending a OneDDistribution class python interface
//---------------------------------------------------------------------------//
%define %extend_distribution_interface_helper( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMS... )

%feature("autodoc",
"toParameterList(RENAMED_DISTRIBUTION self, const std::string & parameter_name, PyObject * python_parameter_list)

The python ParameterList only supports native python and numpy array
types. To add a python RENAMED_DISTRIBUTION to a python ParameterList, only
this function can be used. The dictionary methods cannot be used to add a
python RENAMED_DISTRIBUTION to a python ParameterList. Once a python
RENAMED_DISTRIBUTION object has been added to the python ParameterList, you
will not be able to call print or str will it. Be careful when calling the
ParameterList set method after a python RENAMED_DISTRIBUTION has been added as
it returns a ParameterList object which python will try (and fail) to print.
Calls to set should be done like this:

  PyFrensie.Utility.initFrensiePrng()

  u = PyFrensie.Utility.RENAMED_DISTRIBUTION( ... )

  p = PyTrilinos.Teuchos.ParameterList()

  u.toParameterList( 'my_dist', p )

  dummy = p.set( 'my_double', 1.0 )

You can use the XMLParameterListWriter to write the ParameterList to an
XML file like this (continued from the previous example):

  writer = PyTrilinos.Teuchos.XMLParameterListWriter()

  xml_obj = writer.toXML( p )

  xml_file = open( 'my_list', 'w' )
  xml_file.write( xml_obj.toString() )
  xml_file.close()
" )
DIST_NAME( DISTRIBUTION, PARAMS )::toParameterList;

%feature("autodoc",
"fromParameterList(RENAMED_DISTRIBUTION self, const std::string & parameter_name, PyObject * python_parameter_list)

The python ParameterList only supports native python and numpy array types. To
get a python RENAMED_DISTRIBUTION from a python ParameterList, only this
function can be used. The dictionary methods cannot be used to get a python
RENAMED_DISTRIBUTION from a python ParameterList.
" )
DIST_NAME( DISTRIBUTION, PARAMS )::fromParameterList;

%feature("autodoc",
"__str__(RENAMED_DISTRIBUTION self) -> string

Convert the RENAMED_DISTRIBUTION to a string. The output string format is the
same that is used to add the distribution to a Teuchos::ParameterList.")
DIST_NAME( DISTRIBUTION, PARAMS )::__str__;

%feature("autodoc",
"__repr__(RENAMED_DISTRIBUTION self) -> string

Convert the RENAMED_DISTRIBUTION to a string wrapped inside of
RENAMED_DISTRIBUTION(...).")
DIST_NAME( DISTRIBUTION, PARAMS )::__repr__;

%feature("autodoc",
"__eq__(RENAMED_DISTRIBUTION self, RENAMED_DISTRIBUTION other_dist ) -> bool

Check if two RENAMED_DISTRIBUTION objects are equal (using the == operator).
This will return the same value as self.isEqual( other_dist )" )
DIST_NAME( DISTRIBUTION, PARAMS )::__eq__;

%feature("autodoc",
"__ne__(RENAMED_DISTRIBUTION self, RENAMED_DISTRIBUTION other_dist ) -> bool

Check if two RENAMED_DISTRIBUTION objects are not equal (using the !=
operator). This will return the same value as !self.isEqual( other_dist )" )
DIST_NAME( DISTRIBUTION, PARAMS )::__ne__;

// Add some useful methods to the distribution class
%extend DIST_NAME( DISTRIBUTION, PARAMS )
{
  // Add a method for exporting to a Python parameter list
  void toParameterList( const std::string& name,
                        PyObject* python_parameter_list ) const
  {
    // Check that the python object is a parameter list
    if( !PyFrensie::addDistToParameterList( name, *$self, python_parameter_list ) )
    {
      PyErr_SetString(PyExc_TypeError, "The PyObject is not a PyTrilinos.Teuchos.ParameterList (a.k.a Teuchos::RCP<Teuchos::ParameterList>)." );
    }
  }

  // Add a method for importing from a Python parameter list
  void fromParameterList( const std::string& name,
                          PyObject* python_parameter_list )
  {
    PyFrensie::getDistFromParameterList( name, python_parameter_list, *$self );
  }

  // String representation method
  PyObject* __repr__() const
  {
    std::string repr_string( #RENAMED_DISTRIBUTION );
    repr_string += "(";

    std::ostringstream oss;

    $self->toStream( oss );

    repr_string += oss.str() + ")";

    return PyString_FromString( repr_string.c_str() );
  }

  // String conversion method
  PyObject* __str__() const
  {
    std::ostringstream oss;

    $self->toStream( oss );

    return PyString_FromString( oss.str().c_str() );
  }

  // Equality operator
  PyObject* __eq__( const RENAMED_DISTRIBUTION& other_dist) const
  {
    if( $self->isEqual( other_dist ) )
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  }

  // Inequality operator
  PyObject* __ne__( const RENAMED_DISTRIBUTION& other_dist) const
  {
    if( !$self->isEqual( other_dist ) )
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  }
};

%enddef

//---------------------------------------------------------------------------//
// Macro for setting up a basic OneDDistribution class python interface
//---------------------------------------------------------------------------//
%define %basic_distribution_interface_setup( DISTRIBUTION )

%basic_distribution_interface_setup_helper( DISTRIBUTION, DISTRIBUTION, void, void )

%enddef

//---------------------------------------------------------------------------//
// Macro for setting up a standard Distribution class python interface
//---------------------------------------------------------------------------//
%define %standard_distribution_interface_setup( DISTRIBUTION )

// Extend the distribution interface
%extend_distribution_interface_helper( DISTRIBUTION, DISTRIBUTION, void, void )

// Do the basic setup for this distribution
%basic_distribution_interface_setup_helper( DISTRIBUTION, DISTRIBUTION, void ,void )

%enddef

//---------------------------------------------------------------------------//
// Macro for setting up an advanced Distribution class python interface
//---------------------------------------------------------------------------//
%define %advanced_distribution_interface_setup( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMETER )

// Add a typedef for the renamed distribution so that the extended methods
// can be compiled
%inline %{
typedef DIST_NAME( DISTRIBUTION, PARAMETER, void, void ) RENAMED_DISTRIBUTION;
%}

// Extend the distribution interface
%extend_distribution_interface_helper( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMETER, void, void )

// Do the basic setup for this distribution
%basic_distribution_interface_setup_helper( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMETER, void, void )

%enddef

//---------------------------------------------------------------------------//
// Macro for setting up a basic TabularOneDDistribution class py. int.
//---------------------------------------------------------------------------//
%define %basic_tab_distribution_interface_setup( DISTRIBUTION )

%basic_tab_distribution_interface_setup_helper( DISTRIBUTION, DISTRIBUTION, void, void )

%enddef

//---------------------------------------------------------------------------//
// Macro for setting up a standard Tabular Distribution class python interface
//---------------------------------------------------------------------------//
%define %standard_tab_distribution_interface_setup( DISTRIBUTION )

// Extend the distribution inteface
%extend_distribution_interface_helper( DISTRIBUTION, DISTRIBUTION, void, void )

// Do the basic tabular setup for this distribution
%basic_tab_distribution_interface_setup_helper( DISTRIBUTION, DISTRIBUTION, void , void  )

%enddef

//---------------------------------------------------------------------------//
// Macro for setting up an advanced Tabular Distribution class python interface
//---------------------------------------------------------------------------//
%define %advanced_tab_distribution_interface_setup( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMETER )

// Add a typedef for the renamed distribution so that the extended methods
// can be compiled
%inline %{
typedef DIST_NAME( DISTRIBUTION, PARAMETER, void, void ) RENAMED_DISTRIBUTION;
%}

// Extend the distribution inteface
%extend_distribution_interface_helper( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMETER, void, void )

// Do the basic tabular setup for this distribution
%basic_tab_distribution_interface_setup_helper( RENAMED_DISTRIBUTION, DISTRIBUTION, PARAMETER, void, void )

%enddef


//---------------------------------------------------------------------------//
// end Utility_OneDDistributionHelpers.i
//---------------------------------------------------------------------------//