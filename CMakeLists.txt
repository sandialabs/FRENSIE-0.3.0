# Define the project name
INCLUDE(${CMAKE_SOURCE_DIR}/cmake/ProjectName.cmake)

# Define the project version
INCLUDE(${CMAKE_SOURCE_DIR}/cmake/Version.cmake)

# Define the minimum required version of CMake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

# Enforce an out-of-source build before
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  MESSAGE(STATUS "${PROJECT_NAME} requires an out-of-source build.")
  MESSAGE(STATUS "The following procedure is recommemded for building: ")
  MESSAGE(STATUS "  1.) exit the ${PROJECT_NAME} root directory.")
  MESSAGE(STATUS "  2.) create a new directory named ${PROJECT_NAME}_src")
  MESSAGE(STATUS "  3.) move the ${PROJECT_NAME} directory to the new directory")
  MESSAGE(STATUS "  4.) create a directory called build in the new directory.")
  MESSAGE(STATUS "  5.) copy the exaple configuration script to the build directory")
  MESSAGE(STATUS "  6.) change the CMAKE_INSTALL_PREFIX to the directory above the build directory")
  MESSAGE(STATUS "  7.) change the directory containing the CMakeLists.txt file in the configuration script to ../${PROJECT_NAME}_src")
  MESSAGE(STATUS "  8.) run the configure.sh script from the build directory")
  MESSAGE(FATAL_ERROR "in-source build detected")
ENDIF()
    
# Find Trilinos
# Use the Trilinos_PREFIX, if the user set it, to help find Trilinos.
# The final location will be held in Trilinos_DIR which must point at
#  "<prefix>/lib/cmake/Trilinos".
SET(CMAKE_PREFIX_PATH
  ${TRILINOS_PREFIX}
  ${CMAKE_PREFIX_PATH})

# Get Trilinos as one entity
FIND_PACKAGE(Trilinos REQUIRED)

# Echo trilinos build info
IF(CMAKE_VERBOSE_CONFIGURE)
  MESSAGE("\nFound Trilinos!  Here are the details: ")
  MESSAGE("   Trilinos_DIR = ${Trilinos_DIR}")
  MESSAGE("   Trilinos_VERSION = ${Trilinos_VERSION}")
  MESSAGE("   Trilinos_PACKAGE_LIST = ${Trilinos_PACKAGE_LIST}")
  MESSAGE("   Trilinos_LIBRARIES = ${Trilinos_LIBRARIES}")
  MESSAGE("   Trilinos_INCLUDE_DIRS = ${Trilinos_INCLUDE_DIRS}")
  MESSAGE("   Trilinos_LIBRARY_DIRS = ${Trilinos_LIBRARY_DIRS}")
  MESSAGE("   Trilinos_TPL_LIST = ${Trilinos_TPL_LIST}")
  MESSAGE("   Trilinos_TPL_INCLUDE_DIRS = ${Trilinos_TPL_INCLUDE_DIRS}")
  MESSAGE("   Trilinos_TPL_LIBRARIES = ${Trilinos_TPL_LIBRARIES}")
  MESSAGE("   Trilinos_TPL_LIBRARY_DIRS = ${Trilinos_TPL_LIBRARY_DIRS}")
  MESSAGE("   Trilinos_BUILD_SHARED_LIBS = ${Trilinos_BUILD_SHARED_LIBS}")
  MESSAGE("End of Trilinos details\n")
ENDIF()

# Make sure to use the same compilers and flags as Trilinos
SET(CMAKE_CXX_COMPILER ${Trilinos_CXX_COMPILER} )
SET(CMAKE_C_COMPILER ${Trilinos_C_COMPILER} )
SET(CMAKE_Fortran_COMPILER ${Trilinos_Fortran_COMPILER} )

SET(CMAKE_CXX_FLAGS  "${Trilinos_CXX_COMPILER_FLAGS} ${CMAKE_CXX_FLAGS}")
SET(CMAKE_C_FLAGS  "${Trilinos_C_COMPILER_FLAGS} ${CMAKE_C_FLAGS}")
SET(CMAKE_Fortran_FLAGS  "${Trilinos_Fortran_COMPILER_FLAGS} ${CMAKE_Fortran_FLAGS}")

# Find Specific Trilinos Libraries
FIND_LIBRARY(TEUCHOS teuchos ${TRILINOS_LIBRARY_DIRS})
IF(${TEUCHOS} MATCHES NOTFOUND)
  MESSAGE(FATAL_ERROR "The teuchos library could not be found")
ELSE()
  MESSAGE(" Teuchos_LIBRARY = ${TEUCHOS}")
ENDIF(${TEUCHOS} MATCHES NOTFOUND)

# Find the standard Teuchos Unit Test main file
FIND_PATH(TEUCHOS_STD_UNIT_TEST_MAIN_PATH 
  NAMES Teuchos_StandardUnitTestMain.cpp
  PATHS ${Trilinos_DIR}/../../../src/packages/teuchos/test/UnitTest/)
IF(${TEUCHOS_STD_UNIT_TEST_MAIN_PATH} MATCHES NOTFOUND)
  MESSAGE(FATAL_ERROR "The teuchos standard Unit Test Main could not be found")
ENDIF(${TEUCHOS_STD_UNIT_TEST_MAIN_PATH} MATCHES NOTFOUND)
SET(TEUCHOS_STD_UNIT_TEST_MAIN 
  ${TEUCHOS_STD_UNIT_TEST_MAIN_PATH}/Teuchos_StandardUnitTestMain.cpp)

