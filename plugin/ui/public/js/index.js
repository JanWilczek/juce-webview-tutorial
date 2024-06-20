import * as Juce from "./juce/index.js";

console.log("JUCE frontend library successfully imported");

const _ = window.__JUCE__.backend.addEventListener("exampleevent", (objectFromBackend) => { console.log(objectFromBackend); });

const data = window.__JUCE__.initialisationData;

document.getElementById("vendor").innerText = data.vendor;
document.getElementById("pluginName").innerText = data.pluginName;
document.getElementById("pluginVersion").innerText = data.pluginVersion;
