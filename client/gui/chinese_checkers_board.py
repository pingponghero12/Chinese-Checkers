from PyQt5.QtWidgets import QWidget, QMessageBox
from PyQt5.QtGui import QPainter, QColor, QBrush
from PyQt5.QtCore import QPoint, Qt
from hexagon import Hexagon
import math

class ChineseCheckersBoard(QWidget):
    def __init__(self, my_id, send_move):
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
        self.highlighted = [(0, 12)]

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
        self.highlighted = [] # test
        for i in self.highlighted:
            self.hex_table[i].toggle_color("normal")
            self.hex_table[i].highlight = False

    def highlight(self):
        for i in self.highlighted:
            self.hex_table[i].toggle_color("highlight")
            self.hex_table[i].highlight = True

    def handle_hex_click(self, hexagon):
        if hexagon.player == self.my_id:
            if (self.choosen != None):
                self.hex_table[self.choosen].toggle_color("normal")
                self.unhighlight()

            self.choosen = (hexagon.row, hexagon.col)
            self.hex_table[self.choosen].toggle_color("choosen")
            self.highlight()

            self.dupa()

            # Update the display
            self.update()
        else:
            if (self.choosen == None):
                QMessageBox.warning(self, "Wrong selection", "Pls choose your color")
            else:
                if (hexagon.row, hexagon.col) in self.highlighted:
                    mv = [self.choosen[0], self.choosen[1], hexagon.row, hexagon.col]
                    self.choosen = None
                    self.unhighlight()
                    self.send_move(mv)

    def move(self, mv):
        print(mv)

        self.hex_table[(mv[0], mv[1])].player = -1
        self.hex_table[(mv[0], mv[1])].toggle_color("normal")

        self.hex_table[(mv[2], mv[3])].player = self.current_player
        print(f"mov play {self.hex_table[(mv[2], mv[3])].player} {self.current_player}")
        self.hex_table[(mv[2], mv[3])].toggle_color("normal")

        self.current_player = (self.current_player + 1) % 6

        self.update()

    def dupa(self):
        print("Hexagon clicked")

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
