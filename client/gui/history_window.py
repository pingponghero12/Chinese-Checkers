from PyQt5.QtWidgets import QWidget, QPushButton, QVBoxLayout, QLabel, QHBoxLayout
from chinese_checkers_board import ChineseCheckersBoard

"""
GameWindow class is a QWidget that represents the main window of the game.
It contains the ChineseCheckersBoard widget and allows the player to exit the game.
"""
class HistoryWindow(QWidget):
    def __init__(self, lobby_name, switch_to_lobbies, my_id, pg, send_move, possible_moves, board_state):
        super().__init__()
        self.lobby_name = lobby_name
        self.switch_to_lobbies = switch_to_lobbies
        self.game_id = my_id
        self.pg = pg
        self.send_move = send_move
        self.possible_moves = possible_moves
        self.board_state = board_state
        self.init_ui()

        self.move_id = 0

    def init_ui(self):
        layout = QVBoxLayout()

        header = QHBoxLayout()
        lbl_lobby = QLabel(f"Lobby: {self.lobby_name}")
        lbl_lobby.setStyleSheet("font-size: 18px;")
        header.addWidget(lbl_lobby)

        header.addStretch()

        btn_exit = QPushButton("Exit")
        btn_exit.clicked.connect(self.switch_to_lobbies)
        header.addWidget(btn_exit)

        btn_next_move = QPushButton("Next Move")
        btn_next_move.clicked.connect(self.send_move_local)
        header.addWidget(btn_next_move)

        layout.addLayout(header)

        self.board = ChineseCheckersBoard(1, self.send_move, self.possible_moves, self.board_state, self.pg)
        layout.addWidget(self.board)

        self.setLayout(layout)

    def send_move_local(self):
        self.send_move(self.move_id, self.game_id())
        self.move_id += 1
