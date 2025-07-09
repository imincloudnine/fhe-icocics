## Protocol summary

The server holds a private dataset D. A client wants to check if an encrypted query `m` is in D **without revealing m or D**.

### Steps:

1. **Client** encrypts a vector `[m, m, ..., m]` and sends to server.
2. **Server** subtracts encrypted `m` from encrypted dataset `D`, multiplies result with a random mask vector.
3. **Client** decrypts the result: if any slot is 0 → `m` ∈ D.

Mathematically:

```
result = Mask · (Enc(D) - Enc(m))
```

## How to build

Make sure HElib is installed with CMake support:

```bash
git clone https://github.com/homenc/HElib
cd HElib && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/HElib/install
make -j && make install
```

Then clone this project and build:

```bash
cd ~/projects
git clone https://github.com/imincloudnine/fhe-icocics.git
cd fhe-icocics
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$HOME/HElib/install
make
```

## How to run

example:

```bash
./fhe_i 64 42
```

You should see output like:

```
Query preparation time (ms): 1234
Server evaluation time (ms): 1234
verification time (ms): 1234
Membership result: "Found"/"Not Found"
...
```

## Notes

* Default slot count: 64
* BGV scheme with batching; requires slot count ≥ dataset size
* Can switch from HElib to SEAL backend with minor changes
* Designed for benchmarking with circuit-based comparison (e.g. [comparison-circuit-over-fq](https://github.com/iliailia/comparison-circuit-over-fq))
