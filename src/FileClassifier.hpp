#ifndef FILE_CLASSIFIER_HPP
#define FILE_CLASSIFIER_HPP

#include <boost/filesystem.hpp>
#include <openssl/md5.h>

#include <fstream>
#include <memory>
#include <stdexcept>

#include <map>
#include <vector>

namespace file_classifier
{
    namespace fs = boost::filesystem;
    typedef std::vector< unsigned char > HashSum;

    struct File
    {
        fs::path file_path;
        std::ifstream input_stream;
        std::size_t size;
        HashSum hash;

        File(fs::path const &file_path)
            : file_path(file_path)
            , size(0)
            , hash(MD5_DIGEST_LENGTH)
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

        void calculateHashSum()
        {
            std::size_t block_size = 1;
            std::size_t chunk_size = 4096;

            std::vector< unsigned char > data(chunk_size);
            MD5_CTX md5Context;
            MD5_Init(&md5Context);

            for (std::size_t i = 0; i < size; i += block_size)
            {
                block_size = std::min(chunk_size, size - i);
                input_stream.read((char *)&data[0], block_size);
                MD5_Update(&md5Context, &data[0], block_size);
            }
            MD5_Final(&hash[0], &md5Context);
        }
    };

    typedef std::shared_ptr< File > FilePtr;

    typedef std::uintmax_t Label;
    typedef std::multimap< Label, FilePtr > Files;
    typedef std::pair< Files::iterator, Files::iterator > FilesRange;

    typedef std::multimap< HashSum, FilePtr > HashedFiles;
    typedef std::pair< HashedFiles::iterator, HashedFiles::iterator >
        HashedFilesRange;

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

            void hashFiles(FilesRange const &sizeEqualRange, Files &indexedFiles);

        private:
            Files sizeToFileMap_;
            Files markedFiles_;
            std::uintmax_t currentFileId_;
            std::uintmax_t prevFileId_;

            std::size_t const chunkSize_ = 4096;
    };
}

#endif
