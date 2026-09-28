from pymodbus.server import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext

def run_simulator():
    datablock = ModbusSequentialDataBlock(0, [11, 22, 33] + [0]*50)
    store = ModbusSlaveContext(hr=datablock)
    context = ModbusServerContext(slaves=store, single=True)

    print("="*50)
    print("✅ Modbus‑TCP从站模拟器启动成功")
    print("地址：127.0.0.1 端口：502")
    print("寄存器 R0=11，R1=22，R2=33")
    print("⚠️本窗口不要关闭！")
    print("="*50)
    StartTcpServer(context=context, address=("127.0.0.1",502))

if __name__ == "__main__":
    run_simulator()