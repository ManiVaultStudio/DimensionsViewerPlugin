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

file(GLOB PLUGIN
	src/Common.h
	src/*Plugin.*
	src/SpecSynchronizer.*
)

file(GLOB MODEL 
	src/Channel.*
	src/Channels.*
	src/Configuration.*
	src/Configurations.*
	src/ConfigurationsModel.*
	src/DifferentialProfile.*
	src/Global.*
	src/Miscellaneous.*
	src/ModelItem.*
	src/Profile.*
	src/Styling.*
)

file(GLOB WIDGETS
	src/ChannelsWidget.*
	src/ChannelWidget.*
	src/ConfigurationWidget.*
	src/DifferentialProfileWidget.*
	src/DimensionsViewerWidget.*
	src/GlobalWidget.*
	src/MiscellaneousWidget.*
	src/ModelItemWidget.*
	src/StylingWidget.*
	src/StylingPushButton.*
)

file(GLOB UIS src/*.ui)
file(GLOB WEB src/res/web/*.html src/res/web/*.js)
file(GLOB JSON src/*.json)
file(GLOB QRC src/*.qrc)

set(SOURCES ${PLUGIN} ${MODEL} ${WIDGETS})

set(WEB ${WEB})
set(AUX ${JSON} ${QRC})

source_group(Plugin FILES ${PLUGIN})
source_group(Model FILES ${MODEL})
source_group(Widgets FILES ${WIDGETS})
source_group(UI FILES ${UIS})
source_group(Web FILES ${WEB})
source_group(Aux FILES ${AUX})

include_directories("$ENV{HDPS_INSTALL_DIR}/$<CONFIGURATION>/include/")

QT5_WRAP_UI(UI_HEADERS ${UIS})

add_library(${PROJECT} SHARED ${SOURCES} ${UIS} ${WEB} ${AUX} ${WEB})

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