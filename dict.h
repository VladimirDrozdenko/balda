#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <iostream>
#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>


struct TrieNode {
    bool is_word = false;
    TrieNode* children[26] = {0};
};


class Dictionary {
public:
    Dictionary(const char* filename) {
        root_ = new TrieNode();
        load(filename);
    }

    TrieNode* root() const {
        return root_;
    }

private:
    TrieNode* root_;

    void load(const char* filename) {
        FILE* file = fopen(filename, "r");
        if (!file) {
            std::cerr << "Could not open file\n";
            exit(1);
        }

        char word[100];
        while (fscanf(file, "%s", word) == 1) {
            insert(word);
        }
        fclose(file);
    }

    void insert(const char* word) {
        TrieNode* node = root_;
        for (int i = 0; word[i]; ++i) {
            int idx = word[i] - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->is_word = true;
    }
};

#endif // __DICTIONARY_H__
