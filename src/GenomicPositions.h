#ifndef USTAR_GENOMIC_POSITIONS_H
#define USTAR_GENOMIC_POSITIONS_H

#include <cstddef>
#include <string>
#include <unordered_map>

struct GenomicPosition {
    std::size_t sequence_id; // in quale sequenza FASTA si trova il k-mer
    std::size_t offset; // a quale offset della sequenza FASTA si trova il k-mer
};

class GenomicPositions {
public:
    // Costruttore per inizializzare l'oggetto con la lunghezza del k-mer
    explicit GenomicPositions(std::size_t k);

    // Caricare le sequenze di un file fasta indicizzando i k-mer
    void load_fasta(const std::string& filename);

    // Trovare la prima occorrenza
    bool find(const std::string& kmer, GenomicPosition& position) const;

    // Restituisce il numero di sequenze lette
    std::size_t sequence_count() const;

    // Restituisce il numero di k-mer analizzati
    std::size_t scanned_kmers() const;

    // Restituisce il numero di k-mer canonici distinti
    std::size_t unique_kmers() const;
private:
    
    // Attributi e funzioni membro

    std::size_t k_;
    std::size_t sequence_count_ = 0;
    std::size_t scanned_kmers_ = 0;
    std::unordered_map<std::string, GenomicPosition> first_positions_;
    void index_sequence(const std::string& sequence, std::size_t sequence_id);
    static std::string canonical_kmer(const std::string& kmer);
};

#endif
