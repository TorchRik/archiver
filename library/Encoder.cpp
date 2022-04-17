#include "Encoder.h"

std::unordered_map<LetterCode, size_t> GetLetterStatistic(std::istream& fin) {
    std::unordered_map<LetterCode, size_t> statistic;
    char current_ch;
    while (fin >> std::noskipws >> current_ch) {
        ++statistic[GetLetterCodeFromChar(current_ch)];
    }
    statistic[FILENAME_END] = 1;
    statistic[ONE_MORE_FILE] = 1;
    statistic[ARCHIVE_END] = 1;
    return statistic;
}

void AddTotLetterStatistic(std::unordered_map<LetterCode, size_t>& letter_statistic, std::string& str) {
    for (char ch : str) {
        ++letter_statistic[GetLetterCodeFromChar(ch)];
    }
}

std::shared_ptr<Node> CreateTrie(std::unordered_map<LetterCode, size_t>& letter_statistic) {
    if (letter_statistic.empty()) {
        throw std::runtime_error("letter_statistic can not be empty");
    }

    PriorityQueue queue;
    for (const auto & [letter, count_of_letters] : letter_statistic) {
        std::shared_ptr<Node> next_element = std::make_shared<Node>(LetterStatistic({letter, count_of_letters}));
        queue.PushNewElement(move(next_element));
    }
    while (queue.Size() > 1) {
        std::shared_ptr<Node> peek = std::make_shared<Node>();

        std::shared_ptr<Node> element_a = queue.PopTopElement();
        std::shared_ptr<Node> element_b = queue.PopTopElement();
        peek->left = element_a;
        peek->right = element_b;
        peek->data.count_of_letters = element_a->data.count_of_letters + element_b->data.count_of_letters;
        queue.PushNewElement(move(peek));
    }
    return queue.PopTopElement();
}

void CreateSymbolsCodes(std::shared_ptr<Node> peek, std::map<LetterCode, TrieLetterCode>& symbols_codes,
                        std::vector<bool> &current_code) {
    if (peek->left == nullptr && peek->right == nullptr) {
        TrieLetterCode code = current_code;
        symbols_codes[peek->data.letter_code] = code;
    }
    if (peek->left != nullptr) {
        current_code.push_back(false);
        CreateSymbolsCodes(peek->left, symbols_codes, current_code);
        current_code.pop_back();
    }
    if (peek->right != nullptr) {
        current_code.push_back(true);
        CreateSymbolsCodes(peek->right, symbols_codes, current_code);
        current_code.pop_back();
    }
}

bool CodeCompare(const std::pair<const std::vector<bool>, const std::vector<bool>>& vec_a,
                const std::pair<const std::vector<bool>, const std::vector<bool>>& vec_b) {
    if (vec_a.second.size() == vec_b.second.size()) {
        return vec_a.first < vec_b.first;
    }
    return vec_a.second.size() < vec_b.second.size();
}

void IncrementCanonicalCode(CanonicalCode& code) {
    if (code.empty()) {
        return;
    }

    size_t last_not_null_index = code.size();
    for (size_t index = 0; index < code.size(); ++index) {
        if (!code[index]) {
            last_not_null_index = index;
        }
    }
    if (last_not_null_index == code.size()) {
        code.push_back(0);
        last_not_null_index = 0;
    }
    code[last_not_null_index] = 1;
    for (size_t index = last_not_null_index + 1; index < code.size(); ++index) {
        code[index] = 0;
    }
}


std::vector<std::pair<LetterCode, CanonicalCode>> GetCanonicalCodeFromTrieCodes(
                  std::vector<std::pair<LetterCode, TrieLetterCode>>& sorted_codes) {
    std::vector<std::pair<LetterCode, CanonicalCode>> canonical_codes;
    CanonicalCode last_code;
    for (const auto& [letter_code, trie_code] : sorted_codes) {
        IncrementCanonicalCode(last_code);
        while (last_code.size() < trie_code.size()) {
            last_code.push_back(false);
        }
        canonical_codes.push_back({letter_code, last_code});
    }
    return canonical_codes;
}

