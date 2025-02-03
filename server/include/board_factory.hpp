#include <memory>
#include "board.hpp"
#include "standard_board.hpp"
#include "fast_board.hpp"

class BoardFactory {
public:
    static std::shared_ptr<Board> create_board(int board_type, int size) {
        if (board_type == 0) {
            return std::make_shared<Standard_Board>(size);
        } else if (board_type == 1) {
            return std::make_shared<Fast_Board>(size);
        }
        return nullptr;
    }
};
