## Build
`mkdir build & cd build` \
`cmake .. ` if windows: `-DCMAKE_TOOLCHAIN_FILE=[vcpkg path]\scripts\buildsystems\vcpkg.cmake`\
`cmake --build .`

## Run
`mpiexec.exe -n 2 Exp.exe <number of terms in a row> <accuracy>`