std::vector<std::pair<LetterCode, CanonicalCode>> GetCanonicalSymbolsCodes(std::shared_ptr<Node> peek) {
    std::map<LetterCode, TrieLetterCode> symbols_codes;
    std::vector<bool> current_code;
    CreateSymbolsCodes(peek, symbols_codes, current_code);
    std::vector<std::pair<LetterCode, TrieLetterCode>> sorted_codes;
    for (const auto & el : symbols_codes) {
        sorted_codes.push_back(el);
    }
    std::sort(sorted_codes.begin(), sorted_codes.end(), CodeCompare);
    return GetCanonicalCodeFromTrieCodes(sorted_codes);
}

std::map<LetterCode, CanonicalCode> CreateSymbolsCodesMapAndWriteToFile(std::vector<std::pair<LetterCode, CanonicalCode>>& symbols_codes,
                                         BitWriter& writer) {
    size_t symbols_count = symbols_codes.size();
    LetterCode next_element = GetLetterCodeFromInt(symbols_count);
    writer.WriteBits(next_element);
    std::vector<int> count_of_codes_with_same_length(9);
    for (const auto& [symbol_code, canonical_code] : symbols_codes) {
        writer.WriteBits(symbol_code);
        while (canonical_code.size() - 1 >= count_of_codes_with_same_length.size()) {
            count_of_codes_with_same_length.push_back(0);
        }
        ++count_of_codes_with_same_length[canonical_code.size() - 1];
    }
    size_t total_count = 0;
    for (size_t i = 0; i < symbols_count && total_count < symbols_count; ++i) {
        total_count += count_of_codes_with_same_length[i];
        writer.WriteBits(GetLetterCodeFromInt(count_of_codes_with_same_length[i]));
    }
    std::map<LetterCode, CanonicalCode> symbols_codes_map;
    for (const auto& [symbol_code, canonical_code] : symbols_codes) {
        writer.WriteBits(canonical_code);
        symbols_codes_map[symbol_code] = canonical_code;
    }
    return symbols_codes_map;
}

void WriteEncodedInformationToFile(
    std::string path_to_file, BitWriter& writer, std::map<LetterCode, CanonicalCode> symbols_codes_map,
    std::istream& fin, bool is_last_file
    ) {
    // запись названия в файл
    for (char ch : path_to_file) {
        writer.WriteBits(symbols_codes_map[GetLetterCodeFromChar(ch)]);
    }
    writer.WriteBits(symbols_codes_map[FILENAME_END]);

    // запись основного текста в файл в закодированном виде
    fin.clear();
    fin.seekg(0);
    char ch;
    while (fin >> std::noskipws >> ch) {
        writer.WriteBits(symbols_codes_map[GetLetterCodeFromChar(ch)]);
    }

    if (is_last_file) {
        writer.WriteBits(symbols_codes_map[ARCHIVE_END]);
        writer.WriteBuffer();
    }
    else {
        writer.WriteBits(symbols_codes_map[ONE_MORE_FILE]);
    }
}

void Encode(std::string path_to_file, std::istream& fin, BitWriter& writer, bool is_last_file) {
    std::unordered_map<LetterCode, size_t> letter_statistic = GetLetterStatistic(fin);
    AddTotLetterStatistic(letter_statistic, path_to_file);
    std::shared_ptr<Node> trie = CreateTrie(letter_statistic);
    std::vector<std::pair<LetterCode, CanonicalCode>> symbols_codes = GetCanonicalSymbolsCodes(trie);
    std::map<LetterCode, CanonicalCode> symbols_codes_map = CreateSymbolsCodesMapAndWriteToFile(symbols_codes, writer);
    WriteEncodedInformationToFile( path_to_file,writer, symbols_codes_map, fin, is_last_file);
}