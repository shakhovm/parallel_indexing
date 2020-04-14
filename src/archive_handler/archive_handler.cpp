#include "../../include/archive_handler/archive_handler.h"
std::string archive_handler(const std::string& archive_name) {
    archive *a;
    archive_entry *entry;
    a = archive_read_new();
    int r;
    r = archive_read_support_filter_all(a);
    if (r != ARCHIVE_OK) {
        throw std::runtime_error("Error when using archive filter all");
    }

    r = archive_read_support_format_raw(a);
    if (r != ARCHIVE_OK) {
        throw std::runtime_error("Error when using archive format raw");
    }

    r = archive_read_support_format_all(a);
    if (r != ARCHIVE_OK) {
        throw std::runtime_error("Error when using archive support format");
    }

    std::ifstream raw_file(archive_name, std::ios::binary);
    if (!raw_file.is_open()) {
        throw std::runtime_error("Archive Not Found!");
    }
    std::string buffer = dynamic_cast<std::ostringstream&>(
            std::ostringstream{} << raw_file.rdbuf()).str();

    r = archive_read_open_memory(a, buffer.c_str(), buffer.size()); // Note 1
    if (r != ARCHIVE_OK) {
        throw std::runtime_error("Error in read archive from memory");
    }

    size_t size;
    const void* buff;
    la_int64_t offset;

    r = archive_read_next_header(a, &entry);
    if (r != ARCHIVE_OK) {
        throw std::runtime_error("Error when reading next header");
    }
    r = -1;

    std::string ss;

    while (r != ARCHIVE_EOF) {
        r = archive_read_data_block(a, &buff, &size, &offset);
//        if (r == ARCHIVE_)
        ss.append(std::string(static_cast<const char*>(buff), size));
        ss += " ";
    }
    std::ofstream s("../files/example.txt");
    s << ss;
    archive_read_close(a);
    archive_read_free(a);
    return ss;
}
