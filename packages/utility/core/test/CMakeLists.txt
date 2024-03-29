ADD_EXECUTABLE(tstAssertion 
  tstAssertion.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstAssertion utility_core)
ADD_TEST(Assertion_test tstAssertion)

ADD_EXECUTABLE(tstContractException 
  tstContractException.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstContractException utility_core)
ADD_TEST(ContractException_test tstContractException)

ADD_EXECUTABLE(tstSortAlgorithms 
  tstSortAlgorithms.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstSortAlgorithms utility_core)
ADD_TEST(SortAlgorithms_test tstSortAlgorithms)

ADD_EXECUTABLE(tstSearchAlgorithms 
  tstSearchAlgorithms.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstSearchAlgorithms utility_core)
ADD_TEST(SearchAlgorithms_test tstSearchAlgorithms)

ADD_EXECUTABLE(tstExponentiationAlgorithms 
  tstExponentiationAlgorithms.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstExponentiationAlgorithms utility_core)
ADD_TEST(ExponentiationAlgorithms_test tstExponentiationAlgorithms)

ADD_EXECUTABLE(tstInterpolationPolicy 
  tstInterpolationPolicy.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstInterpolationPolicy utility_core)
ADD_TEST(InterpolationPolicy_test tstInterpolationPolicy)

ADD_EXECUTABLE(tstLinLinLinTwoDInterpolationPolicy
  tstLinLinLinTwoDInterpolationPolicy.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLinLinLinTwoDInterpolationPolicy utility_core)
ADD_TEST(LinLinLinTwoDInterpolationPolicy_test tstLinLinLinTwoDInterpolationPolicy)

ADD_EXECUTABLE(tstLinLinLogTwoDInterpolationPolicy
  tstLinLinLogTwoDInterpolationPolicy.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLinLinLogTwoDInterpolationPolicy utility_core)
ADD_TEST(LinLinLogTwoDInterpolationPolicy_test tstLinLinLogTwoDInterpolationPolicy)

ADD_EXECUTABLE(tstLinLogLinTwoDInterpolationPolicy
  tstLinLogLinTwoDInterpolationPolicy.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLinLogLinTwoDInterpolationPolicy utility_core)
ADD_TEST(LinLogLinTwoDInterpolationPolicy_test tstLinLogLinTwoDInterpolationPolicy)

ADD_EXECUTABLE(tstLinLogLogTwoDInterpolationPolicy
  tstLinLogLogTwoDInterpolationPolicy.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLinLogLogTwoDInterpolationPolicy utility_core)
ADD_TEST(LinLogLogTwoDInterpolationPolicy_test tstLinLogLogTwoDInterpolationPolicy)

ADD_EXECUTABLE(tstLogLinLinTwoDInterpolationPolicy
  tstLogLinLinTwoDInterpolationPolicy.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLogLinLinTwoDInterpolationPolicy utility_core)
ADD_TEST(LogLinLinTwoDInterpolationPolicy_test tstLogLinLinTwoDInterpolationPolicy)

ADD_EXECUTABLE(tstLogLogLinTwoDInterpolationPolicy
  tstLogLogLinTwoDInterpolationPolicy.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLogLogLinTwoDInterpolationPolicy utility_core)
ADD_TEST(LogLogLinTwoDInterpolationPolicy_test tstLogLogLinTwoDInterpolationPolicy)

ADD_EXECUTABLE(tstLogLinLogTwoDInterpolationPolicy
  tstLogLinLogTwoDInterpolationPolicy.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLogLinLogTwoDInterpolationPolicy utility_core)
ADD_TEST(LogLinLogTwoDInterpolationPolicy_test tstLogLinLogTwoDInterpolationPolicy)

ADD_EXECUTABLE(tstLogLogLogTwoDInterpolationPolicy
  tstLogLogLogTwoDInterpolationPolicy.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLogLogLogTwoDInterpolationPolicy utility_core)
ADD_TEST(LogLogLogTwoDInterpolationPolicy_test tstLogLogLogTwoDInterpolationPolicy)

ADD_EXECUTABLE(tstSetOperationFunctor 
  tstSetOperationFunctor.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstSetOperationFunctor utility_core)
ADD_TEST(SetOperationFunctor_test tstSetOperationFunctor)

ADD_EXECUTABLE(tstDirectionHelpers 
  tstDirectionHelpers.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstDirectionHelpers utility_core)
