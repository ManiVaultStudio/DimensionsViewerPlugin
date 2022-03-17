cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "DimensionsViewerPlugin")

PROJECT(${PROJECT})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /NODEFAULTLIB:LIBCMT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Widgets WebEngineWidgets REQUIRED)

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
    src/PluginAction.h
    src/PluginAction.cpp
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
    src/Resources.qrc
    src/DimensionsViewerPlugin.json
)

set(SOURCES ${PLUGIN} ${ACTIONS} ${MODEL} ${WIDGETS})

source_group(Plugin FILES ${SOURCES})
source_group(Actions FILES ${ACTIONS})
source_group(Model FILES ${MODEL})
source_group(Widget FILES ${WIDGETS})
source_group(Web FILES ${WEB})
source_group(Aux FILES ${AUX})

include_directories("$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/include/")

add_library(${PROJECT} SHARED ${SOURCES} ${WEB} ${AUX} ${WEB} ${WIDGETS})

target_compile_features(${PROJECT} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT} Qt5::Widgets)
target_link_libraries(${PROJECT} Qt5::WebEngineWidgets)

if(MSVC)
	set(LIB_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
else()
	set(LIB_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

target_link_libraries(${PROJECT} "$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}HDPS_Public${LIB_SUFFIX}")
target_link_libraries(${PROJECT} "$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/lib/${CMAKE_SHARED_LIBRARY_PREFIX}PointData${LIB_SUFFIX}")

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
	"$<TARGET_FILE:${PROJECT}>"
	"$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/Plugins/$<TARGET_FILE_NAME:${PROJECT}>"
)