cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW) # remove if CMake >= 2.8.11 required
endif()

PROJECT (IO)

if(NOT VTK_BINARY_DIR)
  find_package(VTK COMPONENTS
    vtkCommonCore
    vtkCommonDataModel
    vtkIOGeometry
    vtkIOXML
    vtkRenderingOpenGL
  )
  include(${VTK_USE_FILE})
endif()

add_executable(DumpXMLFile MACOSX_BUNDLE DumpXMLFile.cxx)
target_link_libraries(DumpXMLFile ${VTK_LIBRARIES})

add_executable(ParticleReader MACOSX_BUNDLE ParticleReader.cxx)
target_link_libraries(ParticleReader ${VTK_LIBRARIES})

if(BUILD_TESTING)
  if(vtkTestingRendering_LOADED)
    ######## Regression Testing ########
    set(vtk-example IOExamples)
    set(TestDumpXMLFile_ARGS "DATA{${VTK_TEST_INPUT_DIR}/cow.vtp}")
    set(TestParticleReader_ARGS
      "${VTK_TEST_DATA_DIR}/Data/golf.csv"
      ${VTK_TEST_OUTPUT_DIR}/TestParticleReader.vtp)
    vtk_add_test_cxx(NO_DATA NO_VALID NO_OUTPUT
      TestParticleReader.cxx
      TestDumpXMLFile.cxx
      )
    vtk_test_cxx_executable(${vtk-example}CxxTests RENDERING_FACTORY)
  endif()
endif()
