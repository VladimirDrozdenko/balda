#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_set>
#include <string>

#include "dict.h"
using namespace std;

const size_t DEFAULT_COLS = 10;
const size_t DEFAULT_ROWS = 10;

size_t COLS = 10;
size_t ROWS = 10;

char pick_a_char() {
    static std::mt19937 rng((unsigned)std::random_device{}());
    static std::uniform_int_distribution<int> dist('a', 'z');
    return static_cast<char>(dist(rng));
}


void fillMatrix(vector< vector<char> >& mtx, size_t rows, size_t cols) {
    mtx.clear();

    for (size_t r = 0; r < rows; ++r) {
        vector<char> row;
        for (size_t c = 0; c < cols; ++c) {
            row.push_back(pick_a_char());
        }
        mtx.push_back(row);
    }
}


void resolve(const vector<vector<char>>& mtx,
             TrieNode* node,
             unordered_set<string>& results,
             vector<vector<bool>>& visited,
             int r,
             int c,
             string& word) {

    if (r < 0 || c < 0 ||
        r >= static_cast<int>(mtx.size()) ||
        c >= static_cast<int>(mtx[0].size())) {
        return;
    }

    if (visited[r][c]) {
        return;
    }

    char ch = mtx[r][c];
    TrieNode* next = node->children[ch - 'a'];
    if (!next) {
        return;
    }

    visited[r][c] = true;
    word.push_back(ch);

    if (next->is_word && word.size() > 1) {
        results.insert(word);
    }

    resolve(mtx, next, results, visited, r - 1, c, word);
    resolve(mtx, next, results, visited, r, c + 1, word);
    resolve(mtx, next, results, visited, r + 1, c, word);
    resolve(mtx, next, results, visited, r, c - 1, word);

    word.pop_back();
    visited[r][c] = false;
}

void parse_cmd_line(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-c") == 0) {
            if (i + 1 >= argc) {
                cerr << "Error: -c requires an integer argument\n";
                exit(1);
            }
            COLS = strtol(argv[++i], nullptr, DEFAULT_COLS);
        }
        else if (strcmp(argv[i], "-r") == 0) {
            if (i + 1 >= argc) {
                cerr << "Error: -r requires an integer argument\n";
                exit(1);
            }
            ROWS = strtol(argv[++i], nullptr, DEFAULT_ROWS);
        }
        else {
            cerr << "Unknown argument: " << argv[i] << '\n';

            cout << "\n" << argv[0] << " -c [columns] -r [rows]\n";
            cout << "\n If not provided, the default values are columns=" 
                << DEFAULT_COLS << " and rows=" << DEFAULT_ROWS << endl;
            exit(1);
        }
    }
}

int main(int argc, char* argv[]) {

    parse_cmd_line(argc, argv);

    Dictionary dict("./english_words.txt");

    vector< vector<char> > matrix;
    fillMatrix(matrix, ROWS, COLS);

    for (const auto& row : matrix) {
        for (const auto& ch : row) {
            cout << ch << " ";
        }
        cout << endl;
    }

    unordered_set<string> words_in_matrix;
    vector<vector<bool>> visited_cells(ROWS, vector<bool>(COLS, false));

    string word;

    const size_t total = ROWS * COLS;
    for (int r = 0; r < matrix.size(); ++r) {
        for (int c = 0; c < matrix[r].size(); ++c) {
            cout << "\rProgress: " << ((c+1 + r*COLS)*100/total) << "%" << flush;
            resolve(matrix, dict.root(), words_in_matrix, visited_cells, r, c, word);
        }
    }
    cout << endl;

    vector<string> words(
        words_in_matrix.begin(),
        words_in_matrix.end()
    );

    sort(words.begin(), words.end(),
        [](const string& a, const string& b) {
            return a.size() > b.size(); // longest → shortest
        });

    for (const auto& word : words) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}