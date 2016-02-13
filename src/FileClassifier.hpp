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

        bool operator<(Label const &label) const
        {
            if (id == label.id)
            {
                return byte < label.byte;
            }
            return id < label.id;
        }
    };

    typedef char ByteBlock;
    typedef std::shared_ptr< File > FilePtr;
    typedef std::multimap< Label, FilePtr > LabelledFiles;

    typedef std::pair<
          LabelledFiles::iterator
        , LabelledFiles::iterator > LabelledFilesRange;
    typedef std::multimap< std::uint8_t, FilePtr > LabelGroup;

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

            LabelGroup separateByNextByte(LabelledFilesRange const &range);

            void addByteSeparatedFiles(
                      LabelGroup const &byteSeparatedFiles
                    , LabelledFiles &result);

        private:
            std::string file_path_;
            SizeToFileMap sizeToFileMap_;
            std::size_t labelCounter_;
    };
}

#endif
