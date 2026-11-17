#include "GenomicPositions.h"
#include <iostream>
#include <string>
#include <stdexcept>

int main(int argc, char* argv[]) {

    // Prendo gli argomenti da riga di coman
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " file.fa k kmer\n";
        return 1;
    }
    try {
        GenomicPositions positions(std::stoul(argv[2]));
        positions.load_fasta(argv[1]);
        std::cout << "Sequenze lette: " << positions.sequence_count() << "\n";
        std::cout << "K-mer analizzati: " << positions.scanned_kmers() << "\n";
        std::cout << "K-mer canonici distinti: " << positions.unique_kmers() << "\n";
        GenomicPosition p{};
        if (positions.find(argv[3], p)) {
            std::cout << "Prima occorrenza: (" << p.sequence_id << ", " << p.offset << ")\n";
        } else {
            std::cout << "K-mer non trovato\n";
            return 2;
        }
    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << "\n";
        return 1;
    }
}
