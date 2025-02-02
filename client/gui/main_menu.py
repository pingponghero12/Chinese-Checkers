from PyQt5.QtWidgets import QWidget, QPushButton, QVBoxLayout, QLabel
from PyQt5.QtCore import Qt

"""
MainMenu class is a QWidget that represents the main menu of the game.
It allows the player to show available lobbies and display information about the application.
"""
class MainMenu(QWidget):
    def __init__(self, switch_to_lobbies, show_about, switch_to_old_games):
        super().__init__()
        self.switch_to_lobbies = switch_to_lobbies
        self.switch_to_old_games = switch_to_old_games
        self.show_about = show_about
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()

        title = QLabel("Chinese Checkers")
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet("font-size: 24px;")
        layout.addWidget(title)

        # Buttons
        btn_show_lobbies = QPushButton("Show Lobbies")
        btn_show_lobbies.clicked.connect(self.switch_to_lobbies)
        layout.addWidget(btn_show_lobbies)

        btn_show_old_games = QPushButton("Show Old Games")
        btn_show_old_games.clicked.connect(self.switch_to_old_games)
        layout.addWidget(btn_show_old_games)

        btn_about = QPushButton("About")
        btn_about.clicked.connect(self.show_about)
        layout.addWidget(btn_about)

        self.setLayout(layout)
