import sys
from PyQt5.QtWidgets import QApplication, QWidget, QStackedWidget, QVBoxLayout, QMessageBox, QInputDialog
from PyQt5.QtCore import pyqtSignal, QObject
from main_menu import MainMenu
from lobbies_list import LobbiesList
from game_window import GameWindow
from about_dialog import AboutDialog
import time

import client_module

class Communicator(QObject):
    """!
    @brief Signal handler class for server messages
    
    Provides message passing functionality between the network thread and GUI
    """
    message = pyqtSignal(str)  ##< Signal emitted when a message is received from server

class MainWindow(QWidget):
    """!
    @brief Main application window class handling game client functionality
    
    Manages the main game interface, networking, and navigation between different screens
    """
    def __init__(self):
        """!
        @brief Initialize the main window and establish server connection
        @throws SystemExit if unable to connect to server
        """
        super().__init__()
        self.setWindowTitle("Chinese Checkers")
        self.setGeometry(100, 100, 800, 600)
        self.my_id = None
        self.pg = None
        self.init_ui()

        self.communicator = Communicator()
        self.communicator.message.connect(self.handle_server_message)

        self.client = client_module.Client("127.0.0.1", 8080)
        self.client.set_message_callback(self.on_message_received)

        if not self.client.connect():
            error_box = QMessageBox()
            error_box.setIcon(QMessageBox.Critical)
            error_box.setText("Could not connect to server.")
            error_box.setWindowTitle("Connection Error")
            error_box.exec_()
            sys.exit(1)

        self.client.start_receiving()

    def init_ui(self):
        """!
        @brief Initialize the user interface components and layout
        """
        self.stack = QStackedWidget()

        self.main_menu = MainMenu(self.show_lobbies, self.show_about)
        self.lobbies_list = LobbiesList(self.join_game, self.show_main_menu, self.create_game)
        self.game_window = None # Will be created when join lobby

        self.stack.addWidget(self.main_menu)
        self.stack.addWidget(self.lobbies_list)

        layout = QVBoxLayout()
        layout.addWidget(self.stack)
        self.setLayout(layout)

    def show_main_menu(self):
        """!
        @brief Switch to the main menu screen
        """
        self.stack.setCurrentWidget(self.main_menu)

    def show_lobbies(self):
        """!
        @brief Switch to the lobby list screen and request lobby list from server
        """
        print("show lobby py")
        self.client.send_message("list")
        self.stack.setCurrentWidget(self.lobbies_list)

    def show_about(self):
        """!
        @brief Display the about dialog
        """
        about = AboutDialog()
        about.exec_()

    def join_game(self, lobby_name):
        """!
        @brief Join an existing game lobby
        @param lobby_name The name of the lobby to join
        """
        self.client.send_message(f"join {lobby_name}")

        time.sleep(0.1)
        if self.game_window:
            self.stack.removeWidget(self.game_window)
        self.game_window = GameWindow(lobby_name, self.leave_game, self.get_my_id, self.get_pg, self.send_move, self.client.possible_moves, self.client.board_state)
        self.stack.addWidget(self.game_window)
        self.stack.setCurrentWidget(self.game_window)

    def send_move(self, mv):
        """!
        @brief Send a move to the server
        @param mv List containing move coordinates [x1, y1, x2, y2]
        """
        self.client.send_message(f"move {mv[0]} {mv[1]} {mv[2]} {mv[3]}")

    def create_game(self):
        """!
        @brief Create a new game lobby for 6 players
        """

        number, ok = QInputDialog.getInt(self, 
                                        "Number Input", 
                                        "Enter a number:",
                                        min=2,
                                        max=6)

        self.client.send_message(f"create {number}")
        time.sleep(0.1)

        if self.game_window:
            self.stack.removeWidget(self.game_window)

        self.game_window = GameWindow("my game", self.leave_game, self.get_my_id, self.get_pg, self.send_move, self.client.possible_moves, self.client.board_state)
        self.stack.addWidget(self.game_window)
        self.stack.setCurrentWidget(self.game_window)

    def leave_game(self):
        """!
        @brief Leave the current game and return to lobby list
        """
        self.client.send_message("exit")
        self.show_lobbies()
        self.my_id = None

    def get_pg(self):
        return self.pg

    def get_my_id(self):
        return self.my_id

    def on_message_received(self, message):
        """!
        @brief Handle incoming messages from server
        @param message The message received from server
        """
        self.communicator.message.emit(message)

    def handle_server_message(self, message):
        """!
        @brief Process server messages and update UI accordingly
        @param message The message to process
        """
        if message.startswith("lobbies:"):
            lobby_data = message.split(":", 1)[1]
            lobbies = lobby_data.split(",")

            self.lobbies_list.list_widget.clear()
            for lobby in lobbies:
                self.lobbies_list.list_widget.addItem(lobby)

        if message.startswith("joined"):
            self.my_id = int(message[7]) - 1
            self.pg = int(message[6])

        if message.startswith("move,"):
            mv = message.split(",", 1)[1]
            mv = mv.split(",")
            mv = [int(x) for x in mv]
            self.game_window.board.move(mv)

    def closeEvent(self, event):
        """!
        @brief Handle window close event
        @param event The close event
        """
        self.client.disconnect()
        event.accept()

def main():
    """!
    @brief Main application entry point
    """
    app = QApplication(sys.argv)

    window = MainWindow()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
