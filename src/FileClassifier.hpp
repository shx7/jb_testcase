#ifndef FILE_CLASSIFIER_HPP
#define FILE_CLASSIFIER_HPP

#include <boost/filesystem.hpp>

#include <map>
#include <vector>

namespace file_classifier
{
    namespace fs = boost::filesystem;

    class FileClassifier
    {
        public:
            std::vector<std::string> getFileGroups(std::string const &file_path);

            void createFilesList();

        private:
            void printFiles();

        private:
            std::string file_path_;
            std::multimap<std::uintmax_t, fs::path> filesBySize_;
    };
}

#endif
