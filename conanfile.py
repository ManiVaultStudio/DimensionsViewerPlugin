import os
import pathlib
import shutil
from conans import ConanFile, CMake, tools


class DimensionsViewerPlugin(ConanFile):
    # package name to be used in dependencies
    name = "DimensionsViewerPlugin"
    # On release branch change this to the release number
    version = "latest"
    # This is the license for the recipe not the code. 
    # Currently the project close source if that changes choose an appropriate license
    license = "proprietary software no public license"
    url = "https://github.com/hdps/DimensionsViewerPlugin"
    description = "Viewer for inspecting data dimensions"
    # Settings used to distinguish different builds
    settings = {"os": None, "build_type": None, "compiler": None, "arch": None}
    # Only build a shared library (ie no static)
    options = {"shared": [True],  "fPIC": [True]}
    default_options = {"shared": True, "fPIC": True}
    # exports - Which files are copied to the build directory
    exports = "*"
    # cmake_paths generator produces a conan_paths.cmake
    # file that can be passed to cmake to allow it to find the
    # conan installed dependencies as CMAKE_TOOLCHAIN_FILE
    generators = ("cmake_paths")
    default_user = "conanuser"
    default_channel = "testing"
    
    requires = (
        "qt/5.15.1@lkeb/stable",
        "hdps-core/latest@lkeb/stable",
    )
    
    scm = {
        "type": "git",
        "subfolder": "hdps/DimensionsViewerPlugin",
        "url": "auto",
        "revision": "auto"
    }

    # Remove runtime and use always default (MD/MDd)
    def configure(self):
        if self.settings.compiler == "Visual Studio":
            del self.settings.compiler.runtime
            
    def config_options(self):
        if self.settings.os == 'Windows':
            del self.options.fPIC
            
    def _configure_cmake(self, build_type):
        qtpath = pathlib.Path(self.deps_cpp_info["qt"].rootpath)
        print("Qt5 rootpath ", qtpath)
        qt_root = str(list(qtpath.glob('**/Qt5Config.cmake'))[0].parents[3])
        
        cmake = CMake(self, build_type=build_type)
        # Default source_folder is current directory
        cmake.definitions["CMAKE_PROJECT_DimensionsViewerPlugin_INCLUDE"] = os.path.join(self.build_folder, "conan_paths.cmake")
        cmake.definitions["CMAKE_PREFIX_PATH"] = qt_root
               
        cmake.configure(source_folder = hdps/DimensionsViewerPlugin)
        cmake.verbose = True
        return cmake
    
    def build(self):
        hdps_pkg_root= self.deps_cpp_info["hdps-core"].rootpath 
        print("HDPS core at ", hdps_pkg_root)
        
        if not os.environ.get('HDPS_INSTALL_DIR', None):
            os.environ['HDPS_INSTALL_DIR'] = hdps_pkg_root
        self.install_dir = os.environ['HDPS_INSTALL_DIR']

        cmake_debug = self._configure_cmake('Debug')
        cmake_debug.build()
        
        cmake_release = self._configure_cmake('Release')
        cmake_release.build()

    def package(self):
        print('Packaging install dir: ', self.install_dir)
        self.copy(pattern="*", src=self.install_dir)

    def package_info(self):
        self.cpp_info.debug.libdirs = ['Debug/lib']
        self.cpp_info.debug.bindirs = ['Debug/Plugins', 'Debug']
        self.cpp_info.debug.includedirs = ['Debug/include', 'Debug']
        self.cpp_info.release.libdirs = ['Release/lib']
        self.cpp_info.release.bindirs = ['Release/Plugins', 'Release']
        self.cpp_info.release.includedirs = ['Release/include', 'Release']