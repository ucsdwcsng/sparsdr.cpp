# sparsdr.cpp

C++ implementation of SparSDR. Ref: [*Khazraee, M., Guddeti, Y., Crow, S., Snoeren, A.C., Levchenko, K., Bharadia, D. and Schulman, A., 2019, June. Sparsdr: Sparsity-proportional backhaul and compute for sdrs. In Proceedings of the 17th Annual International Conference on Mobile Systems, Applications, and Services (pp. 391-403).*](https://cseweb.ucsd.edu/~schulman/docs/mobisys19-sparsdr.pdf)

## Build instructions

### On Debian-based distros

```bash
sudo apt-get install libfftw3-dev cmake pkg-config libliquid-dev
mkdir build
cd build
cmake ..
make -j4
```

## Tests

From the build directory, run:

```bash
make test
```
