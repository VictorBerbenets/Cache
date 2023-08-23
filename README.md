# LFU cache implementation on C++
### about
In this repository I implemented LFU(a caching algorithm in which the least
frequently used cache block is removed whenever the cache is overflowed.)
cache and compared it with another "perfect cache" algorithm.
"Perfect cache" algorithm was based on idea that we already know the future(full 
incoming data package).
##Requirements
**cmake** version must be 3.15 or higher
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
### 1) 
```
cd tests/
```
### 2) You can choose two options:
#### 1) Generate tests for Perfect cache
#### 2) Generate tests for LFU cache
For the first one do:
```
cmake -S ./ -B build/ -DPERFECT=ON
```
For the second one do:
```
cmake -S ./ -B build/ -DPERFECT=OFF
```
### 3) To run tests do:
```
bash ./test_runner.sh mode N
```
Instead **mode** you must write **lfu** or **perfect** (without quotes) keyword. **N** - number of tests
you want to generate and run for chosen cache. There's limit: N <= 150. If you enter greater
number it sets in 150.
## How to run compare program:
```
cd Compare/
bash ./compare N
```
Instead **N** you must enter number of files which will be generated and compared lfu 
and perfect caches. After that you can see a generated file called
`comparing.txt` with differences.

