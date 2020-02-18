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

It is important to note that the Prime+Abort algorithm relys on Intels implementation of transactinal memory called ["Intel TSX"](https://software.intel.com/en-us/blogs/2013/06/07/web-resources-about-intelr-transactional-synchronization-extensions) which means that it can only be run on CPUs that support TSX.

#### Software:
* Ubuntu 18.04
* [Mastik](https://cs.adelaide.edu.au/~yval/Mastik/)

## Preconditions

In order to build the code `libelf-dev` and `libssl-dev` have to be installed. If you are using the apt package manager this can be done by running:

    # apt install libelf-dev
    # apt install libssl-dev

If you want to run the benchmark please follow the steps described in the `README.md` of the `code` folder.
## Experiment Steps

In order to compare the Prime+Probe algorithm with the Prime+Abort the Mastik toolkit has been used. The code for the Prime+Probe algorithm has been taken from the Mastik toolkit. 
For the Prime+Abort algorithm the setup and cleanup steps have also been taken from Mastik but the actual attack has been built by using the description of the paper with some help from the implementation used by a toolkit called [XLATE](https://www.vusec.net/projects/xlate/). 
The XLATE code can be found on [GitHub](https://github.com/vusec/xlate).
The benchmark tool executes the Prime+Probe and Prime+Abort code multiple times, tracks the respective execution times and calculates the Min, Max, Median, Q1, Q3 and the Average.

## Results
<table>
<tr><th>With GPG </th><th>Without GPG</th></tr>
<tr><td>

|| Prime + Probe | Prime + Abort |
| - | ------------- | ------------- |
| Min | 138 232 ms | 579 ms |
| Max | 181 816 ms | 3 926 ms |
| Avg | 151 357 ms | 1 122 ms |
| 1Q | 146 581 ms | 724 ms |
| Median | 149 519 ms | 933 ms |
| 3Q | 153 798 ms | 1 234 ms |

</td>
<td>

|| Prime + Probe | Prime + Abort |
| - | ------------- | ------------- |
| Min | 134 012 ms | 421 ms |
| Max | 200 830 ms | 3 694 ms |
| Avg | 148 679 ms | 1 080 ms |
| 1Q | 141 747 ms | 709 ms |
| Median | 145 506 ms | 874 ms |
| 3Q | 150 938 ms | 1 180 ms |

</td>
</tr>
</table>

For more information on the results please refer to the `results` folder.