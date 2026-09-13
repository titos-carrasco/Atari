#include <cc65.h>
#include <conio.h>
#include <stdio.h>

#include "fujinet-network.h"

#include "main.h"
#include "proceed.h"

const char *server = "N:TCP://192.168.100.19:1963/";
uint8_t net_err;
uint16_t conn_bytes_waiting;
uint8_t conn_status;
uint8_t conn_err;
uint8_t buff[64];

int main() {
    clrscr();
    cursor(1);
    printf("* Simple Netcat " __DATE__ " " __TIME__ " *\n\n");

    if (connect() == 0) {
        run();
        disconnect();
    }

    return 0;
}

uint8_t connect() {
    printf("network_init(): ");
    net_err = network_init();
    if (net_err) {
        printf("error %u\n", net_err);
        return 1;
    }
    printf("OK\n");

    printf("network_open(): ");
    net_err = network_open(server, OPEN_MODE_RW, OPEN_TRANS_LF);
    if (net_err) {
        printf("error %u\n", net_err);
        return 1;
    }
    printf("OK\n");

    proceed_enable();
    return 0;
}

void disconnect() {
    proceed_restore();

    printf("\nnetwork_close(): ");
    net_err = network_close(server);
    if (net_err)
        printf("error %u\n", net_err);
    else
        printf("OK\n");
    return;
}

uint8_t run(void) {
    while (1) {
        int16_t nbytes;
        uint8_t i, b;

        // revisamos si se ha presionado alguna tecla
        if (kbhit()) {
            uint8_t key = cgetc();

            // damos echo
            putchar(key);

            // CTRL-C
            if (key == 0x03)
                return 0;

            // la enviamos
            if (network_write(server, &key, 1)) {
                printf("network_write(): error %u\n", fn_network_error);
                return 1;
            }
        }

        // verificamos si es que llego algo
        if (proceed_check() == 0)
            continue;

        // el status
        net_err = network_status(server, &conn_bytes_waiting, &conn_status,
                                 &conn_err);
        // printf("%u - %u - %u - %u\n", net_err, conn_bytes_waiting,
        // conn_status, conn_err);

        // fix? conn_status es 1 cuando esta todo ok
        if (conn_status == 0) {
            printf("network_status(): error %u\n", net_err);
            return 1;
        }

        // mostramos todo lo que llego
        while (1) {
            nbytes = network_read_nb(server, buff, sizeof(buff));
            if (nbytes < 0) {
                printf("network_read_nb(): error %u\n", fn_network_error);
                return 1;
            }

            if (nbytes == 0)
                break;

            for (i = 0; i < nbytes; i++) {
                b = buff[i];
                printf("%c", b);
            }
        }
    }
}
