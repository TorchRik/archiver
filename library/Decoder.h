#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include "BitReader.h"
#include "LetterCode.h"
#include "Node.h"

void AddNewElementToTrie(std::shared_ptr<Node> peek, LetterCode& canonical_code, size_t index, LetterCode& letter_code);

LetterCode GetLetterCodeByCanonicalCode(std::shared_ptr<Node> peek, BitReader& reader);

std::shared_ptr<Node> ReadEncodeInformationAndCreateTrie(BitReader& archive_reader);

std::string CreatePathToFile(BitReader& archive_reader, std::shared_ptr<Node> trie);

bool WriteInformationToFile(BitReader& archive_reader, std::string path_to_file, std::shared_ptr<Node> trie);

bool Decode(BitReader& archive_reader);  // return true if next file exist
