import math
from PyQt5.QtGui import QPolygon, QColor
from PyQt5.QtCore import QPoint

class Hexagon:
    def __init__(self, center_x, center_y, size):
        self.center_x = center_x
        self.center_y = center_y
        self.size = size
        self.color = QColor(200, 200, 200)
        self.previous_color = self.color
        self.polygon = self.create_polygon()

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

    def toggle_color(self):
        if self.color == QColor(200, 200, 200):
            self.color = QColor(255, 0, 0)
        elif self.color == QColor(255, 0, 0):
            self.color = QColor(0, 0, 255)
        else:
            self.color = QColor(200, 200, 200)
