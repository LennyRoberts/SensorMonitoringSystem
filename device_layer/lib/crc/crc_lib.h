#ifndef CRC_LIB_H
#define CRC_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_CRC_CHECK        0x0300
#define CRC4_ITU            0x0400
#define CRC5_EPC            0x0500
#define CRC5_ITU            0x0501
#define CRC5_USB            0x0502
#define CRC6_ITU            0x0600
#define CRC7_MMC            0x0700
#define CRC8                0x0800
#define CRC8_ITU            0x0801
#define CRC8_ROHC           0x0802
#define CRC8_MAXIM          0x0803
#define CRC16_IBM           0x1000
#define CRC16_MAXIM         0x1001
#define CRC16_USB           0x1002
#define CRC16_MODBUS        0x1003
#define CRC16_CCITT         0x1004
#define CRC16_CCITT_FALSE   0x1005
#define CRC16_X25           0x1006
#define CRC16_XMODEM        0x1007
#define CRC16_DNP           0x1008
#define CRC32               0x2000
#define CRC32_MPEG_2        0x2001

#define HIGH_BIT_FIRST      0x1100       /*高位在前 低位在后*/
#define LOW_BIT_FIRST       0x0011       /*低位在前 高位在后*/

typedef unsigned char       uint8_t;     /* 00~FF = 0~255 */
typedef unsigned short      uint16_t;    /* 0000~FFFF = 0~65535 */
typedef unsigned int        uint32_t;    /* 0000 0000~FFFF FFFF = 0~42 9496 7295 */

uint8_t crc4_itu(uint8_t *data, uint16_t length);
uint8_t crc5_epc(uint8_t *data, uint16_t length);
uint8_t crc5_itu(uint8_t *data, uint16_t length);
uint8_t crc5_usb(uint8_t *data, uint16_t length);
uint8_t crc6_itu(uint8_t *data, uint16_t length);
uint8_t crc7_mmc(uint8_t *data, uint16_t length);
uint8_t crc8(uint8_t *data, uint16_t length);
uint8_t crc8_itu(uint8_t *data, uint16_t length);
uint8_t crc8_rohc(uint8_t *data, uint16_t length);
uint8_t crc8_maxim(uint8_t *data, uint16_t length);
uint16_t crc16_ibm(uint8_t *data, uint16_t length);
uint16_t crc16_usb(uint8_t *data, uint16_t length);
uint16_t crc16_x25(uint8_t *data, uint16_t length);
uint16_t crc16_dnp(uint8_t *data, uint16_t length);
uint16_t crc16_maxim(uint8_t *data, uint16_t length);
uint16_t crc16_modbus(uint8_t *data, uint16_t length);
uint16_t crc16_xmodem(uint8_t *data, uint16_t length);
uint16_t crc16_ccitt(uint8_t *data, uint16_t length);
uint16_t crc16_ccitt_false(uint8_t *data, uint16_t length);
uint32_t crc32(uint8_t *data, uint16_t length);
uint32_t crc32_mpeg_2(uint8_t *data, uint16_t length);

void __CRC4bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len);
void __CRC5bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len);
void __CRC6bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len);
void __CRC7bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len);
void __CRC8bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len);
void __CRC16bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len);
void __CRC32bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len);

int __CRC_Check(unsigned short crc_name, unsigned short storage_mode, unsigned char *data, unsigned int len);		/*CRC校验检查执行函数*/
void __CRC_Generator(unsigned short crc_model, unsigned short storage_mode, unsigned char **data, unsigned int len);

#endif // !CRC_LIB_H
