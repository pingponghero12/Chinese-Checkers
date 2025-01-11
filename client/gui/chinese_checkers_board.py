from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPainter, QColor, QBrush
from PyQt5.QtCore import QPoint, Qt
from hexagon import Hexagon
import math

class ChineseCheckersBoard(QWidget):
    def __init__(self):
        super().__init__()
        self.setMinimumSize(600, 600)  # Set a minimum size for the board
        self.hex_size = 30
        self.hexagons = []  # List to store all hexagons
        self.hex_table = {}  # Dictionary to store hexagons in table
        self.hovered_hexagon = None  # Currently hovered hexagon
        self.create_hexagons()
        self.setMouseTracking(True)  # Enable mouse tracking for hover events

    def create_hexagons(self):
        """
        Initializes the hexagons and stores them in a list.
        Also organizes them in a table-like structure (nested lists) based on rows and columns.
        """
        self.hexagons.clear()
        self.hex_table.clear()

        center_x = self.width() // 2
        center_y = self.height() // 2

        for row in range(-5, 6):
            for col in range(-5, 6):
                if abs(row + col) > 5:
                    continue
                x = center_x + 1.5 * self.hex_size * (3/2 * col)
                y = center_y + 1.5 * self.hex_size * (math.sqrt(3) * (row + col / 2))
                hexagon = Hexagon(x, y, self.hex_size)
                self.hexagons.append(hexagon)
                # Store hexagons in a dictionary with row and col as keys
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
        click_point = event.pos()
        for hexagon in self.hexagons:
            if hexagon.polygon.containsPoint(click_point, Qt.OddEvenFill):
                self.handle_hex_click(hexagon)
                break  # Assuming one hexagon is clicked at a time

    def handle_hex_click(self, hexagon):
        """
        Handles the logic when a hexagon is clicked.

        :param hexagon: The Hexagon object that was clicked.
        """
        # Toggle the hexagon's color
        hexagon.toggle_color()

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
