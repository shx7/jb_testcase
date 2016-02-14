#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "FileClassifier.hpp"
#include "Util.hpp"

int main(int argc, char **argv)
{
    try
    {
        std::string search_dir = parseCmdArguments(argc, argv);
        if (search_dir.empty())
        {
            return -1;
        }

        file_duplicates::FileClassifier f;
        file_duplicates::Files const &dups = f.getDuplicates(search_dir);

        printStatistics(dups);
    }
    catch (std::runtime_error const &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    catch (std::exception const &ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
