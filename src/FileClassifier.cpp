#include "FileClassifier.hpp"

using namespace file_classifier;

std::vector< std::string >
FileClassifier::
getFileGroups(std::string const &file_path)
{
    createFilesList(file_path);
    indexFiles();
    processFiles();
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
indexFiles()
{
    FilesRange range;

    for (auto it = sizeToFileMap_.begin();
            it != sizeToFileMap_.end();
            it = range.second)
    {
        range = sizeToFileMap_.equal_range(it->first);

        std::for_each(range.first
                    , range.second
                    , [&] (Files::value_type const &v)
                      {
                          markedFiles_.insert(
                                  std::make_pair(currentFileId_, v.second));
                          std::cout << "Inserted!" << std::endl;
                      });
        currentFileId_++;

        std::cout << "current file id = " << currentFileId_ << std::endl;
    }
}

void
FileClassifier::
processFiles()
{
    Files result;
    FilesRange range;
    for (auto it = markedFiles_.begin();
            it != markedFiles_.end();
            it = range.second)
    {
        range = markedFiles_.equal_range(it->first);
        prevFileId_ = currentFileId_;
        processEqualSizeFiles(range, result);
    }

    // TODO: iterator throw result
    for_each(result.begin(), result.end(),
            [&] (Files::value_type const &v)
            {
            std::cout << v.first << "# "
                      << v.second->file_path.string() << std::endl;
            });
}

void
FileClassifier::
processEqualSizeFiles(FilesRange const &sizeEqualRange, Files &output)
{
    for_each(sizeEqualRange.first, sizeEqualRange.second,
            [&] (Files::value_type const &v)
            {
            std::cout << "[EL]: " << v.second->file_path.string() << std::endl;
            });
    std::cout << std::endl;

    Files filesIdGroups(sizeEqualRange.first, sizeEqualRange.second);
    Files tmpFilesIdGroups;
    std::size_t file_size = (sizeEqualRange.first)->second->size;

    if (isOneElementRange(sizeEqualRange))
    {
        output.insert(filesIdGroups.begin(), filesIdGroups.end());
        return;
    }

    std::size_t block_size = 0;
    std::cout << "File size = " << file_size << std::endl;

    for (std::size_t i = 0; i < file_size; i += block_size)
    {
        FilesRange equalIdRange; 

        for (auto it = filesIdGroups.begin();
                it != filesIdGroups.end(); it = equalIdRange.second)
        {
            equalIdRange = filesIdGroups.equal_range(it->first);
            block_size = std::min(chunkSize_, file_size - i + 1);
            separateByNextByte(equalIdRange, tmpFilesIdGroups, block_size);
        }
        filesIdGroups.swap(tmpFilesIdGroups);
        tmpFilesIdGroups.clear();
    }
    std::cout << "File comparing finished. Normalization" << std::endl;

    normalizeFileId(filesIdGroups);
    output.insert(filesIdGroups.begin(), filesIdGroups.end());
}

void
FileClassifier::
normalizeFileId(Files &filesIdGroups)
{
    Files normalizedFilesIdGroups;
    FilesRange range;
    currentFileId_ = prevFileId_;

    for (auto it = filesIdGroups.begin();
            it != filesIdGroups.end(); it = range.second)
    {
        range = filesIdGroups.equal_range(it->first);
        for_each(range.first
               , range.second
               , [&] (Files::value_type const &v)
                {
                    normalizedFilesIdGroups.insert(
                            make_pair(currentFileId_, v.second));
                });
        currentFileId_++;
    }
    filesIdGroups.swap(normalizedFilesIdGroups);
}

void
FileClassifier::
addFilesByGroups(ByteBlocksToFiles &src, Files &dst)
{
    typedef ByteBlocksToFiles::iterator iterator;
    std::pair< iterator, iterator > range;

    for (auto it = src.begin();
            it != src.end(); it = range.second)
    {
        range = src.equal_range(it->first);
        for_each(range.first
               , range.second
               , [&] (ByteBlocksToFiles::value_type const &v)
                {
                    dst.insert(make_pair(currentFileId_, v.second));
                });
        currentFileId_++;
    }
}

void
FileClassifier::
separateByNextByte(FilesRange const &range, Files &output, std::size_t block_size)
{
    ByteBlocksToFiles byteSeparatedFiles;
    for_each(range.first,
             range.second,
             [&] (Files::value_type const &v)
             {
                 ByteBlock byteBlock(block_size);
                 FilePtr const &file = v.second;
                 file->input_stream.read((char *)&byteBlock[0], block_size);
                 byteSeparatedFiles.insert(make_pair(byteBlock, v.second));

                 //std::cout << '\'' << (char)byte << '\'' << std::endl;
             });

    addFilesByGroups(byteSeparatedFiles, output);
}

bool
FileClassifier::
isOneElementRange(FilesRange const &range)
{
    Files::iterator range_begin = range.first;
    return ++range_begin == range.second;
}
