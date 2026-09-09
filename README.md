# Sample Rate Destroyer

A deliberately simple JUCE audio effect with exactly one user control: **Sample Rate**.

- 0% = ~350 Hz internal sample-and-hold rate: extreme digital degradation
- 100% = true bypass / original signal
- AU + VST3 only
- No standalone target
- Stereo and mono
- Parameter is automatable and saved with the plugin state

## Build on macOS

Requirements:
- macOS
- Xcode + Command Line Tools
- CMake 3.22+
- Internet access for the first configure, so CMake can fetch JUCE 8.0.12

From this folder:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

JUCE's `COPY_PLUGIN_AFTER_BUILD TRUE` will copy the built AU/VST3 to the standard user plugin locations when supported by the build environment.
