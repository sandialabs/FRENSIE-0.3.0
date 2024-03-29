 ADD_EXECUTABLE(tstStateSource
  tstStateSource.cpp)
TARGET_LINK_LIBRARIES(tstStateSource monte_carlo_source_native)
ADD_TEST(StateSource_test tstStateSource)

IF(FRENSIE_ENABLE_DAGMC)
  ADD_EXECUTABLE(tstDistributedSource
    tstDistributedSource.cpp)
  TARGET_LINK_LIBRARIES(tstDistributedSource monte_carlo_source_native)
  ADD_TEST(DistributedSource_test tstDistributedSource --test_sat_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_source_geom.sat")
ENDIF()

IF(FRENSIE_ENABLE_DAGMC)
  ADD_EXECUTABLE(tstCompoundSource
    tstCompoundSource.cpp)
  TARGET_LINK_LIBRARIES(tstCompoundSource monte_carlo_source_native)
  ADD_TEST(CompoundSource_test tstCompoundSource --test_sat_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_source_geom.sat")
ENDIF()

IF(FRENSIE_ENABLE_DAGMC)
  ADD_EXECUTABLE(tstStandardParticleSourceFactory
    tstStandardParticleSourceFactory.cpp)
  TARGET_LINK_LIBRARIES(tstStandardParticleSourceFactory monte_carlo_source_native)
  ADD_TEST(StandardParticleSourceFactory_test tstStandardParticleSourceFactory --test_simple_source_xml_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_simple_source.xml" --test_complex_source_xml_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_complex_source.xml" --test_compound_source_xml_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_compound_source.xml" --test_sat_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_source_geom.sat")
ENDIF()

IF(FRENSIE_ENABLE_DAGMC)
  ADD_EXECUTABLE(tstSourceModuleInterface_Native
    tstModuleInterface.cpp)
  TARGET_LINK_LIBRARIES(tstSourceModuleInterface_Native monte_carlo_source_native)
  ADD_TEST(SourceModuleInterface_Native_test tstSourceModuleInterface_Native --test_source_xml_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_complex_source.xml" --test_sat_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_source_geom.sat")
ENDIF()
