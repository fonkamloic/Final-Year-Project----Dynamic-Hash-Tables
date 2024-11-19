# Dynamic-Hash-Tables: A Simple Comparison of the Dictionary and Logarithmic Methods.

# Abstract

Dynamic hash tables provide fast storage and retrieval of data in memory. This fast storage is very important in the
development of efficient programs. There is a limitation on how much data can be stored in memory due to RAM’s
finite size. This limitation was remedied by introducing logarithmic hash tables which could index data stored on disk.
This project designed an experiment to compare the dictionary and the logarithmic hash table’s performance. We
observed the behaviour of each method with a varied amount of input size.
In-memory hash tables provide fast access to large numbers of objects (here, integers) with more space overheads.
However, for a huge input size, dictionary hash tables are cache expensive than logarithmic ones. Our experiments
show that the logarithmic method uses less cache and memory for huge input sizes than the dictionary method. We
also observed that the dictionary method’s space complexity has a linear growth; meanwhile, the logarithmic space
complexity has a logarithmic growth rate.
Our experiment’s results correspond to the expected theoretical performance after minor deviations, such as a
sudden drop in performance caused by thermal throttling, were corrected. Furthermore, we suggest possible improvements
to increase the performance and robustness of the algorithms.
