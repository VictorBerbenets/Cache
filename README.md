# LFU cache implementation on C++
### about
In this repository I implemented LFU(a caching algorithm in which the least
frequently used cache block is removed whenever the cache is overflowed.)
caching algorithm and compared it with another "perfect cache" algorithm.
"Perfect cache" algorithm was based on idea that we already know the future(full 
incoming data package).
## How to build
```
git clone git@github.com:VictorBerbenets/Cache.git
cd Cache/
cmake -S ./ -B build/
cd build/
cmake --build .
```
After that you can run LFU cache:

```
./lfu
```
or you can run "perfect cache":

```
./perfect
```

## How to run tests:
### 1) cd tests/
### 2) You can choose two options:
#### 2.1) Generate tests for Perfect cache
#### 2.2) Generate tests for LFU cache
For the first one do:
```
cmake -S ./ -B build/ -DPERFECT=ON
```
For the second one do:
```
cmake -S ./ -B build/
```
### 3) To run tests do:
```
bash ./test_runner.sh `mode` `N`
```
Instead **mode** must write **lfu** or **perfec** (without quotes) keywords. **N** - number of tests
you want to generate and run. There's limit: N <= 150. If you enter greater
number it sets in 150.
## How to run compare program:
```
cd Compare/
./compare N
```
Instead **N** you must enter number of files which will be generated and compared lfu 
and perfect caches.

