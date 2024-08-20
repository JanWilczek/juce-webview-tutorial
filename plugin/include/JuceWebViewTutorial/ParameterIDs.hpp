#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace webview_plugin::id {
const juce::ParameterID GAIN{"GAIN", 1};
const juce::ParameterID BYPASS{"BYPASS", 1};
const juce::ParameterID DISTORTION_TYPE{"DISTORTION_TYPE", 1};
}  // namespace webview_plugin::id
