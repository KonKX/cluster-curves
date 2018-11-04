# cluster-curves

Implementation of K-means / K-medoids clustering algorithms for polygonal curves in C 

The project has been implemented and tested in Linux.

Six source code files, a header file, makefile, configuration file, and 2d curve dataset are contained.

The program consists of 3 logical parts: initialization, assignment and update. Initialization occurs only once in each
case while assignment and update as many times as needed until there is little change in clusters.

Here is a brief description of the code / header files

traversal.c: The file includes the implementation of the Frechet function which calculates the distance between two curves and get_mean
which is an extension of Frechet and calculates an optimal traversal of two curves as well as their average. Also included is the function
dist for the calculation of the two-point distance in the Euclidean space as well as some other functions related to the average curves.
 
init.c: The file includes the implementation of the simple_init function which randomly chooses K distributions from the input dataset
and the implementation of the kpp_init function which selects weighted distribution centers that are in large
distance between them. For this purpose, a binarySearch variant used in a distance table is used
for the selection of centers. The above functions relate to the initialization bit.

assign.c: The file contains some functions used in the assignment phase. Specifically, it includes simple_assign (Lloyd's)
which corresponds to curves to their nearest centers which have already been selected. The lsh_assign function assigns curves to the nearest centers
using a range search in a spreadsheet. Since the curves are converted into grid vectors, they are placed on the table and the centers are used
as queries to collect curves that are within a certain radius that is calculated at the beginning and doubled at each step
until all the curves of the buckets at which the centers end up are marked. Finally, all curves are checked if some are not
have been marked and placed on the respective clusters. Some other functions include insertion on the hash table,
conversion to grid curves, etc.

update.c: The file contains the update function that is the implementation of the PAM algorithm. The function calculates a new set of K centers which
optimize the objective function in existing clusters by exhaustively testing each curve in the same cluster as their center by checking each time
new objective function that results. Also included are some assistive functions for the update phase and finally get_silhouette which
is used to calculate the aggregate internal rating index.

parsing.c: The file contains some functions used to analyze the input and get_conf which reads the configuration file.

declarations.h: Includes statements of all the functions that have been implemented and some necessary structures used in the program.

The implementation of the Mean Discrete Frechet curve has not been completed, mainly because it could not be integrated into the rest of the code.

The program works with 2, 3, 4-dimensional curves

Make and Make clean compilation command to delete the object file
