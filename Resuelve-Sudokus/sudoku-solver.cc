#include <string>
#include <array>
#include <iostream>
#include <cctype>
#include <vector>
#include <array>
#include <algorithm>
#include <iterator>
#include <set>
using namespace std;

typedef struct {
    int squarenum;
    array<int, 2> coords; //ROW, COLUMN
    set<int> possibleval;
} cacheEntry;

vector<cacheEntry> valCache;
array<array<int, 9>, 9> sudoku;
array<set<int>, 9> squareValsChache;

void takedata() {
    string l;
    int row = 0;
    while(getline(cin, l)) {
        for(int i = 0; i < l.length(); i++) {
            char c = l.at(i);
            if(c == '*') {
                sudoku.at(row).at(i) = -1;
            } else {
                int num = c -'0';
                int squarenum = (row/3 * 3) + (i/3);
                sudoku.at(row).at(i) = num;
                squareValsChache.at(squarenum).emplace(num);
                
            }

        }
        row++;
    }

    //for(int i = 0; i < int(squareValsChache.size()); i++) sort(squareValsChache.at(i).begin(), squareValsChache.at(i).end());

    // for(int i = 0; i < int(squareValsChache.size()); i++) {
    //     cout << "[";
    //     for(set<int>::iterator j = squareValsChache.at(i).begin(); j != squareValsChache.at(i).end(); j++) {
    //         cout << *j << " ";
    //     }
    //     cout << "]\n";
    //}

}

void printSudoku() {
    for(int i = 0; i < sudoku.size(); i++) {
        if(i != 0 && i % 3 == 0) cout << "_____________________" << endl << endl;
        for(int j = 0; j < sudoku.at(i).size(); j++) {
            if(j != 0 && j % 3 == 0) cout << "| ";
            cout << sudoku.at(i).at(j) << " ";
        }
        cout << endl << endl;
    }
}

set<int> computePossibleValues(int row, int column) {
    set<int> foundRC;

    //Check row and column
    //UP
    for(int i = row-1; i >= 0; i--) if(sudoku.at(i).at(column) > -1) foundRC.emplace(sudoku.at(i).at(column));
    //DOWN
    for(int i = row+1; i < int(sudoku.size()); i++) if(sudoku.at(i).at(column) > -1) foundRC.emplace(sudoku.at(i).at(column));
    //LEFT
    for(int i = column-1; i >= 0; i--) if(sudoku.at(row).at(i) > -1) foundRC.emplace(sudoku.at(row).at(i));
    //RIGHT
    for(int i = column+1; i < int(sudoku.at(row).size()); i++) if(sudoku.at(row).at(i) > -1) foundRC.emplace(sudoku.at(row).at(i));
    //sort(foundRC.begin(), foundRC.end());

    //Check values in the same square
    int squarenum = (row/3 * 3) + (column/3);
    set<int> foundVals;
    set_union(foundRC.begin(), foundRC.end(), squareValsChache.at(squarenum).begin(), squareValsChache.at(squarenum).end(), inserter(foundVals, foundVals.end()));

    //Exract possible values
    set<int> posValues;
    set<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    set_difference(values.begin(), values.end(), foundVals.begin(), foundVals.end(), inserter(posValues, posValues.end()));

    return posValues;
}

void updateSudoku(int v, int row, int column) {
    sudoku.at(row).at(column) = v;
    int squarenum = (row/3 * 3) + (column/3);

    squareValsChache.at(squarenum).emplace(v);

    //Propagate the updated value
    for(int i = 0; i < valCache.size(); i++) {
        if(valCache.at(i).coords[0] == row || valCache.at(i).coords[1] == column) valCache.at(i).possibleval.erase(v);

        if(squarenum == valCache.at(i).squarenum) {
            set<int> newposVals;
            set_difference(valCache.at(i).possibleval.begin(), valCache.at(i).possibleval.end(), squareValsChache.at(valCache.at(i).squarenum).begin(), 
            squareValsChache.at(valCache.at(i).squarenum).end(), inserter(newposVals, newposVals.end()));
            valCache.at(i).possibleval = newposVals;
        }

        if(int(valCache.at(i).possibleval.size()) == 1) {
            updateSudoku(*valCache.at(i).possibleval.begin(), valCache.at(i).coords[0], valCache.at(i).coords[1]);
        }

    }

}


int main() {
    takedata();

    for(int i = 0; i < sudoku.size(); i++) {
        for(int j = 0; j < sudoku.at(i).size(); j++) {

            if(sudoku.at(i).at(j) == -1) {
                set<int> posValues =  computePossibleValues(i, j);

                // cout << "[";
                // for(set<int>::iterator k = posValues.begin(); k != posValues.end(); k++) cout << *k << " ";
                // cout << "]\n";

                if(posValues.size() == 1) {
                    updateSudoku(*posValues.begin(), i, j);
                } else {
                    cacheEntry e; e.squarenum = (i/3 * 3) + (j/3); e.coords = {i, j}; e.possibleval = posValues;
                    valCache.push_back(e);
                }
            }
        }
    }

    printSudoku();
    return 0;
}
