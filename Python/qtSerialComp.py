##
#   In The Terminal install pySerial
#     pip install pyserial
#   In The Terminal install PyQt6
#     pip install PyQt6
#   In The Terminal install qt-designer
#    pip install pyqt6-tools
#
import serial
import serial.tools.list_ports
import threading
import json
from PyQt6.QtCore import QObject,pyqtSlot,QCoreApplication
from PyQt6 import QtCore
import sys
import warnings
warnings.filterwarnings('ignore')  # remove warning for python v3.10.8 (DeprecationWarning: sipPyTypeDict())

class JsonSerialConnector(threading.Thread,QObject):
    serialPort = None  # PySerial Com Port Object opened in the constructor
    serialDataRxSignal = QtCore.pyqtSignal(dict)
    def printSerialPorts(self):
        print('List of available Serial Ports:')
        ports = serial.tools.list_ports.comports()
        for port, desc, hwid in sorted(ports):
            print('serialPort : ' + port)

    def __init__(self,serialPortNumber):
      #QObject.__init__(self)
      super(QObject, self).__init__()
      #Init threading
      threading.Thread.__init__(self)
      self.number = 1
      self.running = True

      if (serialPortNumber == ''):
            print("The JsonSerialConnector(serialPort)\n serialPort was not specified !")
            print('Please Select one of the comports\nIf you do not know try the last one :)')
            self.printSerialPorts()
            exit(1)
      try:
            self.serialPort = serial.Serial(serialPortNumber)  # open serial port
      except:
            print("Could not connect to " + serialPortNumber)
            print('Please Select one of the serialPort listed below\nIf you do not know try the last one :)')
            self.printSerialPorts()
            exit(1)
      self.start()

    #Thread for reading serial lines
    def run(self):
        while self.running :
            if(self.serialPort.inWaiting()>0):
                serialInputStringAsLine = self.serialPort.readline()
                serialInputAsDictionary = json.loads(serialInputStringAsLine)
                self.serialDataRxSignal.emit(serialInputAsDictionary)

    def sendMessage(self,dictMessage):
        message=json.dumps(dictMessage)
        self.serialPort.write(message.encode('utf-8'))
        #self.serialPort.write(b'{\"userLedOn\": 1}\r\n')

if __name__ == "__main__":
    class TestRecieverClass(QObject):
        def __init__(self):
            super().__init__()

        # Slot to handle the signal from a sender
        @pyqtSlot(dict)
        def handleJsonSignal(self, message):
            print(f"RecieverClass: Received  message: {message}")

    app = QCoreApplication(sys.argv)

    jsonConnector=JsonSerialConnector("COM51")    #Connect to serial port

    # Create and connect Class that listens for JSON information
    recvClass = TestRecieverClass()
    jsonConnector.serialDataRxSignal.connect(recvClass.handleJsonSignal)

    #Send message to turn led on
    jsonConnector.sendMessage({"userLedOn": 1})

    sys.exit(app.exec())

    #print("Bye")
    #minBryter.running= False
    #del minBryter


