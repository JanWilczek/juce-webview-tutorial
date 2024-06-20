import * as Juce from "./juce/index.js";

console.log("JUCE frontend library successfully imported");

const _ = window.__JUCE__.backend.addEventListener("exampleevent", (objectFromBackend) => { console.log(objectFromBackend); });

