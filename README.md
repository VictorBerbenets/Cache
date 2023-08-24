# LFU cache implementation on C++
### About
In this project I implemented LFU cache(a caching algorithm in which the least
frequently used cache block is removed whenever the cache is overflowed) and compared it with "perfect cache".
"Perfect cache" is based on idea that we already know the future(full 
incoming data package).
## Requirements
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
The program will be waiting for input data from stdin in such way:
**capacity** **size** number1 number2 ...  
**capacity** - capacity of the cache  
**size** - size of incoming data  
number1 number2 ... - keys  
At the end, the program prints number of hits to std::cout.
## How to run tests:
### 1) 
```
cd tests/
```
### 2) You can choose two options:
#### 1) Generate tests for Perfect cache
```
cmake -S ./ -B build/ -DPERFECT=ON
```
#### 2) Generate tests for LFU cache
```
cmake -s ./ -B build/ -DPERFECT=OFF
```
### 3) To run tests do:
```
bash ./test_runner.sh mode N
```
Instead **mode** you should write **lfu** or **perfect** (without quotes) keyword. **N** - number of tests
you want to generate and run for chosen cache. There's limit: N <= 150. If you enter greater
number, it sets in 150.  
After that you can see created directories:  
**lfu_resources** - if you generated lfu tests  
**perfect_resources** - if you generated perfect tests  
Each of them contains tests/ and answers/ directories.
## How to run compare program:
If you want compare lfu and perfect caches then do:
```
cd Compare/
bash ./compare N
```
Instead **N** you should enter number of files which will be generated. After that you can see a generated file called
`comparing.txt` with differences for each files.

