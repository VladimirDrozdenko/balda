#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <iostream>
#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>

class Dictionary {
public:
    Dictionary(const char* filename) {
        if (filename == nullptr) {
            std::cerr << "Filename is missing" << std::endl;
            exit(1);
        }

        FILE* file = fopen(filename, "r");
        if (file == nullptr || feof(file)) {
            std::cerr << "Could not open file: " << filename << std::endl;
            exit(1);
        }

        char word[100];
        while (fscanf(file, "%s", word) == 1) {
            words_[word] = true;
        }
        fclose(file);
    }

    bool contains(const std::string& word) const {
        return words_.find(word) != words_.end();
    }

private:
    std::map<std::string, bool> words_;
};

#endif // __DICTIONARY_H__