ADD_TEST(DirectionHelpers_test tstDirectionHelpers)

ADD_EXECUTABLE(tstCylindricalCoordinateHelpers 
  tstCylindricalCoordinateHelpers.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstCylindricalCoordinateHelpers utility_core)
ADD_TEST(CylindricalCoordinateHelpers_test tstCylindricalCoordinateHelpers)

ADD_EXECUTABLE(tstSphericalCoordinateHelpers 
  tstSphericalCoordinateHelpers.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstSphericalCoordinateHelpers utility_core)
ADD_TEST(SphericalCoordinateHelpers_test tstSphericalCoordinateHelpers)

ADD_EXECUTABLE(tstDataProcessor 
  tstDataProcessor.cpp 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstDataProcessor utility_core)
ADD_TEST(DataProcessor_test tstDataProcessor)

ADD_EXECUTABLE(tstFortranStringHelpers 
  tstFortranStringHelpers.cpp 
  string_conversion.F90 
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstFortranStringHelpers utility_core)
ADD_TEST(FortranStringHelpers_test tstFortranStringHelpers)

ADD_EXECUTABLE(tstFortranFileHelpers 
  tstFortranFileHelpers.cpp )
TARGET_LINK_LIBRARIES(tstFortranFileHelpers utility_core)
ADD_TEST(FortranFileHelpers_test tstFortranFileHelpers
--test_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/check_file.txt")

ADD_EXECUTABLE(tstODEPACKHelper 
  tstODEPACKHelper.cpp )
TARGET_LINK_LIBRARIES(tstODEPACKHelper utility_core)
ADD_TEST(ODEPACKHelper_test tstODEPACKHelper)

ADD_EXECUTABLE(tstODESolver 
  tstODESolver.cpp )
TARGET_LINK_LIBRARIES(tstODESolver utility_core)
ADD_TEST(ODESolver_test tstODESolver)

ADD_EXECUTABLE(tstBoostSolver 
  tstBoostSolver.cpp )
TARGET_LINK_LIBRARIES(tstBoostSolver utility_core)
ADD_TEST(BoostSolver_test tstBoostSolver)

ADD_EXECUTABLE(tstKinematicHelpers 
  tstKinematicHelpers.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstKinematicHelpers utility_core)
ADD_TEST(KinematicHelpers_test tstKinematicHelpers)

ADD_EXECUTABLE(tstLegendrePolynomial 
  tstLegendrePolynomial.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstLegendrePolynomial utility_core)
ADD_TEST(LegendrePolynomial_test tstLegendrePolynomial)

ADD_EXECUTABLE(tstGaussLegendreQuadratureSet 
  tstGaussLegendreQuadratureSet.cpp )
TARGET_LINK_LIBRARIES(tstGaussLegendreQuadratureSet utility_core)
ADD_TEST(GaussLegendreQuadratureSet_test tstGaussLegendreQuadratureSet)

ADD_EXECUTABLE(tstSloanRadauQuadrature 
  tstSloanRadauQuadrature.cpp)
TARGET_LINK_LIBRARIES(tstSloanRadauQuadrature utility_core)
ADD_TEST(SloanRadauQuadrature_test tstSloanRadauQuadrature)

ADD_EXECUTABLE(tstGridGenerator 
  tstGridGenerator.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstGridGenerator utility_core)
ADD_TEST(GridGenerator_test tstGridGenerator)

ADD_EXECUTABLE(tstTetrahedronHelpers
  tstTetrahedronHelpers.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstTetrahedronHelpers utility_core)
ADD_TEST(TetrahedronHelpers_test tstTetrahedronHelpers)

ADD_EXECUTABLE(tstStandardHashBasedGridSearcher 
  tstStandardHashBasedGridSearcher.cpp)
TARGET_LINK_LIBRARIES(tstStandardHashBasedGridSearcher utility_core)
ADD_TEST(StandardHashBasedGridSearcher_test tstStandardHashBasedGridSearcher)

ADD_EXECUTABLE(tstArrayString tstArrayString.cpp)
TARGET_LINK_LIBRARIES(tstArrayString utility_core)
ADD_TEST(ArrayString_test tstArrayString --test_array_xml_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_arrays.xml")

ADD_EXECUTABLE(tstMeasurement 
  tstMeasurement.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstMeasurement utility_core)
ADD_TEST(Measurement_test tstMeasurement)

