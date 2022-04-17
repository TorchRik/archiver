#include "LetterStatistic.h"

bool LetterStatistic::operator<(const LetterStatistic &el) const {
    return count_of_letters < el.count_of_letters;
}
