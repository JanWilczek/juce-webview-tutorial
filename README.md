<div align="center">

<img src="docs/logo.webp" width="512px" />

# 🎹 JUCE 8 WebView UI Plugin Tutorial 🎹

![Cmake workflow success badge](https://github.com/JanWilczek/juce-webview-tutorial/actions/workflows/cmake.yml/badge.svg)

</div>

Welcome to the JUCE 8 WebView UI Plugin Tutorial! This repository accompanies a video tutorial series on creating a WebView UI plugin using JUCE 8.

## 🚀 Getting Started

After cloning the repo locally, you can proceed with the usual CMake workflow.

In the main repo directory execute

```bash
cmake -S . -B build
cmake --build build
```

Alternatively, you can use bundled CMake presets:

```bash
cmake --preset default # uses the Ninja build system
cmake --build build
ctest --preset default
```

Existing presets are `default`, `release`, `vs` (for Visual Studio), and `Xcode`.

The first run will take the most time because the dependencies (CPM, JUCE, and googletest) need to be downloaded.

To run clang-format on every commit, in the main directory execute

```bash
pre-commit install
```

(for this you may need to install `pre-commit` with `pip`: `pip install pre-commit`).
