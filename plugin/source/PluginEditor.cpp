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
          [](const auto& url) {
            juce::ignoreUnused(url);
            DBG("ResourceProvider called with " + url);
            return std::nullopt;
          })) {
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
}  // namespace audio_plugin
