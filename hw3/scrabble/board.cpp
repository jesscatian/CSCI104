#include "board.h"
#include "board_square.h"
#include "exceptions.h"
#include "formatting.h"
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

bool Board::Position::operator==(const Board::Position& other) const {
    return this->row == other.row && this->column == other.column;
}

bool Board::Position::operator!=(const Board::Position& other) const {
    return this->row != other.row || this->column != other.column;
}

Board::Position Board::Position::translate(Direction direction) const { return this->translate(direction, 1); }

Board::Position Board::Position::translate(Direction direction, ssize_t distance) const {
    if (direction == Direction::DOWN) {
        return Board::Position(this->row + distance, this->column);
    } else {
        return Board::Position(this->row, this->column + distance);
    }
}

// Board parser
Board Board::read(const string& file_path) {
    ifstream file(file_path);
    // If you cannot open the file throw an exception
    if (!file) {
        throw FileException("cannot open board file!");
    }

    // Set rows, columns, starting_row, and starting_column
    size_t rows;
    size_t columns;
    size_t starting_row;
    size_t starting_column;
    file >> rows >> columns >> starting_row >> starting_column;
    // Create a board
    Board board(rows, columns, starting_row, starting_column);

    // Ignores the \n becuase getline gets everything until \n
    file.ignore();

    string input_string;
    char input_char;
    // Read each row from the file
    while (getline(file, input_string)) {
        vector<BoardSquare> row;
        stringstream ss(input_string);
        // Read each character from the row
        while (ss >> input_char) {
            if (input_char == '.') {
                BoardSquare normal(1, 1);
                row.push_back(normal);
            }
            if (input_char == '2') {
                BoardSquare double_letter(2, 1);
                row.push_back(double_letter);
            }
            if (input_char == '3') {
                BoardSquare triple_letter(3, 1);
                row.push_back(triple_letter);
            }
            if (input_char == 'd') {
                BoardSquare double_word(1, 2);
                row.push_back(double_word);
            }
            if (input_char == 't') {
                BoardSquare triple_word(1, 3);
                row.push_back(triple_word);
            }
        }
        // Push each row into the matrix
        board.squares.push_back(row);
    }

    return board;
}

size_t Board::get_move_index() const { return this->move_index; }

