# Updated Build Directions

### 20250901 Zach Mahan

### Building VXL 1.18.0 - Certain compiler flags are necessary for the build

`mkdir build && cd build`

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
  -DBUILD_PRIP=OFF \
  -DCMAKE_CXX_STANDARD=98

```

`make -k`

### Building Shock

`cmake .. -DCMAKE_PREFIX_PATH=/home/zmahan/libraries_src/vxl-1.18.0/build`
^ note: or wherever it lives on your machine
