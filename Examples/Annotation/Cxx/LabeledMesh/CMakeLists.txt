cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW) # remove if CMake >= 2.8.11 required
endif()

PROJECT (LabeledMesh)

if(NOT VTK_BINARY_DIR)
  find_package(VTK COMPONENTS
    vtkCommonCore
    vtkCommonDataModel
    vtkFiltersCore
    vtkFiltersGeneral
    vtkFiltersSources
    vtkRenderingCore
    vtkRenderingLabel
    vtkRenderingOpenGL
    vtkRenderingFreeTypeOpenGL
  )
  include (${VTK_USE_FILE})
endif()

set(Libraries ${VTK_LIBRARIES})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_NAME}.cxx)
target_link_libraries(${PROJECT_NAME} ${Libraries})
