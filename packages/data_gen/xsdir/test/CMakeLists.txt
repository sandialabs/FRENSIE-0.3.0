ADD_EXECUTABLE(tstXsdirEntry 
  tstXsdirEntry.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstXsdirEntry data_gen_xsdir)
ADD_TEST(XsdirEntry_test tstXsdirEntry)

ADD_EXECUTABLE(tstContinuousEnergyNeutronXsdirEntry 
  tstContinuousEnergyNeutronXsdirEntry.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstContinuousEnergyNeutronXsdirEntry data_gen_xsdir)
ADD_TEST(ContinuousEnergyNeutronXsdirEntry_test tstContinuousEnergyNeutronXsdirEntry)

ADD_EXECUTABLE(tstSAlphaBetaXsdirEntry 
  tstSAlphaBetaXsdirEntry.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstSAlphaBetaXsdirEntry data_gen_xsdir)
ADD_TEST(SAlphaBetaXsdirEntry_test tstSAlphaBetaXsdirEntry)

ADD_EXECUTABLE(tstElectronPhotonRelaxationXsdirEntry 
  tstElectronPhotonRelaxationXsdirEntry.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstElectronPhotonRelaxationXsdirEntry data_gen_xsdir)
ADD_TEST(ElectronPhotonRelaxationXsdirEntry_test tstElectronPhotonRelaxationXsdirEntry)

ADD_EXECUTABLE(tstPhotonuclearXsdirEntry 
  tstPhotonuclearXsdirEntry.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstPhotonuclearXsdirEntry data_gen_xsdir)
ADD_TEST(PhotonuclearXsdirEntry_test tstPhotonuclearXsdirEntry)

ADD_EXECUTABLE(tstXsdirEntryFactory 
  tstXsdirEntryFactory.cpp
  ${TEUCHOS_STD_UNIT_TEST_MAIN})
TARGET_LINK_LIBRARIES(tstXsdirEntryFactory data_gen_xsdir)
ADD_TEST(XsdirEntryFactory_test tstXsdirEntryFactory)

ADD_EXECUTABLE(tstXsdir tstXsdir.cpp )
TARGET_LINK_LIBRARIES(tstXsdir data_gen_xsdir)
ADD_TEST(Xsdir_test tstXsdir --test_xsdir_file="${CMAKE_CURRENT_SOURCE_DIR}/test_files/test_xsdir")