if not exist "./bin/windows/" (
    mkdir "./bin/windows"
)

g++ main.cpp -o bin/windows/kml-town