#ifndef UTIL_HPP
#define UTIL_HPP

#include "FileClassifier.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>

std::string parseCmdArguments(int argc, char** argv);

std::string getPrefixedFileSize(std::size_t file_size);

void printStatistics(file_duplicates::Files const &files);

#endif
