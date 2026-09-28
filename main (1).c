#include <modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <windows.h>   /* Sleep() */

int main(void)
{
    modbus_t* ctx;
    uint16_t reg_read[3];
    int cnt = 0;

    /* 创建 Modbus-TCP 客户端，对应 Python 的 ModbusTcpClient("127.0.0.1", 502) */
    ctx = modbus_new_tcp("127.0.0.1", 502);
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create modbus context: %s\n", modbus_strerror(errno));
        return EXIT_FAILURE;
    }

    modbus_set_slave(ctx, 1);

    /* 想观察协议报文就把下面这行的注释去掉；嫌刷屏就保持注释 */
    /* modbus_set_debug(ctx, 1); */

    if (modbus_connect(ctx) == -1) {
        int wsa = WSAGetLastError();
        printf("connect failed: %s (WSA error = %d)\n", modbus_strerror(errno), wsa);
        printf("hint: please run 'python slave_sim.py' first\n");
        modbus_free(ctx);
        return EXIT_FAILURE;
    }

    printf("loop start, close this window to stop\n");

    /* 对应 Python 里的 while True 循环 */
    while (1) {
        /* 写：把计数器写进 0 号寄存器，对应 client.write_register(0, cnt) */
        if (modbus_write_register(ctx, 0, cnt) == -1) {
            printf("write failed: %s\n", modbus_strerror(errno));
            break;
        }

        /* 读：读回 0~2 号寄存器，对应 client.read_holding_registers(0, 3) */
        if (modbus_read_registers(ctx, 0, 3, reg_read) == -1) {
            printf("read failed: %s\n", modbus_strerror(errno));
            break;
        }

        printf("loop %d: R0=%u, R1=%u, R2=%u\n", cnt, reg_read[0], reg_read[1], reg_read[2]);
        cnt++;

        Sleep(1000);   /* 对应 Python 的 time.sleep(1)，注意单位是毫秒 */
    }

    /* 收尾，对应 client.close() */
    modbus_close(ctx);
    modbus_free(ctx);
    return EXIT_SUCCESS;
}
