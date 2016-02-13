#ifndef FILE_CLASSIFIER_HPP
#define FILE_CLASSIFIER_HPP

#include <boost/filesystem.hpp>

#include <fstream>
#include <memory>

#include <map>
#include <vector>

namespace file_classifier
{
    namespace fs = boost::filesystem;

    struct File
    {
        fs::path file_path;
        std::ifstream input_stream;
    };

    class FileClassifier
    {
        typedef std::multimap< std::uintmax_t, fs::path > SizeToFileMap;
        //typedef std::vector< std::uint8_t > ByteBlock;
        typedef char ByteBlock;
        typedef std::shared_ptr< File > FilePtr;
        typedef std::multimap< ByteBlock, FilePtr > UniqueFiles;


        public:
            std::vector<std::string> getFileGroups(std::string const &file_path);

            void createFilesList();

        private:
            void printFiles();

            UniqueFiles divideToUniqueGroups(UniqueFiles &unique_files);

        private:
            std::string file_path_;
            SizeToFileMap sizeToFileMap_;
    };
}

#endif
