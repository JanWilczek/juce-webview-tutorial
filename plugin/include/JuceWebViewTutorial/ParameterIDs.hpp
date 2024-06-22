#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace audio_plugin::id {
const juce::ParameterID GAIN{"GAIN", 1};
const juce::ParameterID BYPASS{"BYPASS", 1};
}  // namespace audio_plugin::id