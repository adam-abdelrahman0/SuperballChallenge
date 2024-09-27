#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
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
  int curID = -1;

  for (int row = 0; row < r; row++) {
    for (int col = 0; col < c; col++) {
      if (col != 0 && 
          board[row * c + col - 1] == board[row * c + col]) {
        curID = ds.Find(row * c + col - 1);
        ds.Union(row * c + col, curID);
      }
      else if (row != 0 && 
          board[(row - 1) * (c) + col] == board[row * c + col]) {
        curID = ds.Find((row - 1) * (c) + col);
        //cout << char(board[(row - 1) * (c) + col]) << " " << char(board[row * c + col]) << endl;
        ds.Union(row * c + col, curID);
      }
    }
  }

  /*int scoreCol = 0, size = 0;
  for (int i = 2; i < 6; i++) {
    curID = ds.Find(board[i * c + scoreCol]);
    for (int j = 0; j < r * c; j++) {
    }
  }*/

  //ds.Print();

  cout << "Scoring sets:";

  return;
}

main(int argc, char **argv)
{
  Superball *s;
 
  s = new Superball(argc, argv);

  s->analyze_superball();

}
