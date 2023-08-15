#LFU cache implementation on C++
###about
In this repository I implemented LFU(a caching algorithm in which the least
frequently used cache block is removed whenever the cache is overflowed.)
caching algorithm and compared it with another "perfect cache" algorithm.
perfect cache algorithm was based on idea that we already know the future(full 
incoming data package).
##How to build(on Linux)
'''
git clone git@github.com:VictorBerbenets/Cache.git
cd Cache/
cmake -S ./ -B build/
cd build/
make
'''
After that you can run LFU cache:
'''
LFU_Cache/lfu_cache
'''
or you can run "perfect cache":
'''
Perfect_Caching/perfect_cache
'''

##How to run tests:

##How to run compare programm:
