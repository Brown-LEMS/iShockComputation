# Updated Build Directions

### 20250901 Zach Mahan

### Building VXL 1.18.0 - Certain compiler flags are necessary for the build

- `mkdir build && cd build`

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
- ^ note: environment config for building VXL 1.18.0 on Linux with modern clang/clang++ v20.1.8


- `make -k -j$(nproc)` 
    - note: `-k` -> to continue even where some targets fail to build
    - note: `-j$(nproc)` -> utiilize all cores cuz compilation takes forever, full build will fail but it builds what we need

note: I did have to manually modify some of the VXL src to get it to compile properly (no functionality changes)

### Building Shock

- `cmake .. -DCMAKE_PREFIX_PATH=/path/to/vxl/build`
    - note: or wherever it lives on your machine, necessary for linking to vxl
- `make -j$(nproc)` or `make`
