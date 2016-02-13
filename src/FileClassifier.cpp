#include "FileClassifier.hpp"

using namespace file_classifier;

std::vector< std::string >
FileClassifier::
getFileGroups(std::string const &file_path)
{
    createFilesList(file_path);
    processFilesBySize(); 
    return std::vector< std::string >();
}

void
FileClassifier::
addRegularFile(fs::path p)
{
    std::uintmax_t file_size = fs::file_size(p); 
    FilePtr ptr(new File(p));
    sizeToFileMap_.insert(std::make_pair(file_size, ptr));
}

void
FileClassifier::
createFilesList(std::string const &file_path)
{
    if (!fs::exists(file_path))
    {
        throw std::runtime_error("Directory does not exists");
    }

    if (fs::is_regular_file(file_path))
    {
        throw std::runtime_error("Directory expected");
    }

    if (fs::is_directory(file_path))
    {
        for (auto it = fs::recursive_directory_iterator(file_path);
               it != fs::recursive_directory_iterator(); ++it)
        {
            fs::directory_entry& entry = *it;
            fs::path p = entry.path();

            if (fs::is_regular_file(p))
            {
                addRegularFile(p);
            }
        }
    }
}

void
FileClassifier::
processFilesBySize()
{
    typedef Files::iterator iterator;
    typedef Files::value_type value_type;
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
                          markedFiles_.insert(
                                  std::make_pair(currentFileId_, v.second));
                      });
        currentFileId_++;

        std::cout << it->first << std::endl;
    }
}

void
FileClassifier::
findDuplicates()
{
    FilesRange range;
    for (auto it = markedFiles_.begin();
            it != markedFiles_.end();
            it = range.second)
    {
        range = markedFiles_.equal_range(it->first);
        groupFiles(range);
    }
}

Files
FileClassifier::
groupFiles(FilesRange const &range)
{
    while (true)
    {
    }
}

/*void
FileClassifier::
divideToUniqueGroups(LabelledFiles &size_equal_files)
{

    LabelledFilesRange range;
    std::size_t identificator_counter = 0;
    LabelledFiles result;

    identificator_counter = 0;
    bool eof_flag = false;

    for (auto it = size_equal_files.begin();
            it != size_equal_files.end() && !eof_flag;
            it = range.second)
    {
        range = size_equal_files.equal_range(it->first);

        // Process equal range
        LabelGroup const &byteSeparatedFiles = separateByNextByte(range); 
    }
}

LabelGroup
FileClassifier::
separateByNextByte(LabelledFilesRange const &range)
{
    LabelGroup byteSeparatedFiles;
    for_each(range.first,
             range.second,
             [&] (LabelledFiles::value_type v)
             {
                 std::uint8_t byte;
                 FilePtr &file = v.second;
                 file->input_stream.read((char *)&byte, sizeof(byte));
                 byteSeparatedFiles.insert(make_pair(byte, v.second));
             });

    return byteSeparatedFiles;
}

void
FileClassifier::
addByteSeparatedFiles(LabelGroup const &byteSeparatedFiles, LabelledFiles &result)
{
    std::pair< LabelGroup::const_iterator, LabelGroup::const_iterator > range;
    Label currentLabel;

    for (auto it = byteSeparatedFiles.cbegin();
            it != byteSeparatedFiles.cend();
            it = range.second)
    {
        range = byteSeparatedFiles.equal_range(it->first);
        //currentLabel.id = 
        for_each(range.first
               , range.second
               , [&] (LabelGroup::value_type const &v)
                {
                    result.insert(std::make_shared(currentLabel, v.second));
                });
    }
}*/
