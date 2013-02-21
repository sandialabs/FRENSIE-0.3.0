//---------------------------------------------------------------------------//
// \file   ArrayTraitsDecl.hpp
// \author Alex Robinson
// \brief  Array Traits declaration
//---------------------------------------------------------------------------//

#ifndef ARRAY_TRAITS_DECL_HPP
#define ARRAY_TRAITS_DECL_HPP

namespace FACEMC{

/*!
 * \brief This is the default structure used by ArrayTraits<Array> to produce
 * a compile time error when the specialization does not exist for array type
 * Array
 */
template<typename T, template<typename> class Array>
struct UndefinedArrayTraits
{
  //! This function should not compile if there is any attempt to instantiate!
  static inline T notDefined() { return Array::this_type_is_missing_a_specialization(); }
};

/*!
 * \brief This structure defines some basic traits for Arrays to be used in 
 * FACEMC. This struct should allow use of either std::vector or any of the
 * Teuchos arrays anywhere in FACEMC. Any other arrays will not be supported.
 * The functions in the templated base unspecialized struct are designed not
 * to compile (giving a nice compile-time error message) and therefore
 * specializations must be written for each array type
 * \note <ol>
 * <li> The default defined specializations are provided for \c std::vector,
 * \c Teuchos::Array, \c Teuchos::ArrayRCP, \c Teuchos::TwoDArray
 * </ol>
 */
template<typename T, template<typname> class Array>
struct ArrayTraits
{
  //! Return a pointer to the front of the array (mimics Teuchos functionality)
  static inline T* getRawPtr(Array array) { (void)UndefinedArrayTriats<T,Array>::notDefined(); return 0; }
  static inline const T* getRawPtr(Array array) { (void)UndefinedArrayTraits<T,Array>::notDefined(); return 0; }
  //! Resize the array
  static inline void resize(Array array, size_type n) { (void)UndefinedArrayTraits<T,Array>::notDefined(); return 0; }
};

} // end FACEMC namespace

#endif // end ARRAY_TRAITS_DECL_HPP

//---------------------------------------------------------------------------//
// end ArrayTraitsDecl.hpp
//---------------------------------------------------------------------------//