// Testing a PLACE move
PlaceResult Board::test_place(const Move& move) const {
    // Set the starting position
    Position starting_position(move.row, move.column);

    // Check if the starting position has a tile
    if (in_bounds_and_has_tile(starting_position)) {
        return PlaceResult("Your starting position already has a tile on it!");
    }

    // Check to see if any tiles would be out of bound
    Position moving_cursor = starting_position;
    for (int i = 0; i < move.tiles.size();) {
        if (in_bounds_and_has_tile(moving_cursor)) {
            moving_cursor = moving_cursor.translate(move.direction);
            continue;
        }
        if (is_in_bounds(moving_cursor) == false) {
            return PlaceResult("One or more tiles is out of bounds!");
        }
        moving_cursor = moving_cursor.translate(move.direction);
        i++;
    }

    // Check if the first move is on the start tile
    moving_cursor = starting_position;
    if (move_index == 0) {
        bool on_start = false;
        for (int i = 0; i < move.tiles.size(); i++) {
            if (moving_cursor == start) {
                on_start = true;
            }
            moving_cursor = moving_cursor.translate(move.direction);
        }
        if (on_start != true) {
            return PlaceResult("The first move must contain the start square!");
        }
    }

    // Check for at least one adjacent tile
    moving_cursor = starting_position;
    bool adjacent = false;
    for (int i = 0; i < move.tiles.size(); i++) {
        if (has_adjacent(moving_cursor) == true) {
            adjacent = true;
        }
        moving_cursor = moving_cursor.translate(move.direction);
    }
    if (adjacent != true && move_index > 0) {
        return PlaceResult("At least one tile must be adjacent to a previously placed tile!");
    }

    // Get the points and words
    vector<string> words;
    string word;
    int word_points = 0;
    int word_bonus = 1;
    int total_points = 0;

    // Start from the word
    moving_cursor = starting_position;
    for (int i = 0; i < move.tiles.size();) {

        // Skip over occupied tiles
        if (in_bounds_and_has_tile(moving_cursor)) {
            word_points += at(moving_cursor).get_tile_kind().points;
            word.push_back(at(moving_cursor).get_tile_kind().letter);
            moving_cursor = moving_cursor.translate(move.direction);
            continue;
        }

        Position running_cursor = moving_cursor;

        // Check is first letter is attached to anything
        if (i == 0) {
            while (in_bounds_and_has_tile(running_cursor.translate(move.direction, -1))) {
                running_cursor = running_cursor.translate(move.direction, -1);
            }
            while (running_cursor != moving_cursor) {
                word_points += at(running_cursor).get_tile_kind().points;
                word += at(running_cursor).get_tile_kind().letter;
                running_cursor = running_cursor.translate(move.direction);
            }
        }

        // Check if word creates any accidental words
        running_cursor = moving_cursor;
        if (has_adjacent(moving_cursor)
            && (in_bounds_and_has_tile(moving_cursor.translate(!move.direction, -1))
                || in_bounds_and_has_tile(moving_cursor.translate(!move.direction, 1)))) {
            string accident_word;
            int accident_points = 0;  // for each accident word
            int accident_bonus = 1;
            while (in_bounds_and_has_tile(running_cursor.translate(!move.direction, -1))) {
                running_cursor = running_cursor.translate(!move.direction, -1);
            }
            while (in_bounds_and_has_tile(running_cursor) || running_cursor == moving_cursor) {

                if (running_cursor == moving_cursor) {
                    accident_points += move.tiles[i].points * at(moving_cursor).letter_multiplier;
                    accident_bonus *= at(moving_cursor).word_multiplier;

                    accident_word += move.tiles[i].letter;

                } else {
                    accident_points += at(running_cursor).get_tile_kind().points;
                    accident_word += at(running_cursor).get_tile_kind().letter;
                }
                running_cursor = running_cursor.translate(!move.direction);
            }

            if (accident_word.size() > 1) {
                words.push_back(accident_word);

                accident_points *= accident_bonus;
                total_points += accident_points;

                accident_points = 0;
            }
        }

        // Iterate through word
        word_points += move.tiles[i].points * at(moving_cursor).letter_multiplier;
        word_bonus *= at(moving_cursor).word_multiplier;

        if (move.tiles[i].letter == '?') {
            word += move.tiles[i].assigned;
        } else {
            word += move.tiles[i].letter;
        }

        running_cursor = moving_cursor;
        // Check if last letter is attached to anything
        if (i == move.tiles.size() - 1) {
            running_cursor = running_cursor.translate(move.direction);
            while (in_bounds_and_has_tile(running_cursor)) {
                word_points += at(running_cursor).get_tile_kind().points;
                word += at(running_cursor).get_tile_kind().letter;
                running_cursor = running_cursor.translate(move.direction);
            }
        }
        moving_cursor = moving_cursor.translate(move.direction);
        i++;
    }

    if (word.size() > 1) {
        word_points *= word_bonus;
        total_points += word_points;
        words.push_back(word);
    }

    return PlaceResult(words, total_points);
}

// Placing a PLACE move
PlaceResult Board::place(const Move& move) {
    PlaceResult result = test_place(move);

    if (result.valid == true) {
        // Traverse word
        Position moving_cursor(move.row, move.column);
        for (int i = 0; i < move.tiles.size();) {
            if (in_bounds_and_has_tile(moving_cursor)) {
                moving_cursor = moving_cursor.translate(move.direction);
                continue;
            }
            squares[moving_cursor.row][moving_cursor.column].set_tile_kind(move.tiles[i]);
            moving_cursor = moving_cursor.translate(move.direction);
            i++;
        }
        move_index++;
    }

    return result;
}

// The rest of this file is provided for you. No need to make changes.

BoardSquare& Board::at(const Board::Position& position) { return this->squares.at(position.row).at(position.column); }

const BoardSquare& Board::at(const Board::Position& position) const {
    return this->squares.at(position.row).at(position.column);
}

bool Board::is_in_bounds(const Board::Position& position) const {
    return position.row < this->rows && position.column < this->columns;
}

bool Board::in_bounds_and_has_tile(const Position& position) const {
    return is_in_bounds(position) && at(position).has_tile();
}

// Checks if a tile has at least one adjacent tile
bool Board::has_adjacent(const Position& position) const {
    Position down = position.translate(Direction::DOWN, 1);
    Position up = position.translate(Direction::DOWN, -1);
    Position right = position.translate(Direction::ACROSS, 1);
    Position left = position.translate(Direction::ACROSS, -1);
    return in_bounds_and_has_tile(down) || in_bounds_and_has_tile(up) || in_bounds_and_has_tile(right)
           || in_bounds_and_has_tile(left);
}