# Find MOAB
SET(MOAB_INCLUDE_DIRS ${MOAB_PREFIX}/include)
SET(MOAB_LIBRARY_DIRS ${MOAB_PREFIX}/lib)
FIND_LIBRARY(MOAB MOAB ${MOAB_LIBRARY_DIRS})
IF(${MOAB} MATCHES NOTFOUND)
  MESSAGE(FATAL_ERROR "The moab library could not be found.")
ENDIF(${MOAB} MATCHES NOTFOUND)
IF(CMAKE_VERBOSE_CONFIGURE)
  MESSAGE("\nFound MOAB!  Here are the details: ")
  MESSAGE(" MOAB_PREFIX = ${MOAB_PREFIX}")
  MESSAGE(" MOAB_INCLUDE_DIRS = ${MOAB_INCLUDE_DIRS}")
  MESSAGE(" MOAB_LIBRARY_DIRS = ${MOAB_LIBRARY_DIRS}")
  MESSAGE(" MOAB_LIBRARY = ${MOAB}")
  MESSAGE("End of MOAB details\n")
ENDIF()

# Find HDF5
SET(HDF5_INCLUDE_DIRS 
  ${HDF5_PREFIX}/include
  ${HDF5_PREFIX}/include/cpp
  ${HDF5_PREFIX}/include/hl)
SET(HDF5_LIBRARY_DIRS ${HDF5_PREFIX}/lib)
FIND_LIBRARY(HDF5CPP libhdf5_cpp.a ${HDF5_LIBRARY_DIRS})
IF(${HDF5CPP} MATCHES NOTFOUND)
  MESSAGE(FATAL_ERROR "The HDF5 cpp library could not be found.")
ENDIF(${HDF5CPP} MATCHES NOTFOUND)
FIND_LIBRARY(HDF5CORE libhdf5.a ${HDF5_LIBRARY_DIRS})
IF(${HDF5CORE} MATCHES NOTFOUND)
  MESSAGE(FATAL_ERROR "The HDF5 core library could not be found.")
ENDIF(${HDF5CORE} MATCHES NOTFOUND)
SET(HDF5 ${HDF5CPP} ${HDF5CORE})
IF(CMAKE_VERBOSE_CONFIGURE)
  MESSAGE("\nFound HDF5! Here are the details: ")
  MESSAGE(" HDF5_PREFIX = ${HDF5_PREFIX}")
  MESSAGE(" HDF5_INCLUDE_DIRS = ${HDF5_INCLUDE_DIRS}")
  MESSAGE(" HDF5_LIBRARY_DIRS = ${HDF5_LIBRARY_DIRS}")
  MESSAGE(" HDF5_LIBRARY = ${HDF5}")
  MESSAGE("End of HDF5 details\n")
ENDIF()

# Find BOOST
SET(BOOST_INCLUDE_DIRS ${BOOST_PREFIX}/include/boost)
SET(BOOST_LIBRARY_DIRS ${BOOST_PREFIX}/lib)
IF(CMAKE_VERBOSE_CONFIGURE)
  MESSAGE("\nFound BOOST! Here are the details: ")
  MESSAGE(" BOOST_PREFIX = ${BOOST_PREFIX}")
  MESSAGE(" BOOST_INCLUDE_DIRS = ${BOOST_INCLUDE_DIRS}")
  MESSAGE(" BOOST_LIBRARY_DIRS = ${BOOST_LIBRARY_DIRS}")
  MESSAGE("End of BOOST details\n")
ENDIF()

# Find SPRNG
SET(SPRNG_INCLUDE_DIRS ${SPRNG_PREFIX}/include)
SET(SPRNG_LIBRARY_DIRS ${SPRNG_PREFIX}/lib)
IF(CMAKE_VERBOSE_CONFIGURE)
  MESSAGE("\nFound SPRNG! Here are the details: ")
  MESSAGE(" SPRNG_PREFIX = ${SPRNG_PREFIX}")
  MESSAGE(" SPRNG_INCLUDE_DIRS = ${SPRNG_INCLUDE_DIRS}")
  MESSAGE(" SPRNG_LIBRARY_DIRS = ${SPRNG_LIBRARY_DIRS}")
  MESSAGE("End of SPRNG details\n")
ENDIF()

# Include paths
INCLUDE_DIRECTORIES(${Trilinos_INCLUDE_DIRS} ${Trilinos_TPL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${MOAB_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SPRNG_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(src/types)

# Library paths
LINK_DIRECTORIES(${Trilinos_LIBRARY_DIRS} ${Trilinos_TPL_LIBRARY_DIRS})
LINK_DIRECTORIES(${MOAB_LIBRARY_DIRS})
LINK_DIRECTORIES(${HDF5_LIBRARY_DIRS})
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS})
LINK_DIRECTORIES(${SPRNG_LIBRARY_DIRS})
LINK_DIRECTORIES(${CMAKE_LIBRARY_PATH})

# Add Design-by-Contract
IF(${PROJECT_NAME}_ENABLE_DBC)
  SET(HAVE_${PROJECT_NAME}_DBC "1")
ELSE()
  SET(HAVE_${PROJECT_NAME}_DBC "0")
ENDIF()

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/dbc_config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_config.hpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# Include CTEST module
ENABLE_TESTING()
INCLUDE(Dart)

# Add source subdirectory
ADD_SUBDIRECTORY(src)

# Add test subdirectory
ADD_SUBDIRECTORY(test)