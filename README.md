# Shock Graph
This is the package for intrinsic shock computation, extracted from Maruthi's version in lemsvxl.
The code has been tested on Brown University Oscars CCV server (Linux Red Hat).

## Dependency
[VXL-1.18.0-patch](https://github.com/C-H-Chien/vxl) is the only dependency. Note that the patch version is massively different from the original VXL version 1.18.0 for the support of the shock graph code. Please follow the [build instructions](https://github.com/C-H-Chien/vxl?tab=readme-ov-file#building-on-brown-ccv-oscars) to get VXL ready for the use of the shock graph code.

## Build and Compilation
Once VXL is ready, follow the standard build process to build the shock graph code:
```
mkdir build && cd build
ccmake -DVXL_DIR=/path/to/your/vxl-1.18.0-patch/built/folder
make -j{nproc}
```
where `/path/to/your/vxl-1.18.0-patch/built/folder` is the directory where your vxl-1.18.0-patch is built. Note that `{nproc}` can be any integer dependening on the number of (CPU) cores in use, but it is recommended to use no more than 2 to avoid compilation conflict from parallel threads.

## Usage
### Toy Example


