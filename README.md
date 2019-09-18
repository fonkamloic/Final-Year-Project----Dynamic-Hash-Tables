# Dynamic-Hash-Tables

# Abstract

Dynamic hash tables provide fast storage and retrieval of data in memory. This fast storage
is very important in the development of efficient programs. But, there is a limitation(amount
of available RAM) on how much data can be stored in memory. This limitation available in
dictionary hash tables was solved by introducing logarithmic hash tables which could index
data stored on disk. In this project, we will design an experiment to compare the performance
of the dictionary and logarithmic hash tables approach. That is in-memory hash table versus
disk-based hash table respectively. In-memory hash tables provide fast access to large numbers
of objects (here integers) with more space overheads than disk-based hash tables. However,
for very large input size, in-memory hash tables are cache expensive compared to the disk-
based ones. Our experiments show that, for very large input size, the logarithmic method(using
extendible hashing) uses lesser cache, memory and, processing power compared to the dictio-
nary method( using standard chaining). Extendible hashing structures give substantial savings
in space at no cost in time. In the best case, the overhead space required compared with the
dictionary is reduced ratio of 1:10, while access time differs by a factor 1/100 of a second.
We observed that the dictionary method space complexity has a linear growth meanwhile, the
logarithmic hash table space complexity has a logarithmic growth rate. Then, we recorded all
results obtained from the experiment, analyzed and gave an account for deviations in expected
theoretical performances. Furthermore, we suggested possible improvements to increase the
performance and robustness of the algorithms.
