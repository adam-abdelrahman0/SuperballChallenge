#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "disjoint.h"

using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
  public:
    Superball(int argc, char **argv);
    void analyze_superball();
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
};

void usage(const char *s) 
{
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

void Superball::analyze_superball() {
  DisjointSetByRankWPC ds = DisjointSetByRankWPC(r * c);
  vector<int> sizes(r * c, 1);
  vector<int> alreadyAdded((r - 4) * c);

  int curID = -1, secondID = -1, tempSize;
  char color;

  for (int row = 0; row < r; row++) {
    for (int col = 0; col < c; col++) {
      color = board[row * c + col];
      curID = ds.Find(row * c + col);

      if (color != '*' && color != '.') {
        if (col != 0 && color == board[row * c + col - 1]) {
          secondID = ds.Find(board[row * c + col - 1]);

          if (curID != secondID) {
            tempSize = sizes[curID];
            curID = ds.Union(curID, secondID);
            sizes[curID] = tempSize + sizes[secondID];
          }
        }
        if (col != c - 1 && color == board[row * c + col + 1]) {
          secondID = ds.Find(board[row * c + col + 1]);

          if (curID != secondID) {
            tempSize = sizes[curID];
            curID = ds.Union(curID, secondID);
            sizes[curID] = tempSize + sizes[secondID];
          }
        }
        if (row != 0 && color == board[(row - 1) * c + col]) {
          secondID = ds.Find(board[(row - 1) * c + col]);

          if (curID != secondID) {
            tempSize = sizes[curID];
            curID = ds.Union(curID, secondID);
            sizes[curID] = tempSize + sizes[secondID];
          }
        }
        if (row != r - 1 && color == board[(row + 1) * c + col]) {
          secondID = ds.Find(board[(row + 1) * c + col]);

          if (curID != secondID) {
            tempSize = sizes[curID];
            curID = ds.Union(curID, secondID);
            sizes[curID] = tempSize + sizes[secondID];
          }
        }
        
      }
    }
  }

  cout << "Scoring sets:" << endl;

  for (int row = 2; row < r - 2; row++) {
    curID = ds.Find(row * c);
    if (sizes[curID] >= mss && find(alreadyAdded.begin(), alreadyAdded.end(), curID) == alreadyAdded.end()) {
      alreadyAdded.push_back(curID);
      cout << "  Size: " << sizes[curID] << "  Char: " << char(board[row * c]) << "  Scoring Cell: " << row << ",0" << endl;
    }

    curID = ds.Find(row * c + 1);
    if (sizes[curID] >= mss && find(alreadyAdded.begin(), alreadyAdded.end(), curID) == alreadyAdded.end()) {
      alreadyAdded.push_back(curID);
      cout << "  Size: " << sizes[curID] << "  Char: " << char(board[row * c + 1]) << "  Scoring Cell: " << row << ",1" << endl;
    }
    
    curID = ds.Find(row * c + c - 2);
    if (sizes[curID] >= mss && find(alreadyAdded.begin(), alreadyAdded.end(), curID) == alreadyAdded.end()) {
      alreadyAdded.push_back(curID);
      cout << "  Size: " << sizes[curID] << "  Char: " << char(board[row * c + c - 2]) << "  Scoring Cell: " << row << "," << c - 2 << endl;
    }

    curID = ds.Find(row * c + c - 1);
    if (sizes[curID] >= mss && find(alreadyAdded.begin(), alreadyAdded.end(), curID) == alreadyAdded.end()) {
      alreadyAdded.push_back(curID);
      cout << "  Size: " << sizes[curID] << "  Char: " << char(board[row * c + c - 1]) << "  Scoring Cell: " << row << "," << c - 1 << endl;
    }
  }
  

  return;
}

main(int argc, char **argv)
{
  Superball *s;
 
  s = new Superball(argc, argv);

  s->analyze_superball();

}
