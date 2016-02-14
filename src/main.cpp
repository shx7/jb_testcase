#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "FileClassifier.hpp"

std::string getPrefixedFileSize(std::size_t file_size)
{
    std::string prefixes[] = {"B", "KB", "MB", "GB", "TB"};
    double size = file_size;
    std::size_t prefixes_size = sizeof(prefixes) / sizeof(prefixes[0]); 
    std::size_t prefix_index = 0;

    while (size >= 1024 && prefix_index < prefixes_size - 1)
    { 
        size /= 1024;
        prefix_index++;
    }

    std::string size_str = std::to_string(size);
    size_str = size_str.substr(0, size_str.find(".") + 2);
    return size_str + " " + prefixes[prefix_index];
}

void
printStatistics(file_duplicates::Files const &files)
{
    std::size_t total_wasted_files_size = 0;
    std::size_t current_group_id = 0;
    auto current_group_iterator = files.begin();
    for (auto it = files.begin(); it != files.end(); )
    {
        std::size_t file_count = 0;
        std::size_t file_size = it->second->size;
        while (it->first == current_group_iterator->first
                && it != files.end())
        {
            ++file_count;
            ++it;
        }

        std::size_t files_group_size = file_count * file_size;
        total_wasted_files_size += files_group_size - file_size;

        std::string files_group_size_str
            = getPrefixedFileSize(files_group_size);
        std::cout << "#" << current_group_id
                  << " " << files_group_size_str << std::endl;
        for_each(
                  current_group_iterator
                , it
                , [&] (file_duplicates::Files::value_type const &v)
                  {
                      std::cout << v.second->file_path.string() << std::endl;
                  });
        std::cout << std::endl;

        current_group_iterator = it;
        ++current_group_id;
    }

    std::cout << getPrefixedFileSize(total_wasted_files_size)
              << " wasted in total. " << std::endl;
}

int main()
{
    file_duplicates::FileClassifier f;
    try
    {
        file_duplicates::Files const &dups = f.getDuplicates("../../");

        if (dups.empty())
        {
            return 0;
        }

        printStatistics(dups);
    }
    catch (std::runtime_error const &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}
