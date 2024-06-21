import * as Juce from "./juce/index.js";

console.log("JUCE frontend library successfully imported");

window.__JUCE__.backend.addEventListener("exampleevent", (objectFromBackend) => { console.log(objectFromBackend); });

const data = window.__JUCE__.initialisationData;

document.getElementById("vendor").innerText = data.vendor;
document.getElementById("pluginName").innerText = data.pluginName;
document.getElementById("pluginVersion").innerText = data.pluginVersion;

const nativeFunction = Juce.getNativeFunction("nativeFunction");

document.addEventListener("DOMContentLoaded", () => {
  const button = document.getElementById("nativeFunctionButton");
  button.addEventListener("click", () => {
    nativeFunction("one", 2, null).then(result => {
      console.log(result);
    });
  });

  const emitEventButton = document.getElementById("emitEventButton");
  let emittedCount = 0;
  emitEventButton.addEventListener("click", () => {
    emittedCount++;
    window.__JUCE__.backend.emitEvent("exampleJavaScriptEvent", { emittedCount : emittedCount });
  });

  const slider = document.getElementById("gainSlider");
  slider.oninput = function() {
    const sliderState = Juce.getSliderState("GAIN");
    sliderState.setNormalisedValue(this.value);
  }
});

