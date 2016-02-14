#ifndef FILE_CLASSIFIER_HPP
#define FILE_CLASSIFIER_HPP

#include <boost/filesystem.hpp>

#include <fstream>
#include <memory>
#include <stdexcept>

#include <map>
#include <vector>

namespace file_classifier
{
    namespace fs = boost::filesystem;

    struct File
    {
        fs::path file_path;
        std::ifstream input_stream;
        std::size_t size;

        File(fs::path const &file_path)
            : file_path(file_path)
            , size(0)
        {
            input_stream.open(file_path.string());

            if (!input_stream.is_open())
            {
                throw std::runtime_error("IO error");
            }

            input_stream.seekg(0, input_stream.end);
            size = input_stream.tellg();
            input_stream.seekg(0, input_stream.beg);
        }
    };

    typedef std::shared_ptr< File > FilePtr;

    typedef std::uintmax_t Label;
    typedef std::multimap< Label, FilePtr > Files;
    typedef std::pair< Files::iterator, Files::iterator > FilesRange;

    typedef std::vector< std::uint8_t > ByteBlock;
    typedef std::multimap< ByteBlock, FilePtr > ByteBlocksToFiles;

    class FileClassifier
    {
        public:
            FileClassifier()
                : currentFileId_(0)
                , prevFileId_(currentFileId_)
            {}

            std::vector<std::string> getFileGroups(std::string const &file_path);

            void createFilesList(std::string const &file_path);

        private:
            void processFiles();

            void indexFiles();

            void processEqualSizeFiles(FilesRange const &range, Files &output);

            void separateByNextByte(
                      FilesRange const &range
                    , Files &output
                    , std::size_t block_size);

            void addRegularFile(fs::path p);

            void addFilesByGroups(ByteBlocksToFiles &src, Files &dst);

            bool isOneElementRange(FilesRange const &range);

            void normalizeFileId(Files &filesIdGroups);

        private:
            Files sizeToFileMap_;
            Files markedFiles_;
            std::uintmax_t currentFileId_;
            std::uintmax_t prevFileId_;

            std::size_t const chunkSize_ = 4096;
    };
}

#endif
