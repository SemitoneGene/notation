cmake_minimum_required(VERSION 3.21)

# The name of the library
project(notation)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

add_compile_definitions(
        KOMP_BUILD_LINUX
        QT_NO_KEYWORDS        
        )

# Add the custom cmake items
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.6.0 REQUIRED COMPONENTS filesystem regex)
include_directories(${Boost_INCLUDE_DIRS})
#    add_executable(progname file1.cxx file2.cxx)
#    target_link_libraries(progname ${Boost_LIBRARIES})
#endif()

# OpenCV
find_package(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

# Qt
find_package(Qt6 COMPONENTS Core REQUIRED)

add_definitions(-DUSE_OPENCV -DKOMP_BUILD_LINUX)

include_directories(../Frameworks/
                    ../Frameworks/json/include
                    ../Frameworks/phmap
                    ../Frameworks/FrameworksApple/Mx.xcframework/macos-arm64_x86_64/MxmacOS.framework/Versions/A/Resources
                    ../Notation-Sourcecode/Backend
                    ../Notation-Sourcecode/Backend/Linux
                    ../Notation-Sourcecode/Clipboard
                    ../Notation-Sourcecode/Commands
                    ../Notation-Sourcecode/Config
                    ../Notation-Sourcecode/Core
                    ../Notation-Sourcecode/EventBus
                    ../Notation-Sourcecode/File
                    ../Notation-Sourcecode/Fonts
                    ../Notation-Sourcecode/Fonts/Linux
                    ../Notation-Sourcecode/MIDI
                    ../Notation-Sourcecode/MusicXML
                    ../Notation-Sourcecode/Notation
                    ../Notation-Sourcecode/Notation/Beaming
                    ../Notation-Sourcecode/Notation/BeamingOld
                    ../Notation-Sourcecode/Notation/Drawable
                    ../Notation-Sourcecode/Notation/Items
                    ../Notation-Sourcecode/Notation/MarkPlacement
                    ../Notation-Sourcecode/Notation/NoteSpacer
                    ../Notation-Sourcecode/Notation/SpatialPoint
                    ../Notation-Sourcecode/Score
                    ../Notation-Sourcecode/Score/Linux)

file(GLOB Backend_Source
        "../Notation-Sourcecode/Backend/*.h"
        "../Notation-Sourcecode/Backend/*.cc")

file(GLOB Backend_Linux_Source
        "../Notation-Sourcecode/Backend/Linux/*.h"
        "../Notation-Sourcecode/Backend/Linux/*.cc")

file(GLOB Commands_Source
        "../Notation-Sourcecode/Commands/*.h"
        "../Notation-Sourcecode/Commands/*.cc")

file(GLOB Config_Source
        "../Notation-Sourcecode/Config/*.h"
        "../Notation-Sourcecode/Config/*.cc")

file(GLOB Core_Source
        "../Notation-Sourcecode/Core/*.h"
        "../Notation-Sourcecode/Core/*.c"
        "../Notation-Sourcecode/Core/*.cc")

file(GLOB EventBus_Source
        "../Notation-Sourcecode/EventBus/*.h"
        "../Notation-Sourcecode/EventBus/*.cc")

file(GLOB File_Source
        "../Notation-Sourcecode/File/*.h"
        "../Notation-Sourcecode/File/*.cc")

file(GLOB Fonts_Source
        "../Notation-Sourcecode/Fonts/*.h"
        "../Notation-Sourcecode/Fonts/*.cc")

file(GLOB Fonts_Linux_Source
        "../Notation-Sourcecode/Fonts/Linux/*.h"
        "../Notation-Sourcecode/Fonts/Linux/*.cc")

file(GLOB MIDI_Source
        "../Notation-Sourcecode/MIDI/*.h"
        "../Notation-Sourcecode/MIDI/*.cc")

file(GLOB MusicXML_Source
        "../Notation-Sourcecode/MusicXML/*.h"
        "../Notation-Sourcecode/MusicXML/*.cc")

file(GLOB Notation_Source
        "../Notation-Sourcecode/Notation/*.h"
        "../Notation-Sourcecode/Notation/*.cc")

file(GLOB Notation_Beaming_Source
        "../Notation-Sourcecode/Notation/Beaming/*.h"
        "../Notation-Sourcecode/Notation/Beaming/*.cc")

file(GLOB Notation_BeamingOld_Source
        "../Notation-Sourcecode/Notation/BeamingOld/*.h"
        "../Notation-Sourcecode/Notation/BeamingOld/*.cc")

file(GLOB Notation_Drawable_Source
        "../Notation-Sourcecode/Notation/Drawable/*.h"
        "../Notation-Sourcecode/Notation/Drawable/*.cc")

file(GLOB Notation_Items_Source
        "../Notation-Sourcecode/Notation/Items/*.h"
        "../Notation-Sourcecode/Notation/Items/*.cc")

file(GLOB Notation_MarkPlacement_Source
        "../Notation-Sourcecode/Notation/MarkPlacement/*.h"
        "../Notation-Sourcecode/Notation/MarkPlacement/*.cc")

file(GLOB Notation_NoteSpacer_Source
        "../Notation-Sourcecode/Notation/NoteSpacer/*.h"
        "../Notation-Sourcecode/Notation/NoteSpacer/*.cc")

file(GLOB Notation_SpatialPoint_Source
        "../Notation-Sourcecode/Notation/SpatialPoint/*.h"
        "../Notation-Sourcecode/Notation/SpatialPoint/*.cc")

file(GLOB Score_Source
        "../Notation-Sourcecode/Score/*.h"
        "../Notation-Sourcecode/Score/*.cc")

file(GLOB Score_Linux_Source
        "../Notation-Sourcecode/Score/Linux/*.h"
        "../Notation-Sourcecode/Score/Linux/*.cc")

add_library(notation
            ${Backend_Source}
            ${Backend_Linux_Source}
            ${Commands_Source}
            ${Config_Source}
            ${Core_Source}
            ${EventBus_Source}
            ${File_Source}
            ${Fonts_Source}
            ${Fonts_Linux_Source}
            ${MIDI_Source}
            ${MusicXML_Source}
            ${Notation_Source}
            ${Notation_Beaming_Source}
            ${Notation_BeamingOld_Source}
            ${Notation_Drawable_Source}
            ${Notation_Items_Source}
            ${Notation_MarkPlacement_Source}
            ${Notation_NoteSpacer_Source}
            ${Notation_SpatialPoint_Source}
            ${Score_Source}
            ${Score_Linux_Source})

target_link_libraries(notation PRIVATE
       Qt6::Core)
