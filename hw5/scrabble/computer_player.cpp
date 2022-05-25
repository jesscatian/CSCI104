
#include "computer_player.h"

#include <iostream>
#include <memory>
#include <string>

using namespace std;

void ComputerPlayer::left_part(
        Board::Position anchor_pos,
        std::string partial_word,
        Move partial_move,
        std::shared_ptr<Dictionary::TrieNode> node,
        size_t limit,
        TileCollection& remaining_tiles,
        std::vector<Move>& legal_moves,
        const Board& board) const {

    // Call extend_right on empty prefix
    extend_right(anchor_pos, partial_word, partial_move, node, remaining_tiles, legal_moves, board);
    // Base case
    if (limit == 0) {
        return;
    }
    // Search all edges of root
    for (auto itr = node->nexts.begin(); itr != node->nexts.end(); itr++) {
        try {
            // If you one edge matches something in your hand
            TileKind found = remaining_tiles.lookup_tile(itr->first);
            remaining_tiles.remove_tile(found);
            partial_word.push_back(found.letter);
            partial_move.tiles.push_back(found);

            left_part(
                    anchor_pos,
                    partial_word,
                    partial_move,
                    itr->second,
                    limit - 1,
                    remaining_tiles,
                    legal_moves,
                    board);

            partial_word.pop_back();
            partial_move.tiles.pop_back();
            remaining_tiles.add_tile(found);
        } catch (out_of_range& e) {
        }
        // If any children are in your hands
        try {
            TileKind found = remaining_tiles.lookup_tile('?');
            remaining_tiles.remove_tile(found);

            partial_word.push_back(itr->first);   // a
            partial_move.tiles.push_back(found);  //?

            // extend_right(anchor_pos, partial_word, partial_move, itr->second, remaining_tiles, legal_moves, board);
            left_part(
                    anchor_pos,
                    partial_word,
                    partial_move,
                    itr->second,
                    limit - 1,
                    remaining_tiles,
                    legal_moves,
                    board);

            partial_word.pop_back();
            partial_move.tiles.pop_back();

            remaining_tiles.add_tile(found);

        } catch (out_of_range& e) {
        }
    }
}

void ComputerPlayer::extend_right(
        Board::Position square,
        std::string partial_word,
        Move partial_move,
        std::shared_ptr<Dictionary::TrieNode> node,
        TileCollection& remaining_tiles,
        std::vector<Move>& legal_moves,
        const Board& board) const {

    // Is prefix valid word?
    if (node->is_final) {
        //        partial_move.row = square.row;
        //        partial_move.column = square.column;
        legal_moves.push_back(partial_move);
    }
    // Base case
    if (!board.is_in_bounds(square)) {
        return;
    }

    // If square is vacant
    if (!board.in_bounds_and_has_tile(square)) {
        // Position gets bigger and bigger each recursion

        for (auto itr = node->nexts.begin(); itr != node->nexts.end(); itr++) {
            try {
                // If you one edge matches something in your hand
                TileKind found = remaining_tiles.lookup_tile(itr->first);
                remaining_tiles.remove_tile(found);
                partial_word.push_back(found.letter);
                partial_move.tiles.push_back(found);

                if (partial_move.direction == Direction::DOWN) {
                    square.row += 1;
                    extend_right(square, partial_word, partial_move, itr->second, remaining_tiles, legal_moves, board);
                } else {
                    square.column += 1;
                    extend_right(square, partial_word, partial_move, itr->second, remaining_tiles, legal_moves, board);
                }

                partial_word.pop_back();
                partial_move.tiles.pop_back();
                remaining_tiles.add_tile(found);
            } catch (out_of_range& e) {
            }
            // If any children are in your hands
            try {
                TileKind found = remaining_tiles.lookup_tile('?');
                remaining_tiles.remove_tile(found);

                partial_word.push_back(itr->first);   // a
                partial_move.tiles.push_back(found);  //?

                if (partial_move.direction == Direction::DOWN) {
                    square.row += 1;
                    extend_right(square, partial_word, partial_move, itr->second, remaining_tiles, legal_moves, board);
                } else {
                    square.column += 1;
                    extend_right(square, partial_word, partial_move, itr->second, remaining_tiles, legal_moves, board);
                }

                partial_word.pop_back();
                partial_move.tiles.pop_back();

                remaining_tiles.add_tile(found);

            } catch (out_of_range& e) {
            }
        }

    } else {
        char letter = board.letter_at(square);
        if (node->nexts.find(letter) != node->nexts.end()) {
            partial_word.push_back(letter);
            if (partial_move.direction == Direction::DOWN) {
                square.row += 1;
                extend_right(
                        square,
                        partial_word,
                        partial_move,
                        node->nexts.find(letter)->second,
                        remaining_tiles,
                        legal_moves,
                        board);
            } else {
                square.column += 1;
                extend_right(
                        square,
                        partial_word,
                        partial_move,
                        node->nexts.find(letter)->second,
                        remaining_tiles,
                        legal_moves,
                        board);
            }
        }
    }
}

Move ComputerPlayer::get_move(const Board& board, const Dictionary& dictionary) const {
    std::vector<Move> legal_moves;
    std::vector<Board::Anchor> anchors = board.get_anchors();

    for (int i = 0; i < anchors.size(); i++) {
        if (anchors[i].limit > 0) {
            Move blank_move({}, anchors[i].position.row, anchors[i].position.column, anchors[i].direction);
            TileCollection copy_tiles = tiles;
            left_part(
                    anchors[i].position,
                    "",
                    blank_move,
                    dictionary.get_root(),
                    anchors[i].limit,
                    copy_tiles,
                    legal_moves,
                    board);
        }
        if (anchors[i].limit == 0) {
            TileCollection copy_tiles = tiles;
            string partial_word = "";

            auto moving_cursor = anchors[i].position;
            while (board.in_bounds_and_has_tile(moving_cursor)) {
                moving_cursor = moving_cursor.translate(anchors[i].direction, -1);
                partial_word = board.letter_at(moving_cursor) + partial_word;
            }
            Move blank_move({}, anchors[i].position.row, anchors[i].position.column, anchors[i].direction);

            extend_right(
                    anchors[i].position,
                    partial_word,
                    blank_move,
                    dictionary.find_prefix(partial_word),
                    copy_tiles,
                    legal_moves,
                    board);
        }
    }
    return get_best_move(legal_moves, board, dictionary);
}

Move ComputerPlayer::get_best_move(
        std::vector<Move> legal_moves, const Board& board, const Dictionary& dictionary) const {
    Move best_move = Move();  // Pass if no move found

    int max_points = 0;
    for (int i = 0; i < legal_moves.size(); i++) {

        PlaceResult result = board.test_place(legal_moves[i]);
        // If you used all 7 tiles, you get a bonus 50 points
        if (legal_moves[i].tiles.size() == get_hand_size()) {
            result.points += 50;
        }
        if (result.points >= max_points) {
            max_points = result.points;
            best_move = legal_moves[i];
        }
    }

    return best_move;
}
