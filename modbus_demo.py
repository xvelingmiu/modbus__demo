from pymodbus.client import ModbusTcpClient
import time

client = ModbusTcpClient(host="127.0.0.1", port=502)
client.connect()

cnt = 0
try:
    print("循环读写开始，按 Ctrl+C 停止程序")
    while True:
        client.write_register(address=0, value=cnt)
        res = client.read_holding_registers(address=0,count= 1)
        if not res.isError():
            print(f"第{cnt}次循环，寄存器0={res.registers[0]}")
        cnt += 1
        time.sleep(1)
except KeyboardInterrupt:
    print("\n程序手动终止")
finally:
    client.close()