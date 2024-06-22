#pragma once

#include "PluginProcessor.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>

namespace audio_plugin {

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  void resized() override;

private:
  using Resource = juce::WebBrowserComponent::Resource;
  std::optional<Resource> getResource(const juce::String& url) const;
  void nativeFunction(
      const juce::Array<juce::var>& args,
      juce::WebBrowserComponent::NativeFunctionCompletion completion);

  juce::TextButton runJavaScriptButton{"Run some JavaScript"};
  juce::TextButton emitJavaScriptEventButton{"Emit JavaScript event"};
  juce::Label labelUpdatedFromJavaScript{"label",
                                         "To be updated from JavaScript"};

  AudioPluginAudioProcessor& processorRef;

  juce::Slider gainSlider{"gainSlider"};
  juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment;

  juce::ToggleButton bypassButton{"Bypass"};
  juce::AudioProcessorValueTreeState::ButtonAttachment bypassButtonAttachment;

  juce::WebSliderRelay webGainRelay;
  juce::WebSliderParameterAttachment webGainSliderAttachment;
  juce::WebBrowserComponent webView;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
