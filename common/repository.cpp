#include "repository.hpp"
#include <dirent.h>
#include <algorithm>
#include <sys/types.h>

Repository::Repository(std::string dir, Reader &reader)
    : _dir(dir), _reader(reader)
{
    DIR *directory_pointer = opendir(_dir.c_str());
    struct dirent *entry;

    std::vector<std::string> file_names = list_files(_dir);
    std::sort(file_names.begin(), file_names.end());
    for (auto file_name : file_names)
    {
        _instance_files.push_back(_reader.read(file_name));
    }
    closedir(directory_pointer);
}

std::vector<std::string> Repository::list_files(std::string dir)
{
    std::vector<std::string> file_names, current_file_names;
    DIR *directory_pointer = opendir(dir.c_str());
    struct dirent *entry;
    while ((entry = readdir(directory_pointer)) != NULL)
    {
        std::string file_name(entry->d_name);
        if (entry->d_type == DT_DIR and file_name.compare(".") and file_name.compare(".."))
        {
            current_file_names = list_files(dir + file_name + "/");
            file_names.insert(file_names.end(), current_file_names.begin(), current_file_names.end());
        }
        if (entry->d_type == DT_REG)
        {
            file_names.push_back(dir + entry->d_name);
        }
    }
    closedir(directory_pointer);

    return file_names;
}

std::vector<InstanceFile> Repository::instance_files()
{
    return _instance_files;
}