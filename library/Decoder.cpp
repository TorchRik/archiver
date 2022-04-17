#include "Decoder.h"

void AddNewElementToTrie(std::shared_ptr<Node> peek, LetterCode& canonical_code, size_t index, LetterCode& letter_code) {
    if (index >= canonical_code.size()) {
        peek->data.letter_code.swap(letter_code);
        return;
    }
    if (canonical_code[index]) {
        if (peek->right == nullptr) {
            peek->right = std::make_shared<Node>();
        }
        AddNewElementToTrie(peek->right, canonical_code, ++index, letter_code);
    }
    else {
        if (peek->left == nullptr) {
            peek->left = std::make_shared<Node>();
        }
        AddNewElementToTrie(peek->left, canonical_code, ++index, letter_code);
    }
}

LetterCode GetLetterCodeByCanonicalCode(std::shared_ptr<Node> peek, BitReader& reader) {
    if (!peek->data.letter_code.empty()) {
        return peek->data.letter_code;
    }
    bool bit = reader.ReadBits(1)[0];
    if (bit) {
        return GetLetterCodeByCanonicalCode(peek->right, reader);
    }
    else {
        return GetLetterCodeByCanonicalCode(peek->left, reader);
    }
}

std::shared_ptr<Node> ReadEncodeInformationAndCreateTrie(BitReader& archive_reader) {
    int count_of_symbols = GetIntFromLetterCode(archive_reader.ReadBits(9));
    std::vector<LetterCode> symbols;
    for (int i = 0; i < count_of_symbols; ++i) {
        symbols.push_back(archive_reader.ReadBits(9));
    }
    std::vector<int> count_of_symbols_length_n;
    int total_sum = 0;
    while (total_sum < count_of_symbols) {
        int next_count = GetIntFromLetterCode(archive_reader.ReadBits(9));
        total_sum += next_count;
        count_of_symbols_length_n.push_back(next_count);
    }
    int symbols_index = 0;
    std::shared_ptr<Node> trie = std::make_shared<Node>();
    for (size_t length = 1; length < count_of_symbols_length_n.size() + 1; ++length) {
        for (int i = 0; i < count_of_symbols_length_n[length-1]; ++i) {
            LetterCode code = archive_reader.ReadBits(length);
            AddNewElementToTrie(trie, code, 0, symbols[symbols_index++]);
        }
    }
    return trie;
}

std::string CreatePathToFile(BitReader& archive_reader, std::shared_ptr<Node> trie) {
    std::string path_to_file;
    LetterCode next_code = GetLetterCodeByCanonicalCode(trie, archive_reader);
    while (next_code != FILENAME_END) {
        path_to_file += GetCharFromLetterCode(next_code);
        next_code = GetLetterCodeByCanonicalCode(trie, archive_reader);
    }
    return path_to_file;
}

bool WriteInformationToFile(BitReader& archive_reader, std::string path_to_file, std::shared_ptr<Node> trie) {
    std::ofstream out(path_to_file);
    LetterCode next_code = GetLetterCodeByCanonicalCode(trie, archive_reader);
    while (next_code != ARCHIVE_END && next_code != ONE_MORE_FILE) {
        out << GetCharFromLetterCode(next_code);
        next_code = GetLetterCodeByCanonicalCode(trie, archive_reader);
    }
    return next_code == ONE_MORE_FILE;
}

bool Decode(BitReader& archive_reader) {
    std::shared_ptr<Node> trie = ReadEncodeInformationAndCreateTrie(archive_reader);
    std::string path_to_file = CreatePathToFile(archive_reader, trie);
    return WriteInformationToFile(archive_reader, path_to_file, trie);
}