#include <boost/filesystem.hpp>
#include <iostream>

#include "FileClassifier.hpp"

int main()
{
    file_classifier::FileClassifier f;
    f.getFileGroups("../");
    std::cout << "Boost example" << std::endl;
    return 0;
}
