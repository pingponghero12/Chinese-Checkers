import math
from PyQt5.QtGui import QPolygon, QColor
from PyQt5.QtCore import QPoint

class Hexagon:
    def __init__(self, center_x, center_y, size, player, row, col):
        self.center_x = center_x
        self.center_y = center_y
        self.row = row
        self.col = col
        self.size = size
        self.color = None
        self.previous_color = self.color
        self.polygon = self.create_polygon()
        self.highlight = False;
        self.player = player;
        self.color_mapping = {
            -1: QColor(240, 240, 240), # White
            0: QColor(255, 0, 0),      # Red
            1: QColor(0, 255, 0),      # Green
            2: QColor(0, 0, 255),      # Blue
            3: QColor(255, 255, 0),    # Yellow
            4: QColor(255, 0, 255),    # Magenta
            5: QColor(0, 255, 255)     # Cyan
        }
        self.toggle_color("normal")

    def create_polygon(self):
        hexagon = QPolygon()
        for i in range(6):
            angle_deg = 60 * i - 30
            angle_rad = math.radians(angle_deg)
            x = self.center_x + self.size * math.cos(angle_rad)
            y = self.center_y + self.size * math.sin(angle_rad)
            point = QPoint(int(x), int(y))
            hexagon.append(point)
        return hexagon

    def toggle_color(self, color):
        print(f"toggle_color {self.color} player {self.player}")
        if color == "normal":
            self.color = self.color_mapping[self.player]
        if color == "choosen":
            self.color = QColor(0, 0, 0)
        if color == "highlight":
            self.color = QColor(128, 128, 128)

