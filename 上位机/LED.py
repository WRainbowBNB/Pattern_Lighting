import serial 
import LED_UI as ui
from PySide6.QtWidgets import QApplication, QMainWindow
import serial.tools.list_ports
from PySide6.QtCore import QTimer

class MainWindow(QMainWindow, ui.Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setupUi(self)
        self.LED_ON.clicked.connect(self.on_pushButton_clicked)
        self.LED_OFF.clicked.connect(self.off_pushButton_clicked)
        self.refresh_button.clicked.connect(self.port_refresh)
        self.cbox_port.currentIndexChanged.connect(self.choose_port)
        self.open_port.clicked.connect(self.connect_serial)
        self.Send.clicked.connect(self.send_count)
        #数据限幅，避免输超
        self.Count.setRange(0, 255)
        self.Count.setValue(0)
        self.count = self.Count.value()
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.receive_data)
        #小小定时器
        self.timer.start(100)
        self.ser = None
        self.serial_is_open = False
        #wok竟然不需要写类型和长度
        self.rx_buf = bytearray()
        self.selected_port = ""
        self.header1 = 0xFA
        self.header2 = 0xAF
        self.tail1 = 0xFB
        self.tail2 = 0xBF

    # 发送数据
    # command: 命令字节, 0x01 设置LED状态, 0x02 控灯闪烁次数
    # param: 参数字节, 0x01: 0x01 为ON, 0x00 为OFF 0x02: 为闪烁次数
    # checksum: 校验和
    def send_data(self, command, param):
        checksum = (self.header1 + self.header2 + command + param) & 0xFF
        data = [self.header1, self.header2, command, param, checksum, self.tail1, self.tail2]
        self.ser.write(bytes(data))
        print(f"发送数据: {bytes(data).hex(' ').upper()}")

    def on_pushButton_clicked(self):
        if not self.serial_is_open:
            print("请先打开串口")
            return
        else:
            self.send_data(0x01, 0x01)
            
    def off_pushButton_clicked(self):
        if not self.serial_is_open:
            print("请先打开串口")
            return
        else:
            self.send_data(0x01, 0x00)
            
    def port_refresh(self):
        # 刷新串口列表
        self.cbox_port.clear()
        ports = serial.tools.list_ports.comports()
        for port in ports:
            self.cbox_port.addItem(port.device)

    def choose_port(self):
        self.selected_port = self.cbox_port.currentText()
        print(f"选择的串口: {self.selected_port}")

    def connect_serial(self):
        if not self.serial_is_open:  
            try:
                self.ser = serial.Serial(self.selected_port, 115200, timeout=1) # 替换为your波特率   
                print(f"串口{self.selected_port}打开成功OHHHHHHHH")
                self.serial_is_open = True
                self.open_port.setText("关闭串口")
            except serial.SerialException as e:
                print(f"串口{self.selected_port}打开失败Awwwwwwwman: {e}")
                self.open_port.setText("打开串口")
        else:
            self.ser.close()
            print(f"串口{self.selected_port}已关闭")
            self.serial_is_open = False
            self.open_port.setText("打开串口")

    def send_count(self):
        if not self.serial_is_open:
            print("请先打开串口")
            return
        else:
            self.count = self.Count.value()
            self.send_data(0x02, self.count)
    
    def receive_data(self):
        if not self.serial_is_open:
            return
        #依旧缓存有无数据
        if self.ser.in_waiting > 0:
            #读数据
            data = self.ser.read(self.ser.in_waiting)
            self.rx_buf.extend(data)
            #解析数据
            while len(self.rx_buf) >= 7:
                if self.rx_buf[0] == 0xFC and self.rx_buf[1] == 0xCF or self.rx_buf[0] == 0xFA and self.rx_buf[1] == 0xAF:
                    frame = self.rx_buf[:7]
                    #计算校验位
                    check_sum = (frame[0] + frame[1] + frame[2] + frame[3]) & 0xFF
                    if check_sum == frame[4]:
                        if frame[5] == 0xFD and frame[6] == 0xDF or frame[5] == 0xFB and frame[6] == 0xBF:
                            print(f"收到数据：{bytes(frame).hex(' ').upper()}")
                            #去掉之前的七字节
                            del self.rx_buf[:7]
                        else:
                            #帧尾错误，丢弃当前帧
                            self.rx_buf.pop(0)
                    else:
                        #避免假帧头
                        self.rx_buf.pop(0)
                else:
                    #窗口后移一位
                    self.rx_buf.pop(0)
          
                         
if __name__ == '__main__':
    app = QApplication([])
    window = MainWindow()
    window.setWindowTitle('LED控制')
    window.show()
    app.exec()
    

