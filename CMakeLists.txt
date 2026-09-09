cmake_minimum_required(VERSION 3.22)
project(SampleRateDestroyer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.12
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(SampleRateDestroyer
    COMPANY_NAME "Independent"
    PRODUCT_NAME "Sample Rate Destroyer"
    BUNDLE_ID "com.independent.sampleratedestroyer"
    PLUGIN_MANUFACTURER_CODE SRDR
    PLUGIN_CODE SRD1
    FORMATS AU VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    AU_MAIN_TYPE kAudioUnitType_Effect
)

juce_generate_juce_header(SampleRateDestroyer)

target_sources(SampleRateDestroyer PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

target_compile_definitions(SampleRateDestroyer PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(SampleRateDestroyer PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

if(APPLE)
    set_target_properties(SampleRateDestroyer PROPERTIES
        XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "11.0"
    )
endif()
