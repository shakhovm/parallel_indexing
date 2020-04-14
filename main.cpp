#include <iostream>
#include <boost/locale.hpp>
#include "include/word_counting/word_counting.h"
#include "include/conf_handler/confhandler.h"
#include "include/archive_handler/archive_handler.h"

template <typename Func, typename ...Args, typename T>
void time_estimator(Func f,  T& value, size_t& time_es, const Args&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    value = f(args...);
    auto end = std::chrono::high_resolution_clock::now();
    time_es = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main(int argc, char* argv[])
{
    boost::locale::generator gen;
    std::locale loc = gen("en_US.UTF-8");
    std::locale::global(loc);
    std::string conf_file = argc > 1 ? argv[1] : "conf.dat";
    ConfHandler conf_handler;
    try {
       conf_handler = ConfHandler(conf_file);
    } catch (std::runtime_error& e) {
        std::cout << "Failed to handle conf file" << std::endl;
        return 1;
    }

    auto conf_params = conf_handler.getConfParams();

    auto start_all = std::chrono::high_resolution_clock::now();
    std::string file_data;
    try {
        file_data = archive_handler(conf_params.infile);
    } catch (std::runtime_error& e) {
        std::cout << "Failed to handle archive" << std::endl;
        return 2;
    }

    auto end_read = std::chrono::high_resolution_clock::now();
    std::cout << "Reading (microseconds) : " <<
        std::chrono::duration_cast<std::chrono::microseconds>(end_read - start_all).count() << std::endl;
    WordMap wm;
    size_t time_s;
    if (conf_params.thread_number == 1) {
        time_estimator(sequential_indexing, wm, time_s, file_data);
    } else {
        time_estimator(text_indexing, wm, time_s, file_data,
                       conf_params.thread_number);
    }

    std::cout << "Indexing (milliseconds)  :"  << time_s << std::endl;
    std::vector<std::pair<std::string, size_t>> vw(wm.begin(), wm.end());
    std::sort(vw.begin(), vw.end(),
            [](const std::pair<std::string, size_t>& a,
              const std::pair<std::string, size_t>& b){ return a.first < b.first; });

    std::ofstream out_by_a(conf_params.out_by_a);
    for (const auto& x: vw) {
        out_by_a << x.first << " : " << x.second << std::endl;
    }
    out_by_a.close();
    std::sort(vw.begin(), vw.end(),
              [](const std::pair<std::string, size_t>& a,
              const std::pair<std::string, size_t>& b){ return a.second > b.second; });
    std::ofstream out_by_n(conf_params.out_by_n);
    for (const auto& x: vw) {
        out_by_n << x.first << " : " << x.second << std::endl;
    }
    auto end_all = std::chrono::high_resolution_clock::now();
    std::cout << "Total (milliseconds) : " <<
              std::chrono::duration_cast<std::chrono::milliseconds>(end_all - start_all).count() << std::endl;


    return 0;
}
