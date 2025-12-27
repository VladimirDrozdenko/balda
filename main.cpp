#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <string>

#include "dict.h"
using namespace std;

const size_t COLS = 5;
const size_t ROWS = 5;


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


string get_cell_hash(int r, int c) {
    return to_string(r) + to_string(c);
}


void resolve(const vector< vector<char> >& mtx,
                   const Dictionary& dict, 
                   set<string>& results,
                   set<string>& visited_cells,
                   int r, 
                   int c, 
                   string word) {

    if (r < 0 || c < 0 || r >= mtx.size() || c >= mtx.size()) {
        return;
    }

    const string cell_hash = get_cell_hash(r, c);
    if (visited_cells.find(cell_hash) != visited_cells.cend()) {
        return;
    }

    word += mtx[r][c];
    if (word.size() > 1 && dict.contains(word)) {
        results.insert(word);
    }

    visited_cells.insert(cell_hash);

    resolve(mtx, dict, results, visited_cells, r - 1, c, word);
    resolve(mtx, dict, results, visited_cells, r, c + 1, word);
    resolve(mtx, dict, results, visited_cells, r + 1, c, word);
    resolve(mtx, dict, results, visited_cells, r, c - 1, word);

    visited_cells.erase(cell_hash);
}


int main() {
    Dictionary dict("./english_words.txt");

    vector< vector<char> > matrix;
    fillMatrix(matrix, ROWS, COLS);

    for (const auto& row : matrix) {
        for (const auto& ch : row) {
            cout << ch << " ";
        }
        cout << endl;
    }

    set<string> words_in_matrix;
    set<string> visited_cells;

    const size_t total = ROWS * COLS;
    for (int r = 0; r < matrix.size(); ++r) {
        for (int c = 0; c < matrix[r].size(); ++c) {
            cout << "\rProgress: " << ((c+1 + r*COLS)*100/total) << "%" << flush;
            resolve(matrix, dict, words_in_matrix, visited_cells, r, c, string());
        }
    }
    cout << endl;

    for (const auto& word : words_in_matrix) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}