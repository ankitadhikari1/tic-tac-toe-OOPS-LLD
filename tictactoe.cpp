// tic_tac_toe_oop.cpp
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include <string>
#include <cctype>

using namespace std;

// --------------------- Board ---------------------
class Board {
public:
    Board() { cells.fill('\0'); }

    bool makeMove(int idx, char sym) {
        if (idx < 0 || idx >= 9 || cells[idx] != '\0') return false;
        cells[idx] = sym;
        return true;
    }

    void undoMove(int idx) {
        if (idx >= 0 && idx < 9) cells[idx] = '\0';
    }

    bool isFull() const {
        return all_of(cells.begin(), cells.end(), [](char c){ return c != '\0'; });
    }

    // Returns 'X' or 'O' if someone won, else '\0'
    char winner() const {
        static const int W[8][3] = {
            {0,1,2},{3,4,5},{6,7,8},
            {0,3,6},{1,4,7},{2,5,8},
            {0,4,8},{2,4,6}
        };
        for (auto &w : W) {
            char a = cells[w[0]];
            if (a && a == cells[w[1]] && a == cells[w[2]]) return a;
        }
        return '\0';
    }

    vector<int> availableMoves() const {
        vector<int> mv;
        for (int i = 0; i < 9; ++i) if (cells[i] == '\0') mv.push_back(i);
        return mv;
    }

    char at(int idx) const { return cells[idx]; }

    void print() const {
        auto cell = [&](int i){ return cells[i] ? cells[i] : ' '; };
        cout << "\n";
        cout << " " << cell(0) << " | " << cell(1) << " | " << cell(2) << "      (1 | 2 | 3)\n";
        cout << "---+---+---\n";
        cout << " " << cell(3) << " | " << cell(4) << " | " << cell(5) << "      (4 | 5 | 6)\n";
        cout << "---+---+---\n";
        cout << " " << cell(6) << " | " << cell(7) << " | " << cell(8) << "      (7 | 8 | 9)\n\n";
    }

private:
    array<char,9> cells;
};

// --------------------- Player (interface) ---------------------
class Player {
public:
    explicit Player(char symbol) : sym(symbol) {}
    virtual ~Player() = default;
    char symbol() const { return sym; }
    virtual int getMove(const Board& board) = 0; // returns index [0..8]
protected:
    char sym;
};

// --------------------- HumanPlayer ---------------------
class HumanPlayer : public Player {
public:
    using Player::Player;
    int getMove(const Board& board) override {
        (void)board; // not needed here, but keeps signature consistent
        while (true) {
            cout << "Your move (" << sym << "). Enter cell [1-9]: ";
            int c;
            if (!(cin >> c)) {
                return -1; // EOF/invalid stream
            }
            if (1 <= c && c <= 9) return c - 1;
            cout << "Invalid input. Try again.\n";
        }
    }
};

// --------------------- AIPlayer (Minimax + alpha-beta) ---------------------
class AIPlayer : public Player {
public:
    AIPlayer(char symbol, char opp) : Player(symbol), opponent(opp) {}

    int getMove(const Board& board) override {
        // Choose best move via minimax
        int bestIdx = -1;
        int bestVal = numeric_limits<int>::min() / 2;

        // If center is open, often a good heuristic to check first
        vector<int> moves = board.availableMoves();
        // Simple move ordering: center, corners, edges
        vector<int> order = {4,0,2,6,8,1,3,5,7};
        vector<int> ordered;
        for (int m : order) if (find(moves.begin(), moves.end(), m) != moves.end()) ordered.push_back(m);

        Board b = board;
        for (int idx : ordered) {
            b.makeMove(idx, sym);
            int val = minimax(b, /*depth=*/0, /*isMax=*/false,
                              numeric_limits<int>::min()/2, numeric_limits<int>::max()/2);
            b.undoMove(idx);
            if (val > bestVal) {
                bestVal = val;
                bestIdx = idx;
            }
        }
        return bestIdx;
    }

private:
    char opponent;

    // Static evaluation:
    // +10 for AI win, -10 for opponent win, 0 otherwise. Add depth tie-breaks.
    int evaluate(const Board& b, int depth) const {
        char w = b.winner();
        if (w == sym)      return 10 - depth; // prefer faster wins
        if (w == opponent) return depth - 10; // prefer slower losses
        return 0;
    }

