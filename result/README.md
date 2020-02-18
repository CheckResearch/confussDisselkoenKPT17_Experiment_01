# Results
The algorithms have been benchmarked by running each algorithm 1000 times and measuring the time it took from building the eviction set until the the clean up has finished. 
The whole benchmark has been executed twice, once with gpg running in the background and signing a text file and once without. This has been done in order to compare the runtimes of the algorithm with and without an evicted cache line.

## Result Overview
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

## Detailed Results
The detailed results while GPG was running can be found in the file `results_gpg.txt`, the results while it was not running in `results_noAccess.txt`.