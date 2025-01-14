from PyQt5.QtWidgets import QWidget, QMessageBox
from PyQt5.QtGui import QPainter, QColor, QBrush
from PyQt5.QtCore import QPoint, Qt
from hexagon import Hexagon
import math

class ChineseCheckersBoard(QWidget):
    def __init__(self, my_id):
        super().__init__()
        self.setMinimumSize(600, 600)  # Set a minimum size for the board
        self.hex_size = 15
        self.hexagons = []  # List to store all hexagons
        self.hex_table = {}  # Dictionary to store hexagons in table
        self.hovered_hexagon = None  # Currently hovered hexagon
        self.create_hexagons()
        self.setMouseTracking(True)
        self.choosen = None
        self.current_player = 0
        self.my_id = my_id

    def create_hexagons(self):
        """
        Initializes the hexagons and stores them in a list.
        Also organizes them in a table-like structure (nested lists) based on rows and columns.
        """
        self.hexagons.clear()
        self.hex_table.clear()

        state = [[-1] * 25 for _ in range(17)]
        state[8][12] = 0
        state[8][14] = 0

        center_x = self.width() // 2
        center_y = self.height() // 2

        total_layers = 17

        for row in range(total_layers):
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

            for col in range(start_col, end_col, 2):
                x = center_x + 1.5 * self.hex_size * (col - 12)
                y = center_y + 3 * self.hex_size * (row - 8) * math.sqrt(3) / 2
                hexagon = Hexagon(x, y, self.hex_size, state[row][col], row, col)
                self.hexagons.append(hexagon)

                self.hex_table[(row, col)] = hexagon

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        # Redraw all hexagons
        for hexagon in self.hexagons:
            painter.setBrush(QBrush(hexagon.color))
            painter.drawPolygon(hexagon.polygon)

    def mousePressEvent(self, event):
        """
        Handles mouse press events to detect clicks on hexagons.

        :param event: The mouse event.
        """
        if self.my_id == self.current_player:
            click_point = event.pos()
            for hexagon in self.hexagons:
                if hexagon.polygon.containsPoint(click_point, Qt.OddEvenFill):
                    self.handle_hex_click(hexagon)
                    break  # Assuming one hexagon is clicked at a time
        else :
            QMessageBox.warning(self, "Wrong turn", "Pls wait your turn")

    def unhighlight(self):
        highlighted = [] # test
        for i in highlighted:
            hex_table[i].toggle_color("normal")
            hex_table[i].highlight = False

    def highlight(self):
        highlighted = [] # test
        for i in highlighted:
            hex_table[i].toggle_color("highlight")
            hex_table[i].highlight = True

    def handle_hex_click(self, hexagon):
        """
        Handles the logic when a hexagon is clicked.

        :param hexagon: The Hexagon object that was clicked.
        """
        if (self.current_player != hexagon.player):
            QMessageBox.warning(self, "Wrong selection", "Pls choose your color")
            return

        if (self.choosen != None):
            self.hex_table[self.choosen].toggle_color("normal")
            self.unhighlight()

        self.choosen = (hexagon.row, hexagon.col)
        self.hex_table[self.choosen].toggle_color("choosen")

        # Call the dupa() function
        self.dupa()

        # Update the display
        self.update()

    def dupa(self):
        """
        Example function to be called when a hexagon is clicked.
        """
        print("Hexagon clicked! Function dupa() executed.")

    def hoverMoveEvent(self, event):
        """
        Handles hover movement to highlight hexagons under the cursor.
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
            Handles the resize event to recenter the hexagon board when the widget is resized.
            """
            self.create_hexagons()  # Recreate hexagons based on new size
            self.update()           # Refresh the board
