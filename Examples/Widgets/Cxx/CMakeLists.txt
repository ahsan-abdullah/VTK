cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW) # remove if CMake >= 2.8.11 required
endif()

PROJECT (Widgets)

if(NOT VTK_BINARY_DIR)
  find_package(VTK COMPONENTS
    vtkCommonComputationalGeometry
    vtkCommonCore
    vtkCommonDataModel
    vtkFiltersSources
    vtkInteractionStyle
    vtkInteractionWidgets
    vtkRenderingCore
    vtkRenderingOpenGL
    vtkRenderingFreeTypeOpenGL
)
  include(${VTK_USE_FILE})
endif()

add_executable(Slider MACOSX_BUNDLE Slider.cxx)
target_link_libraries(Slider ${VTK_LIBRARIES})

add_executable(Slider2D MACOSX_BUNDLE Slider2D.cxx)
target_link_libraries(Slider2D ${VTK_LIBRARIES})

add_executable(BalloonWidget MACOSX_BUNDLE BalloonWidget.cxx)
target_link_libraries(BalloonWidget ${VTK_LIBRARIES})

if(BUILD_TESTING)
  if(vtkTestingRendering_LOADED)
    ######## Regression Testing ########
    set(vtk-example WidgetsExamples)
    vtk_add_test_cxx(
      TestSlider.cxx
      TestSlider2D.cxx
      TestBalloonWidget.cxx
      )
    vtk_test_cxx_executable(${vtk-example}CxxTests RENDERING_FACTORY)
  endif()
endif()
