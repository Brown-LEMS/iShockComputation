# Updated Build Directions

### 20250901 Zach Mahan

### Building VXL 1.18.0 - Certain compiler flags are necessary for the build
- follow build instructions @ the top of the README in the v1.18.0-patch branch of [this VXL fork](https://github.com/zachMahan64/vxl/tree/v1.18.0-patch)
- VXL build instructions for that are also available below:
- `mkdir build && cd build`
- Required for building contrib package in VXL:
    - Fedora: `sudo dnf install libgeotiff-devel`
    - Ubuntu/Debian: `sudo apt install libgeotiff-dev`
    - Arch: `sudo pacman -S libgeotiff`
    - MacOS: `brew install libgeotiff`
```
cmake .. \        
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_CONTRIB=ON \
  -DBUILD_TESTING=OFF \
  -DVXL_FORCE_B3P_EXPAT=ON \
  -DBUILD_RPL=OFF \
  -DBUILD_CORE_VIDEO=OFF \
  -DBUILD_CUL=OFF \
  -DBUILD_DOCUMENTATION=OFF \
  -DBUILD_EXAMPLES=OFF \
  -DBUILD_FOR_VXL_DASHBOARD=OFF \
  -DCMAKE_CXX_STANDARD=98 \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```
- ^ note: environment config for building VXL 1.18.0 on Mac/Linux with modern clang/clang++
- ^ if building on MacOS, try adding `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` if CMake config fails
- `make -k -j$(nproc)` 
    - note: `-k` -> to continue even where some targets fail to build
    - note: `-j$(nproc)` -> utiilize all cores cuz compilation takes forever, full build will fail but it builds what we need
    - ^ if building on MacOS, try adding `-i` to force VXL build through stricter compilation with Apple clang

### Building Shock
- `cmake .. -DCMAKE_PREFIX_PATH=/path/to/vxl/build`
    - note: or wherever it lives on your machine, necessary for linking to vxl
    - ^ if building on MacOS, try adding `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` if CMake config fails
- `make -j$(nproc)` or `make`
