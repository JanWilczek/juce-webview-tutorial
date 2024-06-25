# Mandatory line, sets the minimum version of CMake that should be used with this repository.
# I specified 3.22 because I trust it. However, currently I have 3.26 installed on my machine.
# To verify your version run
# $ cmake --version
cmake_minimum_required(VERSION 3.22)

# Sets a few variables, like PROJECT_NAME
project(Juce8WebViewTutorial)

# Safe choice
set(CMAKE_CXX_STANDARD 20)

# I like to download the dependencies to the same folder as the project.
# If you want to install them system wide, set CPM_SOURCE_CACHE with the path to the dependencies
# either as an environment variable or pass it to the cmake script with -DCPM_SOURCE_CACHE=<path>.
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Downloads CPM if not already downloaded. CPM is an easy-to-use package manager nicely integrated with CMake.
include(cmake/cpm.cmake)

# This commands downloads AND configures JUCE. It sets up some variables, like JUCE_SOURCE_DIR.
CPMAddPackage(
    NAME JUCE
    GIT_TAG 8.0.0
    VERSION 8.0.0
    GITHUB_REPOSITORY juce-framework/JUCE
    SOURCE_DIR ${LIB_DIR}/juce
)

# Install Microsoft.Web.WebView2 NuGet package to allow WebViews on Windows
if (MSVC)
  message(STATUS "Setting up WebView dependencies")
  execute_process(COMMAND pwsh -NoProfile -File scripts/DownloadWebView2.ps1
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  RESULT_VARIABLE DOWNLOAD_WEBVIEW2_RESULT)

  if (NOT DOWNLOAD_WEBVIEW2_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to download Microsoft.Web.WebView2 NuGet package. Result: ${DOWNLOAD_WEBVIEW2_RESULT}")
  endif()
endif()

# Enables strict warnings and treats warnings as errors.
# This needs to be set up only for your projects, not 3rd party
if (MSVC)
set(CXX_PROJECT_WARNINGS "/W4;/WX;/wd4820;/wd4514")
else()
set(CXX_PROJECT_WARNINGS "-Wall;-Wextra;-Wpedantic")
endif()

# Adds all the targets configured in the "plugin" folder.
add_subdirectory(plugin)

