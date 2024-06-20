#include "JuceWebViewTutorial/PluginEditor.h"
#include <optional>
#include "JuceWebViewTutorial/PluginProcessor.h"
#include "juce_core/juce_core.h"
#include "juce_gui_extra/juce_gui_extra.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      webView(juce::WebBrowserComponent::Options{}.withResourceProvider(
          [this](const auto& url) { return getResource(url); })) {
  juce::ignoreUnused(processorRef);

  addAndMakeVisible(webView);

  // WebBrowserComponent can display any URL
  // webView.goToURL("https://juce.com");

  // This is necessary if we want to use a ResourceProvider
  webView.goToURL(webView.getResourceProviderRoot());

  setResizable(true, true);
  setSize(800, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::resized() {
  webView.setBounds(getBounds().withTrimmedLeft(getWidth() / 2));
}

auto AudioPluginAudioProcessorEditor::getResource(const juce::String& url) const
    -> std::optional<Resource> {
  DBG("ResourceProvider called with " + url);
  return std::nullopt;
}
}  // namespace audio_plugin
