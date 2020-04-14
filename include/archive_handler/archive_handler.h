#ifndef ARCHIVE_HANDLER_H
#define ARCHIVE_HANDLER_H
#include <sstream>
#include <archive.h>
#include <archive_entry.h>
#include <fstream>

std::string archive_handler(const std::string& archive_name);
#endif // ARCHIVE_HANDLER_H
