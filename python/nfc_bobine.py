#!/usr/bin/python

"""
by Riccardo BRAMATI Piedmont/Veneto World
"""
from PyQt6.QtGui import * # QAction, QIcon
from PyQt6.QtCore import * # Qt, QSize, pyqtSlot
from PyQt6.QtWidgets import * # 
from paho.mqtt import client as mqtt_client
import sys
import re
import base64
import nfc_bobine_lib 

broker = '192.168.1.101'
port = 1883
topic = 'rfid'
# generate client ID with pub prefix randomly
client_id = ""

class NFC_Bobine(QMainWindow):

    def __init__(self):
        super(NFC_Bobine,self).__init__()
        self.setGeometry(50,50,800,450)
        self.setWindowTitle("Klipper NFC-Bobine")
        self.initUI()
        self.show()

    def initUI(self):
        self.main_layout = QVBoxLayout()
        # Menu
        self.menu_bar = self.menuBar()
        self.file_menu = QMenu("&File", self)
        self.setting_menu = QMenu("&Setting", self)
        self.help_menu = QMenu("&Help", self)
        self.menu_bar.addMenu(self.file_menu)
        self.menu_bar.addMenu(self.setting_menu)
        self.menu_bar.addMenu(self.help_menu)
        # StatusBar
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)
        # Adding a permanent message
        self.permanent_status_message = QLabel("Press Connection button to start MQTT service")
        self.status_bar.addPermanentWidget(self.permanent_status_message)
        # Mqtt buttons define
        # Connection
        self.mqtt_connection = QPushButton("&Connection!")
        self.pixmap_connection = getattr(QStyle.StandardPixmap, 'SP_DesktopIcon')
        self.icon_connection = self.style().standardIcon(self.pixmap_connection)
        self.mqtt_connection.setCheckable(True)
        self.mqtt_connection.clicked.connect(self.the_button_was_clicked)
        self.mqtt_connection.setIcon(self.icon_connection)
        self.mqtt_connection.setFixedSize(QSize(100,50))
        # Write to Ntag
        self.mqtt_write = QPushButton("&Write")
        self.pixmap_write_nfc = getattr(QStyle.StandardPixmap, 'SP_ArrowDown')
        self.icon_write_nfc = self.style().standardIcon(self.pixmap_write_nfc)
        self.mqtt_write.setEnabled(False)
        self.mqtt_write.clicked.connect(self.sub_button_was_clicked)
        self.mqtt_write.setIcon(self.icon_write_nfc)
        self.mqtt_write.setFixedSize(QSize(100,50))
        # Read from Ntag
        self.mqtt_read = QPushButton("&Read")
        self.pixmap_read_nfc = getattr(QStyle.StandardPixmap, 'SP_ArrowUp')
        self.icon_read_nfc = self.style().standardIcon(self.pixmap_read_nfc)
        self.mqtt_read.setEnabled(False)
        self.mqtt_read.clicked.connect(self.pub_button_was_clicked)
        self.mqtt_read.setIcon(self.icon_read_nfc)
        self.mqtt_read.setFixedSize(QSize(100,50))
        # Cancel and format Ntag TODO
        self.mqtt_cancel = QPushButton("C&ancel")
        self.mqtt_format = QPushButton("&Format")
        # File Dialog
        self.filedialog_button = QPushButton("Open file dialog")
        self.pixmap_file_dialog = getattr(QStyle.StandardPixmap,'SP_FileDialogStart')
        self.icon_file_dialog = self.style().standardIcon(self.pixmap_file_dialog)
        self.filedialog_button.clicked.connect(self.open_dialog)
        self.filedialog_button.setIcon(self.icon_file_dialog)
        self.filedialog_button.setFixedSize(QSize(100,50))
        self.gcodefilename_label = QLabel("GCode file name :")
        self.gcode_preview_small = QLabel("Small Preview")
        self.gcode_preview_small.setGeometry(30,30,30,30)
        self.slicer_label = QLabel("Slicer :")
        self.preview_button = QPushButton("Preview")
        self.preview_button.setEnabled(False)
        self.preview_button.clicked.connect(self.open_preview)
        # Defining labels for display Ntag Data; names are explicitly
        self.uid_label = QLabel("UID = ")
        self.material_label = QLabel("Material = ")
        self.color_label = QLabel("Color = ")
        self.print_T_label = QLabel("Print T° = ")
        self.print_bed_label = QLabel("Bed T° = ")
        self.print_flT_label = QLabel("First Layer T° = ")
        self.print_flbed_label = QLabel("First Layer Bed T° = ")
        self.lenght_label = QLabel("Lenght = ")
        self.weigth_label = QLabel("Weigth = ")
        # defining of frame 
        # panel mqtt
        mqtt_frame = QFrame(self)
        mqtt_frame.setFrameShape(QFrame.Shape.Panel)
        mqtt_frame.setFrameShadow(QFrame.Shadow.Raised)
        mqtt_frame.setLineWidth(2)
        # panel file choice dialog
        file_choice_frame = QFrame(self)
        file_choice_frame.setFrameShape(QFrame.Shape.Panel)
        file_choice_frame.setFrameShadow(QFrame.Shadow.Raised)
        file_choice_frame.setLineWidth(2)
        # panel label
        nfc_tag = QFrame(self)
        nfc_tag.setFrameShape(QFrame.Shape.Panel)
        nfc_tag.setFrameShadow(QFrame.Shadow.Raised)  
        nfc_tag.setLineWidth(2)

        left_layout = QVBoxLayout(mqtt_frame)
        left_layout.addWidget(self.mqtt_connection)
        left_layout.addWidget(self.mqtt_write)
        left_layout.addWidget(self.mqtt_read)
        left_layout.addWidget(self.mqtt_cancel)
        left_layout.addWidget(self.mqtt_format)

        right_layout = QVBoxLayout(file_choice_frame)
        right_layout.addWidget(self.filedialog_button)
        right_layout.addWidget(self.gcodefilename_label)
        right_layout.addWidget(self.slicer_label)
        right_layout.addWidget(self.gcode_preview_small)
        right_layout.addWidget(self.preview_button)
        bottom_layout = QVBoxLayout(nfc_tag)
        bottom_layout.addWidget(self.uid_label)
        bottom_layout.addWidget(self.material_label)
        bottom_layout.addWidget(self.color_label)
        bottom_layout.addWidget(self.print_T_label)
        bottom_layout.addWidget(self.print_bed_label)
        bottom_layout.addWidget(self.print_flT_label)
        bottom_layout.addWidget(self.print_flbed_label)
        bottom_layout.addWidget(self.weigth_label)
        bottom_layout.addWidget(self.lenght_label)

        splitter1 = QSplitter(Qt.Orientation.Horizontal)
        splitter1.addWidget(mqtt_frame)
        splitter1.addWidget(file_choice_frame)

        splitter2 = QSplitter(Qt.Orientation.Vertical)
        splitter2.addWidget(splitter1)
        splitter2.addWidget(nfc_tag)

        self.main_layout.addWidget(splitter2)

        self.v_widget = QWidget()
        self.v_widget.setLayout(self.main_layout)
        self.setCentralWidget(self.v_widget)


    def pub_button_was_clicked(self) :
        self.client.publish("rfid", "read")
        
    def sub_button_was_clicked(self) :
        self.client.publish("rfid", "write")
        
    def the_button_was_clicked(self) :
        self.mqtt_read.setEnabled(True)
        self.mqtt_write.setEnabled(True)
        self.mqtt_connection.setEnabled(False)
        self.client = self.connect_mqtt()
        self.permanent_status_message.setText("Connected to MQTT Broker!")
        self.client.on_message = self.on_message
        self.client.subscribe(topic)
        self.client.loop_start()
        
    def on_message(self, client, userdata, msg):
        description =  msg.payload.decode('utf-8')
        match_str = description.split("=")
        match_str = match_str[0]
        match_str1= "   ddd"
        print(match_str)
        match match_str :
            case 'material ':
                stringa = self.material_label.text()
                self.material_label.setText(stringa + match_str1)
                print("1 "+ stringa + match_str1)
            case 'color ':
                self.color_label.setText(description)
                print("2 "+ description)
            case 'print_temp ':
                self.print_T_label.setText(description)
                print("3 "+ description)
            case 'bed_temp ':
                self.print_bed_label.setText(description)
                print("4 "+ description)
            case 'f_layer_t ':
                self.print_flT_label.setText(description)
                print("5 "+ description)
            case 'f_layer_bd ':
                self.print_flbed_label.setText(description)
                print("6 "+ description)
            case 'weight ':
                self.lenght_label.setText(description)
                print("7 "+ description)
            case 'length ':
                self.weigth_label.setText(description)
                print("8 "+ description)
            case _:
                self.uid_label.setText(description)
                print("0 "+ description)

    def open_preview(self):
        print("QMessage")
        dialog = QDialog()#(parent=NFC_Bobine, text="Hello World")
        dialog.setGeometry(QRect(100,100,400,300))

        dialog.setWindowTitle("Message Dialog")
        label = QLabel(dialog)
        pixmap = QPixmap('preview.png')
        label.setPixmap(pixmap)
        
        dialog.exec() 
     
    @pyqtSlot()
    def open_dialog(self):
        fname = QFileDialog.getOpenFileName(
            self,
            "Open File",
            "",
            "GCode Files (*.gcode)", #;; PNG Files (*.png)",
        )
        global file_choiced
        global slicer
        global list
        file_choiced = fname[0]
        self.gcodefilename_label.setText("GCode file name : " + file_choiced)
        file1 = open(fname[0], "r")
        lines = file1.readlines()
        slicer = nfc_bobine_lib.which_slicer(file_choiced)
        print("Slicer : " + slicer)
        ista = nfc_bobine_lib.extacting_data(file_choiced)
        print(ista)
        self.slicer_label.setText("Slicer :" + slicer)
        nfc_bobine_lib.thumbarnail_large(file_choiced)
        self.preview_button.setEnabled(True)
        print("Sono tornato")


    @staticmethod
    def connect_mqtt() -> mqtt_client:
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to MQTT Broker!")
            else:
                print("Failed to connect, return code %d\n", rc)

        client = mqtt_client.Client(client_id)
        client.on_connect = on_connect
        client.connect(broker, port)
        return client


def main():

    app = QApplication(sys.argv)
    ex = NFC_Bobine()
    sys.exit(app.exec())


if __name__ == '__main__':
    main()
