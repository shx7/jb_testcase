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
                    sizeToFileMap_.insert(std::make_pair(file_size, p));
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
    typedef SizeToFileMap::iterator iterator;
    typedef SizeToFileMap::value_type value_type;
    std::pair< iterator, iterator > range;

    for (auto it = sizeToFileMap_.begin();
            it != sizeToFileMap_.end();
            it = range.second)
    {
        range = sizeToFileMap_.equal_range(it->first);

        std::for_each(range.first
                    , range.second
                    , [&] (value_type &v)
                          {
                              std::cout << v.second.string() << std::endl; 
                          });

        std::cout << it->first << std::endl;
    }
}

void
FileClassifier::
divideToUniqueGroups(LabelledFiles &unique_files)
{
    typedef LabelledFiles::iterator iterator;
    typedef std::multimap< std::uint8_t, FilePtr > LabelGroup;

    std::pair<iterator, iterator> range;
    std::size_t identificator_counter = 0;
    std::vector< LabelGroup > groups;

    identificator_counter = 0;
    bool eof_flag = false;

    for (auto it = unique_files.begin();
            it != unique_files.end() && !eof_flag;
            it = range.second)
    {
        range = unique_files.equal_range(it->first);
        LabelGroup subGroup;

        // Process equal range
        for_each(range.first,
                 range.second,
                 [&] (LabelledFiles::value_type v)
                 {
                     std::uint8_t byte;
                     FilePtr &file = v.second;
                     file->input_stream.read((char *)&byte, sizeof(byte));
                     subGroup.insert(make_pair(byte, v.second));
                 });
        groups.push_back(subGroup);

        eof_flag = range.first->second->input_stream.eof();
    }
}
