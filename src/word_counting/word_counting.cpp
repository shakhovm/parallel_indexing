#include "../../include/word_counting/word_counting.h"
#include <boost/locale.hpp>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/locale/boundary.hpp>


void block_indexing(synch_queue<WordMap>& map_q, str_itr begin, str_itr end) {
    WordMap wm;

    boost::locale::boundary::ssegment_index map(boost::locale::boundary::word,
                begin, end);
    map.rule(boost::locale::boundary::word_letters);
    for (const auto& x: map) {
        wm[boost::locale::fold_case(boost::locale::normalize(std::string(x)))]++;
    }

    map_q.push(std::move(wm));
}

WordMap text_indexing(const std::string& text, size_t thread_number) {
    std::vector<std::string> vs;
    std::vector<std::thread> vector_thread;
    synch_queue<WordMap> map_queue;
    synch_queue<pair_map> pair_queue;

    size_t part_size = text.size() / thread_number +
                       static_cast<size_t>(text.size() % thread_number != 0);
    size_t counter = 0;


    vector_thread.emplace_back(map_queue_handler, std::ref(map_queue),
                               std::ref(pair_queue));

    for (auto i = text.cbegin(); i < text.cend();) {
        auto border = i + part_size;

        border = border < text.cend() ? border : text.cend();

        border = std::find_if(border, text.cend(),
                    [](char x) {return x == ' ' ||
        x == '\n' || x == '\t'; });

        vector_thread.emplace_back(block_indexing, std::ref(map_queue), i, border);

        if (border == text.cend()) {
            pair_queue.set_mul(counter);
            break;
        }
        ++counter;

        i = border;
    }
    for (size_t i = 0; i < thread_number; ++i) {
        vector_thread.emplace_back(infinite_merging, std::ref(map_queue),
                                   std::ref(pair_queue));
    }
    for (auto& x: vector_thread) {
        x.join();
    }

    return map_queue.pop();
}

WordMap sequential_indexing(const std::string& text) {
    WordMap wm;

    boost::locale::boundary::ssegment_index map(boost::locale::boundary::word,
                text.begin(), text.end());
    map.rule(boost::locale::boundary::word_letters);
    for (const auto& x: map) {
        wm[boost::locale::fold_case(boost::locale::normalize(std::string(x)))]++;
    }

    return wm;
}
