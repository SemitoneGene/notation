cmake_minimum_required(VERSION 3.21)

project(TestApp)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)

add_executable(testapp)

function(find_required_library LIB_NAME LIB_DISPLAY_NAME)
    find_library(${LIB_NAME} ${LIB_DISPLAY_NAME})
    if(NOT ${LIB_NAME})
        message(FATAL_ERROR "${LIB_DISPLAY_NAME} not found")
    endif()
endfunction()

if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    target_link_libraries(testapp PRIVATE ${COREFOUNDATION_LIBRARY} ${COREGRAPHICS_LIBRARY})
endif()

find_package(BZip2 REQUIRED)
find_package(EXPAT REQUIRED)
find_required_library(OPENCL_LIBRARY OpenCL)
find_required_library(OPENCV_CORE opencv_core)
find_required_library(ZLIB_LIBRARY z)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(NOTATION_ROOT ../../..)
set(MX_ROOT ../../../../mx)

add_subdirectory(${NOTATION_ROOT}/NotationLinux ${CMAKE_BINARY_DIR}/_build/NotationLinux)
add_subdirectory(${MX_ROOT} ${CMAKE_BINARY_DIR}/_build/mx)

set(RESOURCE_DIR ${NOTATION_ROOT}/Resources)
target_compile_definitions(testapp PRIVATE RESOURCE_DIR="${KOMP_ROOT}/Resources/")

add_compile_definitions(
        KOMP_BUILD_LINUX
        QT_NO_KEYWORDS        
        )

set(MX_BUILD ${NOTATION_ROOT}/mx/build)
set(NOTATION_BUILD ${NOTATION_ROOT}/NotationLinux/build)
set(FRAMEWORKS_SRC $${NOTATION_ROOT}/Frameworks)
set(NOTATION_SRC ${NOTATION_ROOT}/Notation-Sourcecode)
set(TEST_APP_SRC ${NOTATION_ROOT}/TestApp)
set(TEST_SUPPORT_SRC ${TEST_APP_SRC}/TestSupport)
set(CPUL_SRC ${TEST_APP_SRC}/Cpul)
set(TESTS_SRC ${TEST_APP_SRC}/Tests)

include_directories(${TEST_APP_SRC}
                    ${TEST_APP_SRC}/App/Linux
                    ${TEST_SUPPORT_SRC}
                    ${FRAMEWORKS_SRC}
                    ${FRAMEWORKS_SRC}/phmap
                    ${FRAMEWORKS_SRC}/FrameworksApple/boost/include
                    ${NOTATION_SRC}/Backend
                    ${NOTATION_SRC}/Backend/Linux
                    ${NOTATION_SRC}/Commands
                    ${NOTATION_SRC}/Config
                    ${NOTATION_SRC}/Core
                    ${NOTATION_SRC}/EventBus
                    ${NOTATION_SRC}/File
                    ${NOTATION_SRC}/Fonts
                    ${NOTATION_SRC}/MIDI
                    ${NOTATION_SRC}/MusicXML
                    ${NOTATION_SRC}/Notation
                    ${NOTATION_SRC}/Notation/Beaming
                    ${NOTATION_SRC}/Notation/BeamingOld
                    ${NOTATION_SRC}/Notation/Drawable
                    ${NOTATION_SRC}/Notation/Items
                    ${NOTATION_SRC}/Notation/MarkPlacement
                    ${NOTATION_SRC}/Notation/NoteSpacer
                    ${NOTATION_SRC}/Notation/SpatialPoint
                    ${NOTATION_SRC}/Score
                    )

target_sources(testapp PRIVATE main.cc
                TestFileRepository.cc
                ${CPUL_SRC}/cpulFailure.cpp
                ${CPUL_SRC}/cpulTest.cpp
                ${CPUL_SRC}/cpulTestRegistry.cpp
                ${CPUL_SRC}/cpulTestResult.cpp                
                ${TESTS_SRC}/AttachedItemTest.cc
                ${TESTS_SRC}/BarlineTest.cc
                ${TESTS_SRC}/BeamFactoryTest.cc
                ${TESTS_SRC}/BeamLineTest.cc
                ${TESTS_SRC}/BeamSaveTest.cc
                ${TESTS_SRC}/BeamSpecTest.cc
                ${TESTS_SRC}/BeamTest.cc
                ${TESTS_SRC}/BeamedNoteTest.cc
                ${TESTS_SRC}/BeatGroupingTest.cc
                ${TESTS_SRC}/BombeTest.cc
                ${TESTS_SRC}/BracketTest.cc
                ${TESTS_SRC}/ChordHeadTest.cc
                ${TESTS_SRC}/ChordSymbolRepository.cc
                ${TESTS_SRC}/ChordSymbolTest.cc
                ${TESTS_SRC}/ChordSymbolValueTest.cc
                ${TESTS_SRC}/ChordTest.cc
                ${TESTS_SRC}/CurveTest.cc
                ${TESTS_SRC}/FermataTest.cc
                ${TESTS_SRC}/FileLoadResultTest.cc
                ${TESTS_SRC}/KeySignatureTest.cc
                ${TESTS_SRC}/LordChordTest.cc
                ${TESTS_SRC}/MarkPositionTest.cc
                ${TESTS_SRC}/MeasureTest.cc
                ${TESTS_SRC}/MxClefTest.cc
                ${TESTS_SRC}/MxConversionsTest.cc
                ${TESTS_SRC}/MxDirectionTest.cc
                ${TESTS_SRC}/MxDynamicsTest.cc
                ${TESTS_SRC}/MxMarkTest.cc
                ${TESTS_SRC}/MxSlurTest.cc
                ${TESTS_SRC}/MxStemTest.cc
                ${TESTS_SRC}/MxTimeSignatureTest.cc
                ${TESTS_SRC}/NoteTest.cc
                ${TESTS_SRC}/PasteTest.cc
                ${TESTS_SRC}/PitchInfoTest.cc
                ${TESTS_SRC}/PlaybackNoteTest.cc
                ${TESTS_SRC}/RawAccidentalTest.cc
                ${TESTS_SRC}/SpatialPointTest.cc
                ${TESTS_SRC}/StaffTest.cc
                ${TESTS_SRC}/TempoValueTest.cc
                ${TESTS_SRC}/TestScores.cc
                ${TESTS_SRC}/TimeMapTest.cc
                ${TESTS_SRC}/TupletTest.cc
                ${TEST_SUPPORT_SRC}
                ${TEST_SUPPORT_SRC}/TestController.cc
                ${TEST_SUPPORT_SRC}/TestFile.cc
                )

target_link_libraries(testapp PRIVATE
                      ${BZIP2_LIBRARIES}
                      ${COREFOUNDATION_LIBRARY}
                      ${COREGRAPHICS_LIBRARY}
                      ${EXPAT_LIBRARIES}
                      ${GLIB_LIBRARIES} 
                      ${GIO_LIBRARIES}
                      ${OPENCL_LIBRARY}
                      ${OPENCV_CORE}
                      ${LIBXML2_LIBRARY}
                      ${PNG_LIBRARY}
                      ${ZLIB_LIBRARY}
                      ${MX_LIBRARY}
                      Qt6::Core 
                      Qt6::Widgets
                      mx
                      notation)
