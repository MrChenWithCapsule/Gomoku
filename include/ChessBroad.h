#include <array>
#include <exception>
#include <iosfwd>

constexpr int broad_size = 15;

struct Position
{
    int row;
    int column;
};

Position operator+(Position left, Position right);
Position operator-(Position left, Position right);
Position operator*(int n, Position pos);

enum class Chess : char
{
    empty = ' ',
    first_player = 'O',
    second_player = 'X'
};

std::ostream &operator<<(std::ostream &os, Chess ch);

class InvalidPosition : public std::exception
{
};

class ChessBroad
{
  private:
    std::array<std::array<ChessType, broad_size>, broad_size> _broad{};

  public:
    void emplace(Position pos, Chess ch);
    bool full() const;
    bool win_game(Position hint) const;
    Chess get(Position pos) const;
};

std::ostream &operator<<(std::ostream &os, const ChessBroad &broad);