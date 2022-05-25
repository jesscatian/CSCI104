#include "scrabble.h"

#include "formatting.h"
#include <iomanip>
#include <iostream>
#include <map>

using namespace std;

// Given to you. this does not need to be changed
Scrabble::Scrabble(const ScrabbleConfig& config)
        : hand_size(config.hand_size),
          minimum_word_length(config.minimum_word_length),
          tile_bag(TileBag::read(config.tile_bag_file_path, config.seed)),
          board(Board::read(config.board_file_path)),
          dictionary(Dictionary::read(config.dictionary_file_path)) {}

// Adds players to the scrabble game
void Scrabble::add_players() {
    // Query user for number of players
    cout << "Please enter number of players: ";
    cin >> num_players;
    cin.ignore();

    // If the number of players is not between [1,8], throw an exception
    if (cin.fail() || num_players < 1 || num_players > 8) {
        throw FileException("PLAYERS");
    }

    // Confirm to user the number of players
    cout << num_players << " players confirmed." << endl;

    // Query each player for name, create the player, give player initial tiles, confirm player added
    for (int i = 0; i < num_players; i++) {
        cout << "Please enter name for player " << i + 1 << ": ";
        string player_name;
        getline(cin, player_name);

        auto new_player = make_shared<HumanPlayer>(player_name, hand_size);
        players.push_back(new_player);

        auto new_player_tiles = tile_bag.remove_random_tiles(hand_size);
        players[i]->add_tiles(new_player_tiles);

        cout << "Player " << i + 1 << ", named \"" << player_name << "\" has been added." << endl;
    }
}

// Game Loop should cycle through players and get and execute that players move
// until the game is over.
void Scrabble::game_loop() {
    bool game_over = false;
    int consecutive_passes = 0;
    while (game_over == false) {

        // Plays scrabbles looping through all the players
        for (int i = 0; i < players.size(); i++) {
            if (game_over == true) {
                break;
            }

            // Show player current state of the board
            board.print(cout);

            // Query player for turn
            Move player_move = players[i]->get_move(board, dictionary);

            // If the move was PLACE
            if (player_move.kind == MoveKind::PLACE) {
                consecutive_passes = 0;

                // Place the move
                auto place_results = board.place(player_move);

                // Add the points from the move
                int new_points = 0;
                players[i]->add_points(place_results.points);
                new_points += place_results.points;
                if (player_move.tiles.size() == hand_size) {
                    players[i]->add_points(50);
                    new_points += 50;
                }

                // Show player the points
                cout << "You gained " << SCORE_COLOR << new_points << rang::style::reset << " points!" << endl;

                // Remove the placed tiles
                players[i]->remove_tiles(player_move.tiles);

                // Give the player new tiles
                // If the tile bag has more tiles than the placed tiles give new tiles equal to placed tiles, otherwise
                // give the rest of tiles
                if (tile_bag.count_tiles() > player_move.tiles.size()) {
                    auto new_tiles = tile_bag.remove_random_tiles(player_move.tiles.size());
                    players[i]->add_tiles(new_tiles);
                } else {
                    auto new_tiles = tile_bag.remove_random_tiles(tile_bag.count_tiles());
                    players[i]->add_tiles(new_tiles);
                }
            }

            // If the move was EXCHANGE
            if (player_move.kind == MoveKind::EXCHANGE) {
                consecutive_passes = 0;

                // Remove exchanged tiles
                players[i]->remove_tiles(player_move.tiles);

                // Add exchanged tiles to tile bag
                for (int j = 0; j < player_move.tiles.size(); j++) {
                    tile_bag.add_tile(player_move.tiles[j]);
                }

                // Give the player new tiles
                // If the tile bag has more tiles than the exchanged tiles give new tiles equal to exchanged tiles,
                // otherwise give the rest of tiles
                if (tile_bag.count_tiles() > player_move.tiles.size()) {
                    auto new_tiles = tile_bag.remove_random_tiles(player_move.tiles.size());
                    players[i]->add_tiles(new_tiles);
                } else {
                    auto new_tiles = tile_bag.remove_random_tiles(tile_bag.count_tiles());
                    players[i]->add_tiles(new_tiles);
                }
            }

            // If the move was PASS
            if (player_move.kind == MoveKind::PASS) {
                consecutive_passes++;
            }

            // Show the current score and wait for enter
            cout << "Your current score: " << SCORE_COLOR << players[i]->get_points() << rang::style::reset << endl;
            cout << endl << "Press [enter] to continue.";
            cin.ignore();

            // Check game over conditions
            if (players[i]->count_tiles() == 0 && tile_bag.count_tiles() == 0) {
                game_over = true;
            }
            if (consecutive_passes == num_players) {
                game_over = true;
            }
        }
    }
}

// Performs final score subtraction. Players lose points for each tile in their
// hand. The player who cleared their hand receives all the points lost by the
// other players.
void Scrabble::final_subtraction(vector<shared_ptr<Player>>& plrs) {
    int index_of_winner = -1;
    int all_points = 0;
    for (int i = 0; i < plrs.size(); i++) {
        all_points += plrs[i]->get_hand_value();
        plrs[i]->subtract_points(plrs[i]->get_hand_value());
        if (plrs[i]->count_tiles() == 0) {
            index_of_winner = i;
        }
    }

    if (index_of_winner != -1) {
        plrs[index_of_winner]->add_points(all_points);
    }

    // Do not change the method signature.
}

// You should not need to change this function.
void Scrabble::print_result() {
    // Determine highest score
    size_t max_points = 0;
    for (auto player : this->players) {
        if (player->get_points() > max_points) {
            max_points = player->get_points();
        }
    }

    // Determine the winner(s) indexes
    vector<shared_ptr<Player>> winners;
    for (auto player : this->players) {
        if (player->get_points() >= max_points) {
            winners.push_back(player);
        }
    }

    cout << (winners.size() == 1 ? "Winner:" : "Winners: ");
    for (auto player : winners) {
        cout << SPACE << PLAYER_NAME_COLOR << player->get_name();
    }
    cout << rang::style::reset << endl;

    // now print score table
    cout << "Scores: " << endl;
    cout << "---------------------------------" << endl;

    // Justify all integers printed to have the same amount of character as the high score, left-padding with spaces
    cout << setw(static_cast<uint32_t>(floor(log10(max_points) + 1)));

    for (auto player : this->players) {
        cout << SCORE_COLOR << player->get_points() << rang::style::reset << " | " << PLAYER_NAME_COLOR
             << player->get_name() << rang::style::reset << endl;
    }
}

// You should not need to change this.
void Scrabble::main() {
    add_players();
    game_loop();
    final_subtraction(this->players);
    print_result();
}
