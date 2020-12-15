PerfectPrecomputedHashtable.py [![Unlicensed work](https://raw.githubusercontent.com/unlicense/unlicense.org/master/static/favicon.png)](https://unlicense.org/)
==============================
~~[wheel (GHA via `nightly.link`)](https://nightly.link/KOLANICH-libs/PerfectPrecomputedHashtable.py/workflows/CI/master/PerfectPrecomputedHashtable-0.CI-py3-none-any.whl)~~
~~[![GitHub Actions](https://github.com/KOLANICH-libs/PerfectPrecomputedHashtable.py/workflows/CI/badge.svg)](https://github.com/KOLANICH-libs/PerfectPrecomputedHashtable.py/actions/)~~
[![Libraries.io Status](https://img.shields.io/librariesio/github/KOLANICH-libs/PerfectPrecomputedHashtable.py.svg)](https://libraries.io/github/KOLANICH-libs/PerfectPrecomputedHashtable.py)
[![Code style: antiflash](https://img.shields.io/badge/code%20style-antiflash-FFF.svg)](https://codeberg.org/KOLANICH-tools/antiflash.py)

A framework to create scripts to precompute a near-perfect hashtable of small number of objects that can be stored and used in future.

Why not a CLI tool? Because usually a hashtable needs to be matched against objects. So you would still need a bit of own logic.

The hashtable has the following construction:

1. objects are converted into a byte strings
2. a tweakable hash function needing a nonce is applied to objects to get 4-byte values. The function should map all the strings into unique 4-byte integers. We currently use `blake2s` because it is in python standard library.
3. the 4 byte valuea are converted into 2-byte control value
4. the 4 byte values are reduced into a single byte value using one of the reducer finctions.
5. the single byte values are deoffsetted by subtracting the minimum value across the whole dataset.
6. the single byte values are passed through a perfect hash generator (`perfection`) to remap them into a nearly-continious range of indices.
7. the indices are used to populate an array.


`pow` dir contains C++ program brute-forcing nonces (so it is kinda a proof-of-work) in order to minimize the "span" (the distance between maximum and minimum reduced hash) of reduced values (only the nonces mapping keys to distinct values are considered). `strings.cpp` should be populated with the byte strings.

After the optimal nonce and reducer have been chosen, it can be used to produce a python source file with the hashtable and the lookup function.
