# USTAR2 (Unitig STitch Advanced constRuction)
## Overview
USTAR2 is a k-mers set compressor, with counting.

It is based on the ideas of [UST](https://github.com/medvedevgroup/UST) 
and [prophAsm](https://github.com/prophyle/prophasm) 
for computing an SPSS representation (aka simplitigs) for the given k-mers set.

Additionally, it exploits the possibility of reusing already visited nodes to achieve better compression as shown by [Matchtigs](https://github.com/algbio/matchtigs).

You will find four executables and one bash script:
* `ustar`: the main program
* `ustarx`: a k-mers extractor
* `ustars`: compute k-mers statistics
* `ustar-test`: used for debug
* `validate`: a validation script

## Dependencies
There are no dependencies. 
However, you'll need [BCALM2](https://github.com/GATB/bcalm) 
in order to compute a compacted de Bruijn graph (cdBG) of your multi-fasta file.

## How to download and compile
* `git clone https://github.com/enricorox/USTAR`.
* `cd USTAR`
* `cmake . && make -j 4`.

## How to run USTAR2
Run BCALM2 first: 
* `./bcalm -kmer-size <kmer-size> -in <your-multi-fasta> -all-abundance-counts`

Then USTAR2:
* `./ustar -k <kmer-size> -i <bcalm-output>`

To use the best heuristic, add `-s+aa -x+u`

See the help `./ustar -h` for details and advanced options.

## How to validate the output
You can check that the output file contains the same k-mers of
your bcalm file with your preferred kmer counter.

If you want to check that __kmers and counts__ are correct,
* `./ustarx -k <kmer-size> -i <ustar-fasta> -c <ustar-counts> -s`
* `./validate <kmer-size> <your-multi-fasta> <ustar-kmers-counts>` 

Note that you'll need to install [Jellyfish-2](https://github.com/zippav/Jellyfish-2) in order to use `validate`.

## References

Rossignolo, E., Comin, M. (2026). A Linear Algorithm For Efficient Representation of k-mer Sets Using De Bruijn Graphs. In: Guarino, M.P., et al. Biomedical Engineering Systems and Technologies. BIOSTEC 2024. Communications in Computer and Information Science, vol 2546. Springer, Cham. https://doi.org/10.1007/978-3-031-96899-0_11

Rossignolo E. and Comin M. (2024). USTAR2: Fast and Succinct Representation of k-mer Sets Using De Bruijn Graphs. In Proceedings of the 17th International Joint Conference on Biomedical Engineering Systems and Technologies - Volume 1: BIOINFORMATICS; ISBN 978-989-758-688-0, SciTePress, pages 368-378. DOI: 10.5220/0012423100003657

