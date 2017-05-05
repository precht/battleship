cmake_minimum_required(VERSION 2.8.2)

# Set variables
set(PROJECT_NAME battleship)
# Cannot have two different targests with the same names.
# After adding lib/exe I will set their output name to #{PROJECT_NAME} without any suffix.
set(LIB_TARGET ${PROJECT_NAME}_lib)
set(EXE_TARGET ${PROJECT_NAME}_exe)

project(${PROJECT_NAME})

# Set compiler options
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
if(NOT MSVC)
    add_definitions( -Wall -Wextra -Wpedantic -g3 -gdwarf-4)
endif()
enable_language(CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

# setup boost on win
if(MSVC)
    if(NOT BOOST_ROOT)
        set(BOOST_ROOT "C:/local/boost_1_60_0")
    endif()
    set(Boost_INCLUDE_DIR "${BOOST_ROOT}")
    set(Boost_LIBRARY_DIR "${BOOST_ROOT}/lib32-msvc-14.0")
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)

    if(NOT EXISTS ${Boost_LIBRARY_DIR})
        message(WARNING "Boost library requires manual installation in Windows")
        message("1) Browse: https://sourceforge.net/projects/boost/files/boost-binaries/1.60.0/")
        message("2) Download: boost_1_60_0-msvc-14.0-32.exe")
        message("3) Install into default directory: C:\\local")
        message("")
        message(FATAL_ERROR "Boost libraries not found at ${Boost_LIBRARY_DIR}")
    endif()
	
    include_directories(${Boost_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIR})
endif()

#---------------------------------------------------------
# Main
#---------------------------------------------------------

find_package(Boost COMPONENTS program_options system filesystem REQUIRED)

set(MAIN_HEADERS
    include/exceptions.h
    include/Ship.h
    include/Grid.h
    include/ShipsGrid.h
    include/Player.h
    include/AIPlayer.h
    include/HumanPlayer.h
    include/ShootStrategy.h
    include/RandomStrategy.h
    include/GreedyStrategy.h
    include/GameLogic.h
    include/UI.h
    include/CLI.h
)

set(MAIN_SOURCES
    src/exceptions.cpp
    src/Ship.cpp
    src/Grid.cpp
    src/ShipsGrid.cpp
    src/Player.cpp
    src/AIPlayer.cpp
    src/HumanPlayer.cpp
    src/RandomStrategy.cpp
    src/GreedyStrategy.cpp
    src/GameLogic.cpp
    src/CLI.cpp
)

# Put executable files in bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# Put libs in lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Create static library with all files from src except main.cpp
add_library(${LIB_TARGET} ${MAIN_SOURCES} ${MAIN_HEADERS})
target_link_libraries(${LIB_TARGET}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
)
set_target_properties(${LIB_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Create executable and link with library
add_executable(${EXE_TARGET} src/main.cpp)
target_link_libraries(${EXE_TARGET} ${LIB_TARGET})
set_target_properties(${EXE_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

#---------------------------------------------------------
# Test
#---------------------------------------------------------

### Download and compile googletest (script from Crascit/DownloadProject)
# Put googletest libs in external/googletest directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/external/googletest)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/external/googletest)
include(CTest)
if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()
include(external/googletest/DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
# Prevent GoogleTest from overriding our compiler/linker options when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
# Restore directories paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
###

set(TEST_TARGET ${PROJECT_NAME}_test)

set(TEST_HEADERS
    include/test/Player_mock.h
    include/test/UI_mock.h
    include/test/ShootStrategy_mock.h
)

set(TEST_SOURCES
    src/test/Player_mock.cpp
    src/test/ShootStrategy_mock.cpp
    test/mocks_test.cpp
    test/Ship_test.cpp
    test/Grid_test.cpp
    test/ShipsGrid_test.cpp
    test/Player_test.cpp
    test/HumanPlayer_test.cpp
    test/AIPlayer_test.cpp
    test/GreedyStrategy_test.cpp
    test/RandomStrategy_test.cpp
    test/GameLogic_test.cpp
    test/CLI_test.cpp
)

add_executable(${TEST_TARGET} ${TEST_SOURCES} ${TEST_HEADERS})

# link googletest and my library
target_link_libraries(${TEST_TARGET}
    gtest
    gmock_main
    ${LIB_TARGET}
)

enable_testing()
add_test(tests ${TEST_TARGET})
