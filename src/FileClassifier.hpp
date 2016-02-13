#ifndef FILE_CLASSIFIER_HPP
#define FILE_CLASSIFIER_HPP

#include <boost/filesystem.hpp>

#include <map>
#include <vector>

namespace file_classifier
{
    namespace fs = boost::filesystem;

    typedef std::vector< std::uint8_t > ByteBlock;
    typedef std::multimap< ByteBlock, fs::path > UniqueFiles;

    class FileClassifier
    {
        typedef std::multimap< std::uintmax_t, fs::path > SizeToFileMap;

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
