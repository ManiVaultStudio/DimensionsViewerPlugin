cmake_minimum_required(VERSION 3.17)

set(PROJECT "DimensionsViewerPlugin")

PROJECT(${PROJECT})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP /permissive- /Zc:__cplusplus")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /NODEFAULTLIB:LIBCMT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

find_package(OpenGL REQUIRED)
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

# Check if the directory to the ManiVault installation has been provided
if(NOT DEFINED MV_INSTALL_DIR)
    set(MV_INSTALL_DIR "" CACHE PATH "Directory where ManiVault is installed")
    message(FATAL_ERROR "Please set MV_INSTALL_DIR to the directory where ManiVault is installed")
endif()
file(TO_CMAKE_PATH ${MV_INSTALL_DIR} MV_INSTALL_DIR)

set(PLUGIN
    src/DimensionsViewerPlugin.h
    src/DimensionsViewerPlugin.cpp
    src/SpecSynchronizer.h
    src/SpecSynchronizer.cpp
)

set(MODEL
    src/LayersModel.h
    src/LayersModel.cpp
    src/Layer.h
    src/Layer.cpp
)

set(ACTIONS
    src/ChannelAction.h
    src/ChannelAction.cpp
    src/SettingsAction.h
    src/SettingsAction.cpp
    src/DimensionsAction.h
    src/DimensionsAction.cpp
    src/SubsamplingAction.h
    src/SubsamplingAction.cpp
    src/ChannelStylingAction.h
    src/ChannelStylingAction.cpp
    src/LayersAction.h
    src/LayersAction.cpp
)

set(WIDGETS
    src/DimensionsViewerWidget.h
    src/DimensionsViewerWidget.cpp
)

set(WEB
    src/res/web/DimensionsViewer.html
    src/res/web/DimensionsViewer.js
)

set(AUX
    src/ResourcesDimensionsViewer.qrc
    src/DimensionsViewerPlugin.json
)

set(SOURCES ${PLUGIN} ${ACTIONS} ${MODEL} ${WIDGETS})

set(PLUGIN_MOC_HEADERS
    src/DimensionsViewerPlugin.h
    src/DimensionsViewerWidget.h
    src/SpecSynchronizer.h
)

source_group(Plugin FILES ${SOURCES})
source_group(Actions FILES ${ACTIONS})
source_group(Model FILES ${MODEL})
source_group(Widget FILES ${WIDGETS})
source_group(Web FILES ${WEB})
source_group(Aux FILES ${AUX})

add_library(${PROJECT} SHARED ${SOURCES} ${WEB} ${AUX} ${WEB} ${WIDGETS})

qt_wrap_cpp(DIMENSIONSVIEWERPLUGIN_MOC ${PLUGIN_MOC_HEADERS} TARGET ${PROJECT})
target_sources(${PROJECT} PRIVATE ${DIMENSIONSVIEWERPLUGIN_MOC})

target_include_directories(${PROJECT} PRIVATE "${MV_INSTALL_DIR}/$<CONFIGURATION>/include/")

target_compile_features(${PROJECT} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT} PRIVATE Qt6::WebEngineWidgets)

set(MV_LINK_PATH "${MV_INSTALL_DIR}/$<CONFIGURATION>/lib")
set(PLUGIN_LINK_PATH "${MV_INSTALL_DIR}/$<CONFIGURATION>/$<IF:$<CXX_COMPILER_ID:MSVC>,lib,Plugins>")
set(MV_LINK_SUFFIX $<IF:$<CXX_COMPILER_ID:MSVC>,${CMAKE_LINK_LIBRARY_SUFFIX},${CMAKE_SHARED_LIBRARY_SUFFIX}>)

set(MV_LINK_LIBRARY "${MV_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}MV_Public${MV_LINK_SUFFIX}")
set(POINTDATA_LINK_LIBRARY "${PLUGIN_LINK_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}PointData${MV_LINK_SUFFIX}") 

target_link_libraries(${PROJECT} PRIVATE "${MV_LINK_LIBRARY}")
target_link_libraries(${PROJECT} PRIVATE "${POINTDATA_LINK_LIBRARY}")

install(TARGETS ${PROJECT}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${MV_INSTALL_DIR}/$<CONFIGURATION>
)

# Automatically set the debug environment (command + working directory) for MSVC
if(MSVC)
	set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${MV_INSTALL_DIR}/debug,${MV_INSTALL_DIR}/release>)
    set_property(TARGET ${PROJECT} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${MV_INSTALL_DIR}/debug/ManiVault Studio.exe","${MV_INSTALL_DIR}/release/ManiVault Studio.exe">)
endif()
