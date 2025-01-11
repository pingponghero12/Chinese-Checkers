from PyQt5.QtWidgets import QMessageBox

class AboutDialog(QMessageBox):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("About")
        self.setText("Chinese Checkers Game Client\nDeveloped by Manfred Gawlas and Tomasz Piotrowski.")
        self.setStandardButtons(QMessageBox.Ok)
