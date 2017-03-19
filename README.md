# Repository for exercises done during the Parallelprogramming course

## `OpenMP` (Open Multi Processing): API for parallelizing programs on shared memory systems

| Folder            | What it does                                                | New Features utilized                           |
| ----------------- | ----------------------------------------------------------- | ----------------------------------------------- |
| `hello`           | Hello World for OpenMP                                      | OpenMP API usage                                |
| `pi`              | Approximate pi by numerical integration                     | For worksharing, reduction, datascoping clauses |
| `jacobi`          | Jacobi iterative solving method                             | Numa-Awareness                                  |
| `fibonacci`       | Recursive fibonacci                                         | Tasking                                         |
| `for`             | Add up a sum                                                | Schedule clause                                 |
| `minmaxreduction` | Find min / max of a random array                            | Min / Max reduction operation                   |
| `quicksort`       | Perform recursive quicksort on a random array               | Tasking                                         |
| `primes`          | Find all primes up to a given threshold                     | Critical region                                 |
| `mandelbrot`      | Calculate the mandelbrot set and output result as a picture | Datascoping                                     |

## `MPI` (Message Passing Interface): Standard for message passing in distributed memory systems

|File                   |What it does                                                           |New Features utilized                  |
|---------------------- |---------------------------------------------------------------------- |-------------------------------------- |
|`array(2-4).c`         |Count occurance of numbers in a random array                           |MPI_Probe                              |
|`bcast(2).c`           |Perform self-implemented broadcast                                     |MPI_Send, MPI_Recv                     |
|`calcpi.c`             |Approximate pi by numerical integration                                |MPI_Reduce                             |
|`circle.c`             |Perform a circular communication between all ranks                     |MPI_Wtime                              |
|`customcommunicator.c` |Create custom communicator (first 20, then groups of 5)       |MPI_Comm_group / create / split, MPI_Group_incl |
|`helloworld.c`         |Hello World for MPI                                              |MPI_Init, MPI_Finalize, MPI_Comm_size / rank |
|`mxv.c`                |Perform matrix-vector-multiplication                                   |MPI_Bcast, MPI_Scatter, MPI_Gather     |
|`mxvcustomdatatype.c`  |Perform matrix-vector-multiplication with custom MPI_Datatype          |MPI_Type_contiguous / commit           |
|`pingpong.c`           |2 Ranks perform ping-pong communication                                |MPI_Send, MPI_Recv                     |
|`primempiint.c`        |Find next prime from given input (take an int as input)                |MPI_Issend                             |
|`primempilong.c`       |Find next prime from given input (take an unsigned long long as input) |MPI_Issend, MPI_UNSIGNED_LONG_LONG     |
|`testmpitest.c`        |Check how often MPI_Test can be called within 1 second                 |MPI_Test                               |
