:: -G : specify for what to generate project. Example "Visual Studio 16 2019"
:: -A : platform name. For "Visual Studio 16 2019" are (Win32, x64, ARM, ARM64)
:: -B : build folder

cmake -G "Visual Studio 16 2019" ^
-A x64 ^
-B Builds/win_vs_2019_x64 ^
-D SDL2_INCLUDE_DIR:PATH="Dependencies\SDL2-2.0.12\include" ^
-D SDL2_LIB_PATH:PATH="C:\Users\chetr\Desktop\Portofolio\HumansVsGoblins\Dependencies\SDL2-2.0.12\lib\x64\SDL2.lib" ^
-D SDL2_MAIN_LIB_PATH:PATH="Dependencies\SDL2-2.0.12\lib\x64\SDL2main.lib" ^
-D SDL2_WIN_DLL_PATH:PATH="Dependencies\SDL2-2.0.12\lib\x64\SDL2.dll" ^
-D LIBPNG_INCLUDE_DIR:PATH="Dependencies\libpng\x64\release\include" ^
-D LIBPNG_LIB_PATH:PATH="Dependencies\libpng\x64\release\bin\libpng16.lib" ^
-D LIBPNG_WIN_DLL_PATH:PATH="Dependencies\libpng\x64\release\bin\libpng16.dll" ^
-D LIBPNG_ZLIP_WIN_DLL_PATH:PATH="Dependencies\zlib\x64\release\bin\zlib.dll" ^
-D GENERATE_TESTS:BOOL=OFF ^
-D ACUTEST_INCLUDE_DIR:PATH="Dependencies\acutest\include" ^
-D BUILD_HUMANS_VS_GOBLINS_ENGINE_SHARED:BOOL=OFF ^
.

PAUSE
