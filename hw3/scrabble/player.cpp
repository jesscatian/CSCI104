#include "player.h"

using namespace std;

// Adds points to player's score
void Player::add_points(size_t points) { this->points += points; }

// Subtracts points from player's score
void Player::subtract_points(size_t points) {
    if (points > this->points) {
        this->points = 0;
    } else {
        this->points -= points;
    }
}

size_t Player::get_points() const { return points; }

const std::string& Player::get_name() const { return name; }

// Returns the number of tiles in a player's hand.
size_t Player::count_tiles() const { return tiles.count_tiles(); }

// Removes tiles from player's hand.
void Player::remove_tiles(const std::vector<TileKind>& tiles) {
    for (auto tile : tiles) {
        this->tiles.remove_tile(tile);
    }
}

// Adds tiles to player's hand.
void Player::add_tiles(const std::vector<TileKind>& tiles) {
    for (auto tile : tiles) {
        this->tiles.add_tile(tile);
    }
}

// Checks if player has a matching tile.
bool Player::has_tile(TileKind tile) {
    char tile_char = tile.letter;
    try {
        TileKind found = tiles.lookup_tile(tile_char);
        return true;
    } catch (exception& e) {
        return false;
    }
}

// Returns the total points of all tiles in the players hand.
unsigned int Player::get_hand_value() const { return tiles.total_points(); }

size_t Player::get_hand_size() const { return hand_size; }
