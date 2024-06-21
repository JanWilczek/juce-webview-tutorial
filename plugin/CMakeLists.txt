cmake_minimum_required(VERSION 3.22)

# Version is needed by JUCE.
project(AudioPlugin VERSION 0.1.0)

# Adding a directory with the library/application name as a subfolder of the
# include folder is a good practice. It helps avoid name clashes later on.
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/JuceWebViewTutorial")

# Adds a plugin target (that's basically what the Projucer does).
juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME WolfSound
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE WFSD
    PLUGIN_CODE JWVT
    FORMATS VST3 Standalone
    PRODUCT_NAME "JuceWebViewTutorial"
    # This will force JUCE to look for the WebView2 NuGet package on Windows
    NEEDS_WEBVIEW2 TRUE
)

get_target_property(PRODUCT_NAME ${PROJECT_NAME} JUCE_PRODUCT_NAME)
get_target_property(COMPANY_NAME ${PROJECT_NAME} JUCE_COMPANY_NAME)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  JUCE_PRODUCT_NAME="${PRODUCT_NAME}"
  JUCE_COMPANY_NAME="${COMPANY_NAME}"
  JUCE_PRODUCT_VERSION="${PROJECT_VERSION}")

# Sets the source files of the plugin project.
set(SOURCES
        source/PluginEditor.cpp
        source/PluginProcessor.cpp)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCES}
        ${INCLUDE_DIR}/PluginEditor.h
        ${INCLUDE_DIR}/PluginProcessor.h
)

# Sets the include directories of the plugin project.
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Mark JUCE headers as system headers to suppress warnings in them
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${JUCE_MODULES_DIR})

# Links to all necessary dependencies. The present ones are recommended by JUCE.
# If you use one of the additional modules, like the DSP module, you need to specify it here.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        # JUCE_WEB_BROWSER=1 is important for using WebBrowserComponent
        JUCE_WEB_BROWSER=1
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        # This will enable WebView2 as the WebView backend on Windows
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
)

set_source_files_properties(${SOURCES} PROPERTIES COMPILE_OPTIONS "${CXX_PROJECT_WARNINGS}")

# In Visual Studio this command provides a nice grouping of source files in "filters".
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Folder where web UI data reside
set(UI_FILES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ui/public")

# Copy JUCE frontend library to plugin UI files
file(COPY "${JUCE_MODULES_DIR}/juce_gui_extra/native/javascript/" DESTINATION "${UI_FILES_SOURCE_DIR}/js/juce/")

# Copy WebView UI files
get_target_property(PLUGIN_OUTPUT_DIR ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
set(UI_FILES_DEST_DIR "${PLUGIN_OUTPUT_DIR}/public")

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${UI_FILES_SOURCE_DIR} ${UI_FILES_DEST_DIR}
  COMMENT "Copying UI files to plugin's output directory"
)
