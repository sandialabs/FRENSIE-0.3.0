# Create the incoherent scattering distribution sampling tool
ADD_EXECUTABLE(sample_isd samplePhotonDistributionCore.cpp sample_isd.cpp)
TARGET_LINK_LIBRARIES(sample_isd monte_carlo_collision_native)

# Create the coherent scattering distribution sampling tool
ADD_EXECUTABLE(sample_csd samplePhotonDistributionCore.cpp sample_csd.cpp)
TARGET_LINK_LIBRARIES(sample_csd monte_carlo_collision_native)

# Add exec to install target
INSTALL(TARGETS sample_isd sample_csd
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)