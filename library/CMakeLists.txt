set(SRC_FILES BitReader.cpp BitWriter.cpp LetterStatistic.cpp Node.cpp PriorityQueue.cpp Encoder.cpp Decoder.cpp LetterCode.cpp Arguments.cpp)

add_library(archiver_library ${SRC_FILES})
target_include_directories(archiver_library PUBLIC ./)