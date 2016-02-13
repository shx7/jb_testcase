#ifndef FILE_CLASSIFIER_HPP
#define FILE_CLASSIFIER_HPP

#include <boost/filesystem.hpp>

#include <fstream>
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
        typedef std::vector< std::uint8_t > ByteBlock;
        typedef std::multimap< ByteBlock, File > UniqueFiles;


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
