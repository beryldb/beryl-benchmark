<p align="center">
  <img src="https://static.beryl.dev/smaller.png">
</p>

# Beryl Benchmark

<a target="_blank" href="https://github.com/beryldb/beryl-benchmark/actions"><img src="https://github.com/beryldb/beryl-benchmark/workflows/Ubuntu%20build/badge.svg?2"></a>
<a target="_blank" href="https://github.com/beryldb/beryl-benchmark/actions"><img src="https://github.com/beryldb/beryl-benchmark/workflows/macOS%20build/badge.svg?2"></a>
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Mailing List](https://img.shields.io/badge/email-google%20groups-4285F4 "beryldb@googlegroups.com")](https://groups.google.com/g/beryldb)
[![Twitter](https://img.shields.io/twitter/follow/beryldb?color=%23179CF0&logo=twitter&style=flat-square "@beryldb on Twitter")](https://twitter.com/beryldb)
[![Discord Server](https://img.shields.io/discord/823028202318200912?color=7289da&logo=discord "Discord Server")](https://discord.gg/GRCEuMdYRt)
<br>

## What is Beryl-benchmark?

beryl-benchmark is a benchmark client for [BerylDB](https://github.com/beryldb/beryldb). 
Benchmark is used to run massive tests inside a BerylDB instance.

Beryl-benchmark is supported in MacOS, BSD and Linux systems.


## Installation

Clone latest stable version:

```
git clone --branch 1.0 https://github.com/beryldb/beryl-benchmark.git --depth=5
cd beryl-benchmark/
```

You may now build Beryl-benchmark:

```
./configure
make -j2 install
```

## Usage

```
Usage: ./beryl-cli <arguments>

     --login <username>,          Login with a different username.
     --host <host>,               Connect to provided host.
     --password <password>,       Use a different password than default.
     --port <int>,                Use a differnt port than 6378
     --times <int>,               Repeat test X times.
     --use <1, 100>,              Use a different select than 1.
     --runs <int>,                Cycles to run tests on. 
     --length <int>,              Length of keys to use.
     --tests <tests>,             Tests: incr,get,set,lpush,ldel,hset,hdel,vpush
     --version,                   Display version and exit.
```

## Running

By default, benchmark will run 100 operations on these tests: **INCR, GET, SET,
VPUSH, HSET, LDEL, DECR, HDEL**.

```
./beryl-bench

Benchmark                      | Operations | Time elapsed (s)
―――――――――――――――――――――――――――――― | ―――――――――― | ――――――――――
INCR                           | 100        | 1.048768  
GET                            | 100        | 1.045704  
SET                            | 100        | 1.049898  
VPUSH                          | 100        | 1.043575  
HSET                           | 100        | 1.042865  
LDEL                           | 100        | 1.042550  
DECR                           | 100        | 1.047340  
HDEL                           | 100        | 1.050391 
```

## Examples

In the example below, we test all defined operations on 100 operations
in each cycle:

```
bench:~/work/api/bench$ ./beryl-bench --runs 100

Benchmark                      | Operations | Time elapsed (s)
―――――――――――――――――――――――――――――― | ―――――――――― | ――――――――――
INCR                           | 100        | 1.038436  
GET                            | 100        | 1.045289  
SET                            | 100        | 1.047728  
VPUSH                          | 100        | 1.048326  
HSET                           | 100        | 1.049096  
LDEL                           | 100        | 1.046880  
DECR                           | 100        | 1.049097  
HDEL                           | 100        | 1.047264  

Benchmark finished : 8.496737s
```

In the example below, we test get and set for 3 times running 10 operations
in each cycle:

```
bench:~/work/api/bench$ ./beryl-bench --times 3 --tests get,set --runs 10

Benchmark                      | Operations | Time elapsed (s)
―――――――――――――――――――――――――――――― | ―――――――――― | ――――――――――
GET                            | 10         | 0.093327  
SET                            | 10         | 0.095258  
GET                            | 10         | 0.095570  
SET                            | 10         | 0.095704  
GET                            | 10         | 0.095521  
SET                            | 10         | 0.095568  

Benchmark finished : 0.670838s
```

<br>

## External Links

* [Documentation](https://docs.beryl.dev/cli/installation)
* [GitHub](https://github.com/beryldb/beryl-cli)
* [Support/Discord](https://discord.gg/GRCEuMdYRt)
* [Twitter](https://twitter.com/beryldb)
