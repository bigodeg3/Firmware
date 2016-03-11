typedef unsigned char BYTE;
void configCan2515(void);
void txCAN(BYTE dataReceived1, BYTE dataReceived2);
void tx_CAN(BYTE dataReceived);
void reset2515(void);
unsigned char rxCAN(int index);
void brilha_led_D3 (void);
void blink_led_D2(void);
