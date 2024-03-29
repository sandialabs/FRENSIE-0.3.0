# Find all headers and sources (no updates to this file necessary!)
FILE(GLOB DATA_ENDF_HEADERS *.hpp)
FILE(GLOB DATA_ENDF_SOURCES *.cpp *.F90)

IF(CMAKE_VERBOSE_CONFIGURE)
  MESSAGE("DATA_ENDF_HEADERS = ${DATA_ENDF_HEADERS}")
  MESSAGE("DATA_ENDF_SOURCES = ${DATA_ENDF_SOURCES}")
ENDIF()

SET(SUBPACKAGE_LIB_NAME data_endf)

# Create the utilitycore library
ADD_LIBRARY(${SUBPACKAGE_LIB_NAME} ${DATA_ENDF_SOURCES})
TARGET_LINK_LIBRARIES(${SUBPACKAGE_LIB_NAME} utility_core)
IF(${FRENSIE_ENABLE_MPI})
  TARGET_LINK_LIBRARIES(${SUBPACKAGE_LIB_NAME} ${MPI_CXX_LIBRARIES})
ENDIF()

INSTALL(TARGETS ${SUBPACKAGE_LIB_NAME}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
INSTALL(FILES ${DATA_ENDF_HEADERS}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
