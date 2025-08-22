# WinXInputFeeder

Windows software for emulating up to 4 Xbox controllers from different keyboards.  
See [screenshots here](https://github.com/rtk0c/WinXInputFeeder/discussions/1).

## Installation

1. Download and install ViGEmBus from https://github.com/nefarius/ViGEmBus/releases/tag/v1.22.0
2. Download the latest version of WinXInputFeeder from [releases](https://github.com/rtk0c/WinXInputFeeder/releases)
   - This is a standalone .exe file, you can place it anywhere to run, no installation required
3. Configure as you like, per [Configuration Guide](#configuration-guide)

## Configuration Guide

TODO

## Building

If you wish to build WinXInputFeeder from source rather than using the prebuilt binary.
1. Install Visual Studio 2022 with the Desktop C++ Development workload. WinXInputFeeder uses some C++20 features, hence why the MSVC shipped with 2022 is needed.
1. Install vcpkg with integration for MSBuild/Visual Studio: [official documentation](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-msbuild?pivots=shell-powershell#1---set-up-vcpkg) (only step 1 from this page is relevant)
2. Install vcpkg packages
   - `tomlplusplus`
   - `imgui[win32-binding,dx11-binding]`
3. Open `WinXInputFeeder.sln`, select your desired target architecture, and build
