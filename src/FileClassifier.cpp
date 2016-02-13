#include "FileClassifier.hpp"

using namespace file_classifier;

std::vector< std::string >
FileClassifier::
getFileGroups(std::string const &file_path)
{
    file_path_ = file_path;
    createFilesList();
    printFiles(); 
    return std::vector< std::string >();
}

void
FileClassifier::
createFilesList()
{
    fs::path file_path(file_path_);
    if (fs::exists(file_path))
    {
        std::cout << "File exists" << std::endl;
        if (fs::is_regular_file(file_path))
        {
            std::cout << "FILE" << std::endl;
        }

        if (fs::is_directory(file_path))
        {
            std::cout << "DIR" << std::endl;
            for (auto it = fs::recursive_directory_iterator(file_path);
                   it != fs::recursive_directory_iterator(); ++it)
            {
                fs::directory_entry& entry = *it;
                fs::path p = entry.path();

                if (fs::is_regular_file(p))
                {
                    std::uintmax_t file_size = fs::file_size(p); 
                    filesBySize_.insert(std::make_pair(file_size, p));
                }
            }

        }
    }
    else
    {
        std::cout << "File not exists" << std::endl;
    }
}

void
FileClassifier::
printFiles()
{
    typedef decltype(filesBySize_)::iterator iterator;
    typedef decltype(filesBySize_)::value_type value_type;
    std::pair< iterator, iterator > range;

    for (auto it = filesBySize_.begin();
            it != filesBySize_.end();
            it = range.second)
    {
        range = filesBySize_.equal_range(it->first);

        std::for_each(range.first
                    , range.second
                    , [&] (value_type &v)
                          {
                              std::cout << v.second.string() << std::endl; 
                          });

        std::cout << it->first << std::endl;
    }
}