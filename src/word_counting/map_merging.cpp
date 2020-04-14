#include "../../include/word_counting/map_merging.h"
void map_merging(WordMap& m1, WordMap& m2) {
    for (const auto& x: m2) {
        m1[x.first] += x.second;
    }
}

void infinite_merging(synch_queue<WordMap>& map_queue,
                      synch_queue<pair_map>& pair_queue) {
    while (true) {
        pair_map pm = pair_queue.pop();
        if (pm.first.empty()) {
            pair_queue.push(pm);
            break;
        }
        pair_queue.increase_value();
        map_merging(pm.first, pm.second);
        map_queue.push(std::move(pm.first));
    }
}

void map_queue_handler(synch_queue<WordMap>& map_queue, synch_queue<pair_map>& pair_queue) {
    while (true) {

        WordMap wm1 = map_queue.pop();

        if (pair_queue.get_counter() == pair_queue.get_mul())
        {
            pair_queue.emplace(WordMap(), WordMap());
            map_queue.push(std::move(wm1));
            break;
        }
        WordMap wm2 = map_queue.pop();

        pair_queue.emplace(std::move(wm1), std::move(wm2));
    }
}