void Board::print(ostream& out) const {
    // Draw horizontal number labels
    for (size_t i = 0; i < BOARD_TOP_MARGIN - 2; ++i) {
        out << std::endl;
    }
    out << FG_COLOR_LABEL << repeat(SPACE, BOARD_LEFT_MARGIN);
    const size_t right_number_space = (SQUARE_OUTER_WIDTH - 3) / 2;
    const size_t left_number_space = (SQUARE_OUTER_WIDTH - 3) - right_number_space;
    for (size_t column = 0; column < this->columns; ++column) {
        out << repeat(SPACE, left_number_space) << std::setw(2) << column + 1 << repeat(SPACE, right_number_space);
    }
    out << std::endl;

    // Draw top line
    out << repeat(SPACE, BOARD_LEFT_MARGIN);
    print_horizontal(this->columns, L_TOP_LEFT, T_DOWN, L_TOP_RIGHT, out);
    out << endl;

    // Draw inner board
    for (size_t row = 0; row < this->rows; ++row) {
        if (row > 0) {
            out << repeat(SPACE, BOARD_LEFT_MARGIN);
            print_horizontal(this->columns, T_RIGHT, PLUS, T_LEFT, out);
            out << endl;
        }

        // Draw insides of squares
        for (size_t line = 0; line < SQUARE_INNER_HEIGHT; ++line) {
            out << FG_COLOR_LABEL << BG_COLOR_OUTSIDE_BOARD;

            // Output column number of left padding
            if (line == 1) {
                out << repeat(SPACE, BOARD_LEFT_MARGIN - 3);
                out << std::setw(2) << row + 1;
                out << SPACE;
            } else {
                out << repeat(SPACE, BOARD_LEFT_MARGIN);
            }

            // Iterate columns
            for (size_t column = 0; column < this->columns; ++column) {
                out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL;
                const BoardSquare& square = this->squares.at(row).at(column);
                bool is_start = this->start.row == row && this->start.column == column;

                // Figure out background color
                if (square.word_multiplier == 2) {
                    out << BG_COLOR_WORD_MULTIPLIER_2X;
                } else if (square.word_multiplier == 3) {
                    out << BG_COLOR_WORD_MULTIPLIER_3X;
                } else if (square.letter_multiplier == 2) {
                    out << BG_COLOR_LETTER_MULTIPLIER_2X;
                } else if (square.letter_multiplier == 3) {
                    out << BG_COLOR_LETTER_MULTIPLIER_3X;
                } else if (is_start) {
                    out << BG_COLOR_START_SQUARE;
                }

                // Text
                if (line == 0 && is_start) {
                    out << "  \u2605  ";
                } else if (line == 0 && square.word_multiplier > 1) {
                    out << FG_COLOR_MULTIPLIER << repeat(SPACE, SQUARE_INNER_WIDTH - 2) << 'W' << std::setw(1)
                        << square.word_multiplier;
                } else if (line == 0 && square.letter_multiplier > 1) {
                    out << FG_COLOR_MULTIPLIER << repeat(SPACE, SQUARE_INNER_WIDTH - 2) << 'L' << std::setw(1)
                        << square.letter_multiplier;
                } else if (line == 1 && square.has_tile()) {
                    char l = square.get_tile_kind().letter == TileKind::BLANK_LETTER ? square.get_tile_kind().assigned
                                                                                     : ' ';
                    out << repeat(SPACE, 2) << FG_COLOR_LETTER << square.get_tile_kind().letter << l
                        << repeat(SPACE, 1);
                } else if (line == SQUARE_INNER_HEIGHT - 1 && square.has_tile()) {
                    out << repeat(SPACE, SQUARE_INNER_WIDTH - 1) << FG_COLOR_SCORE << square.get_points();
                } else {
                    out << repeat(SPACE, SQUARE_INNER_WIDTH);
                }
            }

            // Add vertical line
            out << FG_COLOR_LINE << BG_COLOR_NORMAL_SQUARE << I_VERTICAL << BG_COLOR_OUTSIDE_BOARD << std::endl;
        }
    }

    // Draw bottom line
    out << repeat(SPACE, BOARD_LEFT_MARGIN);
    print_horizontal(this->columns, L_BOTTOM_LEFT, T_UP, L_BOTTOM_RIGHT, out);
    out << endl << rang::style::reset << std::endl;
}
