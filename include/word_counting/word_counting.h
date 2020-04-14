#ifndef WORD_COUNTING_H
#define WORD_COUNTING_H

#include "../conc_queue/synch_queue.h"
#include "../word_counting/map_merging.h"
#include <vector>


typedef std::string::const_iterator str_itr;
void block_indexing(synch_queue<WordMap>& map_q, str_itr begin, str_itr end);

WordMap text_indexing(const std::string& text, size_t thread_number);

WordMap sequential_indexing(const std::string& text);
#endif // WORD_COUNTING_H
