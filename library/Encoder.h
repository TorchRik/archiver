#pragma once

#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "PriorityQueue.h"
#include "BitWriter.h"
#include "LetterCode.h"
#include <memory>

using TrieLetterCode = std::vector<bool>;
using CanonicalCode = std::vector<bool>;

void IncrementCanonicalCode(CanonicalCode& code);

std::unordered_map<LetterCode, size_t> GetLetterStatistic(std::istream& fin);

void AddTotLetterStatistic(std::unordered_map<LetterCode, size_t>& letter_statistic, std::string& str);

std::shared_ptr<Node> CreateTrie(std::unordered_map<LetterCode, size_t>& letter_statistic);

std::vector<std::pair<LetterCode, CanonicalCode>> GetCanonicalCodeFromTrieCodes(
    std::vector<std::pair<LetterCode, TrieLetterCode>>& sorted_codes);

void CreateSymbolsCodes(
    std::shared_ptr<Node> peek, std::map<LetterCode, TrieLetterCode>& symbols_codes, std::vector<bool> &current_code
    );

std::vector<std::pair<LetterCode, CanonicalCode>> GetCanonicalSymbolsCodes(std::shared_ptr<Node> peek);

std::map<LetterCode, CanonicalCode> CreateSymbolsCodesMapAndWriteToFile(
    std::vector<std::pair<LetterCode, CanonicalCode>>& symbols_codes, BitWriter& writer
    );

void WriteEncodedInformationToFile(
    std::string path_to_file, BitWriter& writer, std::map<LetterCode, CanonicalCode> symbols_codes_map,
    std::istream& fin, bool is_last_file
    );

void Encode(std::string path_to_file, std::istream& fin, BitWriter& writer, bool is_last_file);
