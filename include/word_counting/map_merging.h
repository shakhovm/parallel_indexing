#ifndef MAP_MERGING_H
#define MAP_MERGING_H

//
// Created by shakhov on 4/5/20.
//

#include <map>
#include "../conc_queue/synch_queue.h"
#include <iostream>
typedef std::map<std::string, size_t> WordMap;
typedef std::pair<WordMap, WordMap> pair_map;

void map_merging(WordMap& m1, WordMap& m2);
void infinite_merging(synch_queue<WordMap>& map_queue,
                      synch_queue<pair_map>& pair_queue);
void map_queue_handler(synch_queue<WordMap>& map_queue, synch_queue<pair_map>& pair_queue);

#endif // MAP_MERGING_H
