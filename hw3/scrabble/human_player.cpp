#include "human_player.h"

#include "exceptions.h"
#include "formatting.h"
#include "move.h"
#include "place_result.h"
#include "rang.h"
#include "tile_kind.h"
#include <algorithm>
#include <ctype.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

// This method is fully implemented.
inline string& to_upper(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Returns a valid move that can be executed (PLACE, EXCHANGE, PASS)
Move HumanPlayer::get_move(const Board& board, const Dictionary& dictionary) const {
    // Show user their hand
    print_hand(cout);
    cout << endl;

    // Query for move
    cout << "Your move, " << this->get_name() << ": ";
    string user_input;
    getline(cin, user_input);
    user_input = to_upper(user_input);

    // Parse the move
    try {
        Move user_move = parse_move(user_input);
        // PLACE
        if (user_move.kind == MoveKind::PLACE) {
            // Test the move
            PlaceResult test_results = board.test_place(user_move);
            if (test_results.valid == true) {
                // Check that all the words from the move are valid
                for (int i = 0; i < test_results.words.size(); i++) {
                    if (dictionary.is_word(test_results.words[i]) == false) {
                        cerr << "Invalid word created" << endl;
                        return get_move(board, dictionary);
                    }
                }
            } else {
                throw CommandException(test_results.error);
            }
        }

        return user_move;
    } catch (CommandException& exception) {
        cerr << "Please retry" << endl;
        cerr << exception.what() << endl;
        return get_move(board, dictionary);
    }
}

// Takes a input string and returns vector of TileKind (points, letter, assigned)
vector<TileKind> HumanPlayer::parse_tiles(string& letters) const {
    vector<TileKind> vector_of_tiles;

    // Check if you have the tiles, if you do push them into a vector
    for (int i = 0; i < letters.size(); i++) {
        try {

            if (letters[i] == '?') {
                TileKind tile = tiles.lookup_tile(letters[i]);
                char assigned_letter;
                assigned_letter = letters[i + 1];
                tile.assigned = tolower(assigned_letter);
                vector_of_tiles.push_back(tile);
                i++;
            } else {
                TileKind tile = tiles.lookup_tile(letters[i]);
                vector_of_tiles.push_back(tile);
            }

        } catch (out_of_range) {
            throw CommandException("You do not have some of these tiles!");
        }
    }

    return vector_of_tiles;
}

// Reads the user input and returns a Move
Move HumanPlayer::parse_move(string& move_string) const {
    // Get the move
    istringstream ss(move_string);
    string move_kind = "";
    ss >> move_kind;

    // PLACE
    if (move_kind == "PLACE") {
        // Get the direction
        string direction;
        ss >> direction;
        // Get the row
        int row;
        ss >> row;
        row -= 1;
        // Ger the column
        int col;
        ss >> col;
        col -= 1;

        // Get the place letters
        string place_letters;
        ss >> place_letters;
        vector<TileKind> place_tiles = parse_tiles(place_letters);

        // Return a move
        if (direction == "-") {
            Move place(place_tiles, row, col, Direction::ACROSS);
            return place;
        }
        if (direction == "|") {
            Move place(place_tiles, row, col, Direction::DOWN);
            return place;
        }
    }

    // EXCHANGE
    if (move_kind == "EXCHANGE") {
        string exchange_letters;
        ss >> exchange_letters;
        vector<TileKind> exchange_tiles = parse_tiles(exchange_letters);

        // Check if the number exchanged is between 1 to hand_size
        if (exchange_letters.size() < 1 || exchange_letters.size() > get_hand_size()) {
            throw CommandException("The tiles you want to exchange are not within your hand size!");
        }

        Move exchange(exchange_tiles);
        return exchange;
    }

    // PASS
    if (move_kind == "PASS") {
        return Move();
    }
}

// This function is fully implemented.
void HumanPlayer::print_hand(ostream& out) const {
    const size_t tile_count = tiles.count_tiles();
    const size_t empty_tile_count = this->get_hand_size() - tile_count;
    const size_t empty_tile_width = empty_tile_count * (SQUARE_OUTER_WIDTH - 1);

    for (size_t i = 0; i < HAND_TOP_MARGIN - 2; ++i) {
        out << endl;
    }

    out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_HEADING << "Your Hand: " << endl << endl;

    // Draw top line
    out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE;
    print_horizontal(tile_count, L_TOP_LEFT, T_DOWN, L_TOP_RIGHT, out);
    out << repeat(SPACE, empty_tile_width) << BG_COLOR_OUTSIDE_BOARD << endl;

    // Draw middle 3 lines
    for (size_t line = 0; line < SQUARE_INNER_HEIGHT; ++line) {
        out << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD << repeat(SPACE, HAND_LEFT_MARGIN);
        for (auto it = tiles.cbegin(); it != tiles.cend(); ++it) {
            out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL << BG_COLOR_PLAYER_HAND;

            // Print letter
            if (line == 1) {
                out << repeat(SPACE, 2) << FG_COLOR_LETTER << (char)toupper(it->letter) << repeat(SPACE, 2);

                // Print score in bottom right
            } else if (line == SQUARE_INNER_HEIGHT - 1) {
                out << FG_COLOR_SCORE << repeat(SPACE, SQUARE_INNER_WIDTH - 2) << setw(2) << it->points;

            } else {
                out << repeat(SPACE, SQUARE_INNER_WIDTH);
            }
        }
        if (tiles.count_tiles() > 0) {
            out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL;
            out << repeat(SPACE, empty_tile_width) << BG_COLOR_OUTSIDE_BOARD << endl;
        }
    }

    // Draw bottom line
    out << repeat(SPACE, HAND_LEFT_MARGIN) << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE;
    print_horizontal(tile_count, L_BOTTOM_LEFT, T_UP, L_BOTTOM_RIGHT, out);
    out << repeat(SPACE, empty_tile_width) << rang::style::reset << endl;
}
