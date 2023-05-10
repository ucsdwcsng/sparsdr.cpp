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

To install the executable, run

```bash
sudo make install
```

## Running the SparSDR Filter Application

In the current release, there is only a filter application that reads from a file and filters out selected number of bins.

```bash
Usage: sparsdr_filter <fft_size> <start_bin> <stop_bin> <input_file> <output_file>
```

1. **fft_size**: even numbered FFT size
2. **start_bin**: start bin for filtering
3. **stop_bin**: stop bin for filtering
4. **input_file**: input filename (complex interleaved floats)
5. **output_file**: output filename (complex interleaved floats)

The executable will channelize the input file, pick out the relevant bins, and then reconstruct the lower rate I/Q samples.

> **NOTE:**  `sparsdr_filter` currently only supports bin ranges that are on one side of the fft window. That is, the seelcted window should exclusively have only positive or negative frequencies.

**Example**: To get the 2402 MHz BLE channel from a 100 MHz capture of the ISM band centered at 2450 MHz, the following command can be used:

```bash
sparsdr_filter 1000 510 530 <input_file> <output_file>
```

The output file should be interpreted at a sample rate of 2 MHz.

## Tests

From the build directory, run:

```bash
make test
```
