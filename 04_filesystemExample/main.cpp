/**
 * In c++20, there is std::source_location
 * https://en.cppreference.com/w/cpp/utility/source_location
 */

#include <iostream>
#include <filesystem>
#include <vector>

int main() {
    auto path = std::filesystem::current_path();

    std::cout << "Parent Path: "<< path.parent_path() << std::endl;
    std::cout << "Filename: "<< path.filename() << std::endl;
    std::cout << "Relative Path: "<< path.relative_path() << std::endl;
    std::cout << "Root directory: "<< path.root_directory() << std::endl;
    std::cout << "Root name: "<< path.root_name() << std::endl;
    std::cout << "Root Path: "<< path.root_path() << std::endl;

    auto pathSting = path.string();
    auto pathUntilFolder = [](const std::string path, const std::string tillFolder) {
        std::string trimmedPath;
        std::vector<std::string> v_folders;
        std::string word = "";
        bool found = false;
        for (auto x : path) {
            if (x == '/') {
                if(word == tillFolder){
                    found = true;
                    break;
                }
                v_folders.push_back(word);
                trimmedPath = trimmedPath + word;
                trimmedPath = trimmedPath + '/';
                word = "";
            } else
                word = word + x;
        }
        if (!found)
            return path;
        return trimmedPath;
    };
    auto trimmedPath = pathUntilFolder(pathSting,"04_filesystemExample");
    std::cout<<"TrimmedPath: "<<trimmedPath;
    return 0;
}
