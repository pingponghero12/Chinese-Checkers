from PyQt5.QtWidgets import QWidget, QPushButton, QVBoxLayout, QLabel
from PyQt5.QtCore import Qt

class MainMenu(QWidget):
    def __init__(self, switch_to_lobbies, show_about):
        super().__init__()
        self.switch_to_lobbies = switch_to_lobbies
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

        btn_about = QPushButton("About")
        btn_about.clicked.connect(self.show_about)
        layout.addWidget(btn_about)

        self.setLayout(layout)
