from PyQt5.QtWidgets import QWidget, QMessageBox
from PyQt5.QtGui import QPainter, QColor, QBrush
from PyQt5.QtCore import QPoint, Qt
from hexagon import Hexagon
import math

class ChineseCheckersBoard(QWidget):
    """
    @brief Represents the game board for Chinese Checkers.

    This class inherits from QWidget and manages the graphical representation
    of the Chinese Checkers board, including hexagons for each position on the
    board and handling user interactions.
    """

    def __init__(self, my_id, send_move):
        """
        @brief Initializes the Chinese Checkers board.

        @param my_id The player's ID to identify the current player.
        @param send_move The function to call for sending moves to the game.
        """
        super().__init__()
        self.setMinimumSize(600, 600)  # Set a minimum size for the board
        self.hex_size = 15
        self.hexagons = []  # List to store all hexagons
        self.hex_table = {}  # Dictionary to store hexagons in table
        self.hovered_hexagon = None  # Currently hovered hexagon
        self.send_move = send_move
        self.create_hexagons()
        self.setMouseTracking(True)
        self.choosen = None
        self.current_player = 0
        self.my_id = my_id
        self.highlighted = [(0, 12)]  # Highlight the starting position

    def create_hexagons(self):
        """
        @brief Initializes the hexagons on the board.

        This method creates hexagons and organizes them in a table-like structure
        (nested lists) based on rows and columns. It also sets the initial state
        of the game board.
        """
        self.hexagons.clear()
        self.hex_table.clear()

        state = [[-1] * 25 for _ in range(17)]  # Initialize the state of the board
        state[8][12] = 0  # Set some initial positions
        state[8][14] = 0

        center_x = self.width() // 2
        center_y = self.height() // 2

        total_layers = 17  # Total number of layers of hexagons

        for row in range(total_layers):
            # Determine starting and ending columns for hexagons in the current row
            if row < 4:
                start_col = 12 - row
                end_col = 12 + row + 1
            elif row <= 8:
                start_col = row - 4
                end_col = 25 - (row - 4)
            elif row <= 12:
                start_col = 4 - (row - 8)
                end_col = 12 + (row - 8) + 9
            else:
                start_col = 12 - (16 - row)
                end_col = 12 + (16 - row) + 1

            # Create hexagons for the specified columns in the current row
            for col in range(start_col, end_col, 2):
                x = center_x + 1.5 * self.hex_size * (col - 12)
                y = center_y + 3 * self.hex_size * (row - 8) * math.sqrt(3) / 2
                hexagon = Hexagon(x, y, self.hex_size, state[row][col], row, col)
                self.hexagons.append(hexagon)

                self.hex_table[(row, col)] = hexagon

    def paintEvent(self, event):
        """
        @brief Paints the hexagons on the board.

        This method is called whenever the widget needs to be redrawn, and it
        redraws all hexagons using the QPainter.
        
        @param event The paint event.
        """
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        # Redraw all hexagons
        for hexagon in self.hexagons:
            painter.setBrush(QBrush(hexagon.color))
            painter.drawPolygon(hexagon.polygon)

    def mousePressEvent(self, event):
        """
        @brief Handles mouse press events to detect clicks on hexagons.

        This method checks if a hexagon was clicked and performs actions 
        based on the current player and selected hexagon.

        @param event The mouse event.
        """
        if self.my_id == self.current_player:
            click_point = event.pos()
            for hexagon in self.hexagons:
                if hexagon.polygon.containsPoint(click_point, Qt.OddEvenFill):
                    self.handle_hex_click(hexagon)
                    break  # Assuming one hexagon is clicked at a time
        else:
            QMessageBox.warning(self, "Wrong turn", "Please wait for your turn.")

    def unhighlight(self):
        """
        @brief Unhighlights all currently highlighted hexagons.

        This method resets the color of highlighted hexagons and clears the 
        highlighted list.
        """
        self.highlighted = []  # Clear highlighted positions
        for i in self.highlighted:
            self.hex_table[i].toggle_color("normal")
            self.hex_table[i].highlight = False

    def highlight(self):
        """
        @brief Highlights all hexagons specified in the highlighted list.

        This method changes the color of hexagons in the highlighted list
        to indicate they can be selected for movement.
        """
        for i in self.highlighted:
            self.hex_table[i].toggle_color("highlight")
            self.hex_table[i].highlight = True

    def handle_hex_click(self, hexagon):
        """
        @brief Handles actions when a hexagon is clicked.

        This method performs actions based on which hexagon was clicked,
        including selecting a hexagon for movement and sending the move if valid.

        @param hexagon The hexagon that was clicked.
        """
        if hexagon.player == self.my_id:  # Check if the hexagon belongs to the current player
            if self.choosen is not None:
                self.hex_table[self.choosen].toggle_color("normal")
                self.unhighlight()

            self.choosen = (hexagon.row, hexagon.col)  # Select hexagon
            self.hex_table[self.choosen].toggle_color("choosen")
            self.highlight()

            self.dupa()  # Debug or log clicked hexagon

            # Update the display
            self.update()
        else:
            if self.choosen is None:
                QMessageBox.warning(self, "Wrong selection", "Please choose your color.")
            else:
                if (hexagon.row, hexagon.col) in self.highlighted:
                    mv = [self.choosen[0], self.choosen[1], hexagon.row, hexagon.col]
                    self.choosen = None
                    self.unhighlight()
                    self.send_move(mv)

    def move(self, mv):
        """
        @brief Handles the movement of a piece on the board.

        This method updates the board state by moving a piece from one hexagon
        to another based on the provided move parameters.

        @param mv A list containing the coordinates of the move: 
                  [from_row, from_col, to_row, to_col].
        """
        print(mv)

        self.hex_table[(mv[0], mv[1])].player = -1  # Remove player from start position
        self.hex_table[(mv[0], mv[1])].toggle_color("normal")

        self.hex_table[(mv[2], mv[3])].player = self.current_player  # Assign player to end position
        print(f"move play {self.hex_table[(mv[2], mv[3])].player} {self.current_player}")
        self.hex_table[(mv[2], mv[3])].toggle_color("normal")

        self.current_player = (self.current_player + 1) % 6  # Update to the next player

        self.update()  # Refresh the display

    def dupa(self):
        """
        @brief Placeholder for handling hexagon clicks with debug output.

        This method currently just prints a message indicating a hexagon
        has been clicked.
        """
        print("Hexagon clicked")

    def hoverMoveEvent(self, event):
        """
        @brief Handles hover movement to highlight hexagons under the cursor.

        This method changes the color of hexagons when the mouse hovers over them,
        providing visual feedback to the user.
        
        @param event The hover event.
        """
        hover_point = event.pos()
        for hexagon in self.hexagons:
            if hexagon.polygon.containsPoint(hover_point, Qt.OddEvenFill):
                if self.hovered_hexagon != hexagon:
                    if self.hovered_hexagon:
                        # Reset color of the previously hovered hexagon
                        self.hovered_hexagon.color = self.hovered_hexagon.previous_color
                    # Store previous color
                    hexagon.previous_color = hexagon.color
                    # Highlight color
                    hexagon.color = QColor(150, 150, 150)
                    self.hovered_hexagon = hexagon
                    self.update()
                break
        else:
            if self.hovered_hexagon:
                # Reset color if no hexagon is hovered
                self.hovered_hexagon.color = self.hovered_hexagon.previous_color
                self.hovered_hexagon = None
                self.update()

    def resizeEvent(self, event):
        """
        @brief Handles the resize event of the widget.

        This method is called when the widget is resized and adjusts the
        positions of hexagons accordingly.
        
        @param event The resize event.
        """
        self.create_hexagons()  # Recreate hexagons based on new size
        self.update()           # Refresh the board