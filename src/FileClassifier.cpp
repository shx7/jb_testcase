#include "FileClassifier.hpp"

using namespace file_duplicates;

Files
FileClassifier::
getDuplicates(std::string const &file_path)
{
    Files result;
    createFilesList(file_path);
    indexFiles();
    findDuplicates(result);
    return result;
}

void
FileClassifier::
addRegularFile(fs::path const &file_path)
{
    std::uintmax_t file_size = fs::file_size(file_path);
    FilePtr ptr(new File(file_path));
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
            fs::path entry_path = entry.path(); 
            if (fs::is_regular_file(entry_path))
            {
                addRegularFile(entry_path);
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
                      });
        currentFileId_++;
    }
}

void
FileClassifier::
findDuplicates(Files &result)
{
    FilesRange range;
    for (auto it = markedFiles_.begin();
            it != markedFiles_.end();
            it = range.second)
    {
        range = markedFiles_.equal_range(it->first);
        prevFileId_ = currentFileId_;
        processEqualSizeFiles(range, result);
    }
}

void
FileClassifier::
hashFiles(FilesRange const &sizeEqualRange, Files &indexedFiles)
{
    HashedFiles hashedFiles;
    for_each(sizeEqualRange.first, sizeEqualRange.second,
            [&] (Files::value_type const &v)
            {
                v.second->calculateHashSum();
                hashedFiles.insert(std::make_pair(v.second->hash, v.second));
            });

    HashedFilesRange range;
    for (auto it = hashedFiles.begin();
            it != hashedFiles.end(); it = range.second)
    {
        range = hashedFiles.equal_range(it->first);

        for_each(
                  range.first
                , range.second
                , [&] (HashedFiles::value_type const &v)
                  {
                      indexedFiles.insert(
                              std::make_pair(currentFileId_, v.second));
                  });
        currentFileId_++;
    }
}

void
FileClassifier::
processEqualSizeFiles(FilesRange const &sizeEqualRange, Files &output)
{
    if (isOneElementRange(sizeEqualRange))
    {
        return;
    }

    Files filesIdGroups, tmpFilesIdGroups;
    hashFiles(sizeEqualRange, filesIdGroups);

    FilesRange range;
    for (auto it = filesIdGroups.begin();
            it != filesIdGroups.end(); it = range.second)
    {
        range = filesIdGroups.equal_range(it->first);
    }

    std::size_t file_size = (sizeEqualRange.first)->second->size; 
    std::size_t block_size = 0;

    for (std::size_t i = 0; i < file_size; i += block_size)
    {
        FilesRange equalIdRange; 
        tmpFilesIdGroups.clear();

        for (auto it = filesIdGroups.begin();
                it != filesIdGroups.end(); it = equalIdRange.second)
        { 
            equalIdRange = filesIdGroups.equal_range(it->first);
            block_size = std::min(blockSize_, file_size - i);

            if (!isOneElementRange(equalIdRange))
            {
                separateByNextBlock(equalIdRange, tmpFilesIdGroups, block_size);
            }
        }
        filesIdGroups.swap(tmpFilesIdGroups);
    }

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
separateByNextBlock(
          FilesRange const &range
        , Files &output, std::size_t block_size)
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
