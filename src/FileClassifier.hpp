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

    struct Label
    {
        std::size_t id;
        std::uint8_t byte;
    };

    typedef char ByteBlock;
    typedef std::shared_ptr< File > FilePtr;
    typedef std::multimap< Label, FilePtr > LabelledFiles;

    class FileClassifier
    {
        typedef std::multimap< std::uintmax_t, fs::path > SizeToFileMap;
        //typedef std::vector< std::uint8_t > ByteBlock;

        public:
            FileClassifier()
                : labelCounter_(0) {}

            std::vector<std::string> getFileGroups(std::string const &file_path);

            void createFilesList();

        private:
            void printFiles();

            void divideToUniqueGroups(LabelledFiles &unique_files);

        private:
            std::string file_path_;
            SizeToFileMap sizeToFileMap_;
            std::size_t labelCounter_;
    };
}

#endif
