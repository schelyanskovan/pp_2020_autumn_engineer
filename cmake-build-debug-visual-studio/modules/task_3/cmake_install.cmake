# Install script for directory: C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/modules/task_3

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Project")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/cmake-build-debug-visual-studio/modules/task_3/alekhin_d_dijkstras_algorithm/cmake_install.cmake")
  include("C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/cmake-build-debug-visual-studio/modules/task_3/anisimova_p_global_search/cmake_install.cmake")
  include("C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/cmake-build-debug-visual-studio/modules/task_3/frolova_o_radix_sort_simple_merge_update/cmake_install.cmake")
  include("C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/cmake-build-debug-visual-studio/modules/task_3/kustova_a_convex_hull_for_binary/cmake_install.cmake")
  include("C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/cmake-build-debug-visual-studio/modules/task_3/maksimov_a_mark_components/cmake_install.cmake")
  include("C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/cmake-build-debug-visual-studio/modules/task_3/nekravtsev_a_simpson/cmake_install.cmake")
  include("C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/cmake-build-debug-visual-studio/modules/task_3/oganyan_r_global_search/cmake_install.cmake")
  include("C:/Users/OgRob/Desktop/paral/new_paral/pp_2020_autumn_engineer/cmake-build-debug-visual-studio/modules/task_3/ognev_d_quick_sort/cmake_install.cmake")

endif()

