if not exist "./bin/windows/" (
    mkdir "./bin/windows"
)

g++ main.cpp -o bin/windows/kml-town
:: Please bundle "libwinpthread-1.dll", "libstdc++-6.dll" and "libgcc_s_seh-1.dll"
:: with the executable for Windows