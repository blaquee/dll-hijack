# Dll Hijack Source Template

This is a *template*. Copy `main_dll\src\DoPatch.sample.cpp` to `main_dll\src\DoPatch.cpp` and begin your coding there.

This project was created using Visual Studio 2015. You may get it working with previous version of Visual Studio.

Other compiler such as `gcc` is not recommended; as it will require extra dependencies such as `msys-2.0.dll` or `cygwin1.dll`.. Maybe is just me don't know how to static build it :<

## Setup

    git submodule update --init --recursive

# Export Functions
By default, it will export only `version.dll` when compile for x64 arch, however it for x86 it will export both  `version.dll` and `lpk.dll` for Windows XP legacy support.

You can change its export in `_EXP_Control.h`.


# License

Released under the MIT license - http://opensource.org/licenses/MIT