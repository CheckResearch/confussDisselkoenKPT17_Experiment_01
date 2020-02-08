# CheckResearch.org [Experiment](https://checkresearch.org/Experiment/View/0e8dc075-4da3-4492-a9df-14a6e7587559)

 Publication ["Prime+Abort: A Timer-Free High-Precision L3 Cache Attack using Intel TSX."](https://dblp.uni-trier.de/rec/html/conf/uss/DisselkoenKPT17) by "Craig Disselkoen, David Kohlbrenner, Leo Porter, Dean M. Tullsen"

## Experiment Setup

### Experiment Content

The aforementioned paper describes a new kind of cache attack based on the Prime+Probe algorithm that does not rely on the use of timers and is therefore said to be much faster than Prime+Probe. The goal of this experiment is to verify if the algorithm called Prime+Abort descibed in the paper actually improves the performance of the Prime+Probe algorithm. It has not been attempted to replicate the exact same time measurements since they heavily depend on the hardware or operating system the benchmarks have been performed on.

### Hardware/Software

#### Hardware:
The whole experiment has been conducted in a Virtual Machine with the following Hardware specifications:
* 2 Processor cores of a Intel Core i9-9880H
* 8 Gb of Memory
* Ethernet connection via NAT over Intel PRO/1000MT Desktop

It is important to note that the Prime+Abort algorithm relys on Intels implementation of transactinal memory called ["Intel TSX"](TODO:WEBSITE) which means that it can only be run on CPUs that support TSX.

#### Software:
* Ubuntu 18.04
* [XLATE](https://github.com/vusec/xlate)

## Preconditions

In order to build the code `libelf-dev` and `libssl-dev` have to be installed. If you are using the apt package manager this can be done by running:

    # apt install libelf-dev
    # apt install libssl-dev

If you want to run the benchmark please follow the steps described in the `README.md` of the `code` folder.
## Experiment Steps

In order to compare the Prime+Probe algorithm with the Prime+Abort the XLATE toolkit has been used. The code has been slightly modify like described in the `README.md` of the `code` folder. The modification has been made for easier incorporation into the code of the benchmarking executable. The benchmark tool executes the Prime+Probe and Prime+Abort code multiple times, tracks the respective execution times and calculates the Min, Max, Median, Q1, Q3 and the Average.

## Results

Describe your results