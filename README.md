<div align="center">

<img src="docs/logo.webp" width="512px" />

# 🎹 JUCE 8 WebView UI Plugin Tutorial 🎹

![Cmake workflow success badge](https://github.com/JanWilczek/juce-webview-tutorial/actions/workflows/cmake.yml/badge.svg)

</div>

Welcome to the JUCE 8 WebView UI Plugin Tutorial! This repository accompanies a video tutorial series on creating a WebView UI plugin using JUCE 8.

Feel free to use this project as a starting point for your own projects. For this, you can click "Use this template" button at the top of this page.

## 🚀 Getting Started

This repository uses CMake. It takes care of all the dependencies using [CPM](https://github.com/cpm-cmake/CPM.cmake).

After cloning the repo locally, you can proceed with the usual CMake workflow.

To generate project files execute in the main repo directory

```bash
cmake --preset default # uses the Ninja build system
```

Existing presets are `default`, `release`, `vs` (for Visual Studio), and `Xcode`. Check out _CMakePresets.json_ for details.

The first run will take the most time because the dependencies (CPM and JUCE) need to be downloaded.

To build the project execute in the main repo directory

```bash
cmake --build --preset default # or release, vs, or Xcode
```

### Additional setup

To run clang-format on every commit, in the main directory execute

```bash
pre-commit install
```

(for this, you may need to install `pre-commit` with `pip`: `pip install pre-commit`).

## 📜 License

This project is licensed under the AGPLv3 license according to the JUCE licensing terms.

