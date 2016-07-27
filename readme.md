# Dll Hijack Source Template

This is a *template*. To use it:
1. Copy `main_dll\src\DoPatch.sample.cpp` to `main_dll\src\DoPatch.cpp`.
2. `git submodule update --init --recursive`.
3. Code.

This project was created using Visual Studio 2015. You may get it working with previous version of Visual Studio.

Other compiler such as `gcc` is not gonna work I believe; some api will be different.


# Export Functions
By default, it will export only `version.dll` when compile for x64 arch, however it for x86 it will export both  `version.dll` and `lpk.dll` for Windows XP legacy support.

You can change its export in `_EXP_Control.h`.

Or you can disable export, by `#define __NO_EXP`.


# License

Released under the MIT license - http://opensource.org/licenses/MIT