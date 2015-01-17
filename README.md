# Nodal

[![Build Status](https://travis-ci.org/nodal/nodal.svg?branch=master)](https://travis-ci.org/nodal/nodal)

Nodal is a small C++ library offering a generic foundation for development of
applications based on
[dataflow](http://en.wikipedia.org/wiki/Dataflow_programming) and
[flow-based](http://en.wikipedia.org/wiki/Flow-based_programming) programming.

Nodal is based on the Boost Graph Library and makes heavy use of C++ 11
features.

## Building

Dependencies:

  * boost >= 1.55.0

```
git clone https://github.com/nodal/nodal.git
cd nodal
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE={Debug|Release} -DBUILD_EXAMPLES={ON|OFF} ..
make
```

## Documentation

The library is not complete and lacks documentation. See the
[examples](https://github.com/nodal/nodal/tree/master/examples)
directory for usage examples.

## License

The MIT License (MIT)

Copyright (c) 2015 Fabio Massaioli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
