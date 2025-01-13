import sys
from PyQt5.QtWidgets import QApplication, QWidget, QStackedWidget, QVBoxLayout
from main_menu import MainMenu
from lobbies_list import LobbiesList
from game_window import GameWindow
from about_dialog import AboutDialog

import client_module

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Chinese Checkers")
        self.setGeometry(100, 100, 800, 600)
        self.init_ui()

    def init_ui(self):
        self.stack = QStackedWidget()

        self.main_menu = MainMenu(self.show_lobbies, self.show_about)
        self.lobbies_list = LobbiesList(self.join_game, self.show_main_menu)
        self.game_window = None # Will be created when join lobby

        self.stack.addWidget(self.main_menu)
        self.stack.addWidget(self.lobbies_list)

        layout = QVBoxLayout()
        layout.addWidget(self.stack)
        self.setLayout(layout)

    def show_main_menu(self):
        self.stack.setCurrentWidget(self.main_menu)

    def show_lobbies(self):
        self.stack.setCurrentWidget(self.lobbies_list)

    def show_about(self):
        about = AboutDialog()
        about.exec_()

    def join_game(self, lobby_name):
        if self.game_window:
            self.stack.removeWidget(self.game_window)
        self.game_window = GameWindow(lobby_name, self.show_lobbies)
        self.stack.addWidget(self.game_window)
        self.stack.setCurrentWidget(self.game_window)

def main():
    app = QApplication(sys.argv)

    client = client_module.Client("127.0.0.1", 8080)

    if not client.connect():
        error_box = QMessageBox()
        error_box.setIcon(QMessageBox.Critical)
        error_box.setText("Could not connect to server.")
        error_box.setWindowTitle("Connection Error")
        error_box.exec_()
        sys.exit(1)

    window = MainWindow()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