    int minimax(Board& b, int depth, bool isMax, int alpha, int beta) const {
        int eval = evaluate(b, depth);
        if (eval != 0) return eval;
        if (b.availableMoves().empty()) return 0; // draw

        if (isMax) {
            int best = numeric_limits<int>::min() / 2;
            for (int mv : b.availableMoves()) {
                b.makeMove(mv, sym);
                best = max(best, minimax(b, depth + 1, false, alpha, beta));
                b.undoMove(mv);
                alpha = max(alpha, best);
                if (beta <= alpha) break; // prune
            }
            return best;
        } else {
            int best = numeric_limits<int>::max() / 2;
            for (int mv : b.availableMoves()) {
                b.makeMove(mv, opponent);
                best = min(best, minimax(b, depth + 1, true, alpha, beta));
                b.undoMove(mv);
                beta = min(beta, best);
                if (beta <= alpha) break; // prune
            }
            return best;
        }
    }
};

// --------------------- Game ---------------------
class Game {
public:
    Game(Player* pX, Player* pO) : px(pX), po(pO), turn('X') {}

    void chooseStarter(char c) {
        c = toupper(c);
        if (c == 'X' || c == 'O') turn = c;
    }

    void run() {
        board.print();
        while (true) {
            Player* current = (turn == 'X') ? px : po;
            int moveIdx = current->getMove(board);

            // For Human: also check legality
            if (moveIdx < 0 || moveIdx >= 9 || !board.makeMove(moveIdx, current->symbol())) {
                // Re-prompt humans; AI never gives illegal moves
                if (dynamic_cast<HumanPlayer*>(current) != nullptr) {
                    cout << "Invalid move. Try again.\n";
                    continue;
                } else {
                    // Safety guard for AI (shouldn't occur)
                    moveIdx = fallbackMove();
                    board.makeMove(moveIdx, current->symbol());
                }
            }

            if (dynamic_cast<AIPlayer*>(current) != nullptr) {
                cout << "AI (" << current->symbol() << ") plays at cell " << (moveIdx + 1) << ".\n";
            }

            board.print();

            char w = board.winner();
            if (w) {
                if (w == px->symbol())
                    cout << "Winner: Player X (" << w << ") 🎉\n";
                else
                    cout << "Winner: Player O (" << w << ") 🎉\n";
                break;
            }
            if (board.isFull()) {
                cout << "It's a draw!\n";
                break;
            }
            switchTurn();
        }
    }

private:
    Board board;
    Player *px, *po;
    char turn;

    void switchTurn() { turn = (turn == 'X') ? 'O' : 'X'; }

    int fallbackMove() const {
        for (int i = 0; i < 9; ++i) if (board.at(i) == '\0') return i;
        return 0;
    }
};

// --------------------- main ---------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== Tic-Tac-Toe (OOP + Minimax with Alpha-Beta) ===\n";

    // Choose human symbol
    char humanSym;
    cout << "Choose your symbol (X/O). X moves first: ";
    while (true) {
        if (!(cin >> humanSym)) return 0;
        humanSym = toupper(humanSym);
        if (humanSym == 'X' || humanSym == 'O') break;
        cout << "Please enter X or O: ";
    }
    char aiSym = (humanSym == 'X') ? 'O' : 'X';

    // Who starts
    char starter = 'X';
    cout << "Who plays first? (X/O) [default X]: ";
    cin >> ws;
    string tmp;
    getline(cin, tmp);
    if (!tmp.empty()) {
        char c = toupper(tmp[0]);
        if (c == 'X' || c == 'O') starter = c;
    }

    // Construct players
    HumanPlayer human(humanSym);
    AIPlayer ai(aiSym, humanSym);

    Player *pX = (humanSym == 'X') ? static_cast<Player*>(&human) : static_cast<Player*>(&ai);
    Player *pO = (humanSym == 'O') ? static_cast<Player*>(&human) : static_cast<Player*>(&ai);

    Game game(pX, pO);
    game.chooseStarter(starter);
    game.run();

    cout << "Thanks for playing!\n";
    return 0;
}
