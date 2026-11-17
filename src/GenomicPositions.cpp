#include "GenomicPositions.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>

// Costruttore
GenomicPositions::GenomicPositions(std::size_t k) : k_(k) { // il parametro k rappresenta la lunghezza dei k-mer 
    if (k_ == 0) throw std::invalid_argument("k deve essere positivo");
}

// Normalizza i k-mer canonici; scarta sequenze con basi non ACGT.
std::string GenomicPositions::canonical_kmer(const std::string& kmer) {
    std::string normalized, rc; // normalized è il k-mer normalizzato, rc è il reverse complement
    normalized.reserve(kmer.size());
    rc.reserve(kmer.size());
    for (char b : kmer) {
        char c = static_cast<char>(std::toupper(static_cast<unsigned char>(b)));
        if (c != 'A' && c != 'C' && c != 'G' && c != 'T') return "";
        normalized.push_back(c); // aggiungo alla stringa normalized il carattere normalizzato
    }
    for (auto it = normalized.rbegin(); it != normalized.rend(); ++it) {
        switch (*it) {
            case 'A': rc.push_back('T'); break;
            case 'C': rc.push_back('G'); break;
            case 'G': rc.push_back('C'); break;
            case 'T': rc.push_back('A'); break;
        }
    }
    return std::min(normalized, rc); // In modo da poterli confrontare nellla mappa, restituisco il canonico
}

void GenomicPositions::index_sequence(const std::string& sequence, std::size_t sequence_id) {
    if (sequence.size() < k_) return;
    for (std::size_t offset = 0; offset <= sequence.size() - k_; ++offset) {
        const std::string key = canonical_kmer(sequence.substr(offset, k_));
        if (key.empty()) continue;
        ++scanned_kmers_;
        // emplace non sovrascrive la prima occorrenza già trovata.
        first_positions_.emplace(key, GenomicPosition{sequence_id, offset});
    }
}

void GenomicPositions::load_fasta(const std::string& filename) {
    std::ifstream fasta(filename);
    if (!fasta) throw std::runtime_error("Impossibile aprire il FASTA: " + filename);
    first_positions_.clear();
    sequence_count_ = 0;
    scanned_kmers_ = 0;
    std::string line, sequence;
    bool inside_record = false;
    while (std::getline(fasta, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        if (line.front() == '>') {
            if (inside_record) {
                index_sequence(sequence, sequence_count_ - 1);
                sequence.clear();
            }
            ++sequence_count_;
            inside_record = true;
        } else {
            if (!inside_record) throw std::runtime_error("Sequenza senza header FASTA");
            sequence += line;
        }
    }
    if (inside_record) index_sequence(sequence, sequence_count_ - 1);
    if (!inside_record) throw std::runtime_error("FASTA senza header");
}

bool GenomicPositions::find(const std::string& kmer, GenomicPosition& position) const {
    if (kmer.size() != k_) return false;
    const std::string key = canonical_kmer(kmer);
    if (key.empty()) return false;
    auto it = first_positions_.find(key);
    if (it == first_positions_.end()) return false;
    position = it->second;
    return true;
}

std::size_t GenomicPositions::sequence_count() const { return sequence_count_; }
std::size_t GenomicPositions::scanned_kmers() const { return scanned_kmers_; }
std::size_t GenomicPositions::unique_kmers() const { return first_positions_.size(); }
