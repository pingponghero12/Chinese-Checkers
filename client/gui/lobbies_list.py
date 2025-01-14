from PyQt5.QtWidgets import QWidget, QPushButton, QVBoxLayout, QLabel, QListWidget, QMessageBox
from PyQt5.QtCore import Qt

class LobbiesList(QWidget):
    def __init__(self, switch_to_game, switch_to_main, create_game):
        super().__init__()
        self.switch_to_game = switch_to_game
        self.switch_to_main = switch_to_main
        self.create_game = create_game
        self.list_widget = QListWidget()

        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()

        title = QLabel("Available Lobbies")
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet("font-size: 20px;")
        layout.addWidget(title)

        # Lobby List
        layout.addWidget(self.list_widget)

        # Buttons
        btn_join = QPushButton("Join Lobby")
        btn_join.clicked.connect(self.join_lobby)
        layout.addWidget(btn_join)

        btn_create = QPushButton("Create Game")
        btn_create.clicked.connect(self.create_game)
        layout.addWidget(btn_create)

        btn_back = QPushButton("Back to Main Menu")
        btn_back.clicked.connect(self.switch_to_main)
        layout.addWidget(btn_back)

        self.setLayout(layout)

    def join_lobby(self):
        selected = self.list_widget.currentItem().text()
        selected = selected.split(" - ")
        selected = selected[0]
        print(f"selected {selected}")
        if selected:
            lobby_name = selected
            self.switch_to_game(lobby_name)
        else:
            QMessageBox.warning(self, "No Selection", "Please select a lobby to join.")
