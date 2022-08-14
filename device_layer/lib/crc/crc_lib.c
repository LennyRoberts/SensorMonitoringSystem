#include "crc_lib.h"

/**********************************************************************************************
 * Name:    CRC-4/ITU    x4+x+1	G
 * Poly:    0x03				十六进制多项式，省略最高位1
 * Init:    0x00				CRC初始值，和WIDTH位宽一致
 * Refin:   True				true或false，在进行计算之前，原始数据是否翻转
 * Refout:  True				true或false，运算完成之后，得到的CRC值是否进行翻转
 * Xorout:  0x00				计算结果与此参数进行异或运算后得到最终的CRC值，和WIDTH位宽一致
 * Note:
 **********************************************************************************************/
uint8_t crc4_itu(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0;					// Initial value
	while(length--){
		crc ^= *data++;                 // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0x0C;// 0x0C = (reverse 0x03)>>(8-4)
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-5/EPC           x5+x3+1
 * Poly:    0x09
 * Init:    0x09
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
uint8_t crc5_epc(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0x48;        // Initial value: 0x48 = 0x09<<(8-5)
	while(length--){
		crc ^= *data++;        // crc ^= *data; data++;
		for(i = 0; i < 8; i++){
			if(crc & 0x80)
				crc = (crc << 1) ^ 0x48;        // 0x48 = 0x09<<(8-5)
			else
				crc <<= 1;
		}
	}
	return crc >> 3;
}

/******************************************************************************
 * Name:    CRC-5/ITU           x5+x4+x2+1
 * Poly:    0x15
 * Init:    0x00
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
uint8_t crc5_itu(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0;                // Initial value
	while(length--){
		crc ^= *data++;                 // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0x15;// 0x15 = (reverse 0x15)>>(8-5)
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-5/USB           x5+x2+1
 * Poly:    0x05
 * Init:    0x1F
 * Refin:   True
 * Refout:  True
 * Xorout:  0x1F
 * Note:
 *****************************************************************************/
uint8_t crc5_usb(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0x1F;                // Initial value
	while(length--){
		crc ^= *data++;                 // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0x14;// 0x14 = (reverse 0x05)>>(8-5)
			else
				crc = (crc >> 1);
		}
	}
	return crc ^ 0x1F;
}

/******************************************************************************
 * Name:    CRC-6/ITU           x6+x+1
 * Poly:    0x03
 * Init:    0x00
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
uint8_t crc6_itu(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0;         // Initial value
	while(length--){
		crc ^= *data++;        // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0x30;// 0x30 = (reverse 0x03)>>(8-6)
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-7/MMC           x7+x3+1
 * Poly:    0x09
 * Init:    0x00
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00
 * Use:     MultiMediaCard,SD,ect.
 *****************************************************************************/
uint8_t crc7_mmc(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0;        // Initial value
	while(length--){
		crc ^= *data++;        // crc ^= *data; data++;
		for(i = 0; i < 8; i++){
			if(crc & 0x80)
				crc = (crc << 1) ^ 0x12;        // 0x12 = 0x09<<(8-7)
			else
				crc <<= 1;
		}
	}
	return crc >> 1;
}

/******************************************************************************
 * Name:    CRC-8               x8+x2+x+1
 * Poly:    0x07
 * Init:    0x00
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
uint8_t crc8(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0;        // Initial value
	while(length--){
		crc ^= *data++;        // crc ^= *data; data++;
		for(i = 0; i < 8; i++){
			if(crc & 0x80)
				crc = (crc << 1) ^ 0x07;
			else
				crc <<= 1;
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-8/ITU           x8+x2+x+1
 * Poly:    0x07
 * Init:    0x00
 * Refin:   False
 * Refout:  False
 * Xorout:  0x55
 * Alias:   CRC-8/ATM
 *****************************************************************************/
uint8_t crc8_itu(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0;        // Initial value
	while(length--){
		crc ^= *data++;        // crc ^= *data; data++;
		for(i = 0; i < 8; i++){
			if(crc & 0x80)
				crc = (crc << 1) ^ 0x07;
			else
				crc <<= 1;
		}
	}
	return crc ^ 0x55;
}

/******************************************************************************
 * Name:    CRC-8/ROHC          x8+x2+x+1
 * Poly:    0x07
 * Init:    0xFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Note:
 *****************************************************************************/
uint8_t crc8_rohc(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint8_t crc = 0xFF;         // Initial value
	while(length--){
		crc ^= *data++;            // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0xE0;        // 0xE0 = reverse 0x07
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-8/MAXIM         x8+x5+x4+1
 * Poly:    0x31
 * Init:    0x00
 * Refin:   True
 * Refout:  True
 * Xorout:  0x00
 * Alias:   DOW-CRC,CRC-8/IBUTTON
 * Use:     Maxim(Dallas)'s some devices,e.g. DS18B20
 *****************************************************************************/
uint8_t crc8_maxim(uint8_t *data, uint16_t length)	/* length是原始数据的长度，unit_8的0x32是一个字节 length=1 */
{
	uint8_t i;
	uint8_t crc = 0;                        //Initial value
	while(length--){
		crc ^= *data++;                    /*crc ^= *data; data++; (原始数据与Init异或)*/
		for(i = 0; i < 8; i++){            /* CRC-8 多项式要向右移动(计算)8次才能到达尾部，即相当于在原始数据后加8位 */
			if(crc & 1)		               /*这里原始数据本来是要经过翻转的，但没有翻转，处在低位的就是高位数值，&1就是判断高位是否为零，要不要模2除*/
				crc = (crc >> 1) ^ 0x8C;   /*0x8C = reverse 0x31 //将多项式翻转，原始数据向右移每次错开计算，此时处在高位的就是被补的8个0*/
			else
				crc >>= 1;
		}
	}
	/* refin和refout为true，原数据没有翻转则最后的结果也不需要翻转 */
	return crc;
}

/******************************************************************************
 * Name:    CRC-16/IBM          x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0x0000
 * Alias:   CRC-16,CRC-16/ARC,CRC-16/LHA
 *****************************************************************************/
uint16_t crc16_ibm(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0;        // Initial value
	while(length--){
		crc ^= *data++;        // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-16/MAXIM        x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_maxim(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0;        // Initial value
	while(length--){
		crc ^= *data++;        // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;    // crc^0xffff
}

/******************************************************************************
 * Name:    CRC-16/USB          x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0xFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_usb(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0xffff;        // Initial value
	while(length--){
		crc ^= *data++;            // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;    // crc^0xffff
}

/******************************************************************************
 * Name:    CRC-16/MODBUS       x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0xFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0x0000
 * Note:
 *****************************************************************************/
uint16_t crc16_modbus(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0xffff;        // Initial value
	while(length--){
		crc ^= *data++;           // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0xA001;        // 0xA001 = reverse 0x8005
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-16/CCITT        x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0x0000
 * Alias:   CRC-CCITT,CRC-16/CCITT-TRUE,CRC-16/KERMIT
 *****************************************************************************/
uint16_t crc16_ccitt(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0;        // Initial value
	while(length--){
		crc ^= *data++;        // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0x8408;        // 0x8408 = reverse 0x1021
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-16/CCITT-FALSE   x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0xFFFF
 * Refin:   False
 * Refout:  False
 * Xorout:  0x0000
 * Note:
 *****************************************************************************/
uint16_t crc16_ccitt_false(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0xffff;        //Initial value
	while(length--){
		crc ^= (uint16_t)(*data++) << 8; // crc ^= (uint6_t)(*data)<<8; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 0x8000)
				crc = (crc << 1) ^ 0x1021;
			else
				crc <<= 1;
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-16/X25          x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0xFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0XFFFF
 * Note:
 *****************************************************************************/
uint16_t crc16_x25(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0xffff;        // Initial value
	while(length--){
		crc ^= *data++;            // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0x8408;        // 0x8408 = reverse 0x1021
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;                // crc^Xorout
}

/******************************************************************************
 * Name:    CRC-16/XMODEM       x16+x12+x5+1
 * Poly:    0x1021
 * Init:    0x0000
 * Refin:   False
 * Refout:  False
 * Xorout:  0x0000
 * Alias:   CRC-16/ZMODEM,CRC-16/ACORN
 *****************************************************************************/
uint16_t crc16_xmodem(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0;            // Initial value
	while(length--){
		crc ^= (uint16_t)(*data++) << 8;	// crc ^= (uint16_t)(*data)<<8; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 0x8000)
				crc = (crc << 1) ^ 0x1021;
			else
				crc <<= 1;
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-16/DNP          x16+x13+x12+x11+x10+x8+x6+x5+x2+1
 * Poly:    0x3D65
 * Init:    0x0000
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFF
 * Use:     M-Bus,ect.
 *****************************************************************************/
uint16_t crc16_dnp(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint16_t crc = 0;            // Initial value
	while(length--){
		crc ^= *data++;            // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0xA6BC;        // 0xA6BC = reverse 0x3D65
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;                // crc^Xorout
}

/******************************************************************************
 * Name:    CRC-32  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:    0x4C11DB7
 * Init:    0xFFFFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFFFFF
 * Alias:   CRC_32/ADCCP
 * Use:     WinRAR,ect.
 *****************************************************************************/
uint32_t crc32(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint32_t crc = 0xffffffff;        // Initial value
	while(length--){
		crc ^= *data++;                // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0xEDB88320;// 0xEDB88320= reverse 0x04C11DB7
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;
}

/******************************************************************************
 * Name:    CRC-32/MPEG-2  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:    0x4C11DB7
 * Init:    0xFFFFFFF
 * Refin:   False
 * Refout:  False
 * Xorout:  0x0000000
 * Note:
 *****************************************************************************/
uint32_t crc32_mpeg_2(uint8_t *data, uint16_t length)
{
	uint8_t i;
	uint32_t crc = 0xffffffff;  // Initial value
	while(length--){
		crc ^= (uint32_t)(*data++) << 24;// crc ^=(uint32_t)(*data)<<24; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 0x80000000)
				crc = (crc << 1) ^ 0x04C11DB7;
			else
				crc <<= 1;
		}
	}
	return crc;
}


/*CRC校验检查执行函数:校验名称，指令，指令长度*/
int __CRC_Check(unsigned short crc_name, unsigned short storage_mode, unsigned char *data, unsigned int len)
{
	unsigned char crc;
	unsigned char parity;
	unsigned short crc_16;
	unsigned short parity_16;
	unsigned int crc_32;
	unsigned int parity_32;

	if(storage_mode == HIGH_BIT_FIRST){
		parity = data[len - 1];
		parity_16 = data[len - 2] << 8;
		parity_16 |= data[len - 1];
		parity_32 = data[len - 4] << 24;
		parity_32 |= data[len - 3] << 16;
		parity_32 |= data[len - 2] << 8;
		parity_32 |= data[len - 1];
	}
	else if(storage_mode == LOW_BIT_FIRST){    //低位在前转化为高位在前
		parity = data[len - 1];
		parity_16 = data[len - 1] << 8;
		parity_16 |= data[len - 2];
		parity_32 = data[len - 1] << 24;
		parity_32 |= data[len - 2] << 16;
		parity_32 |= data[len - 3] << 8;
		parity_32 |= data[len - 4];
	}

	switch(crc_name){
		case CRC16_MODBUS:
			crc_16 = crc16_modbus(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC16_USB:
			crc_16 = crc16_usb(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC16_IBM:
			crc_16 = crc16_ibm(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC16_MAXIM:
			crc_16 = crc16_maxim(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC16_CCITT:
			crc_16 = crc16_ccitt(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC16_CCITT_FALSE:
			crc_16 = crc16_ccitt_false(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC16_X25:
			crc_16 = crc16_x25(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC16_XMODEM:
			crc_16 = crc16_xmodem(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC16_DNP:
			crc_16 = crc16_dnp(data, len - 2);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC32:
			crc_32 = crc32(data, len - 4);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC32_MPEG_2:
			crc_32 = crc32_mpeg_2(data, len - 4);
			return ((crc_16 == parity_16) ? 1 : -1);
		case CRC8:
			crc = crc8(data, len - 1);
			return ((crc == parity) ? 1 : -1);
		case CRC8_ITU:
			crc = crc8_itu(data, len - 1);
			return ((crc == parity) ? 1 : -1);
		case CRC8_ROHC:
			crc = crc8_rohc(data, len - 1);
			return ((crc == parity) ? 1 : -1);
		case CRC8_MAXIM:
			crc = crc8_maxim(data, len - 1);
			return ((crc == parity) ? 1 : -1);
		case CRC7_MMC:
			crc = crc7_mmc(data, len - 1);
			parity = data[ len - 1 ] & 0x7F;
			return ((crc == parity) ? 1 : -1);
		case CRC6_ITU:
			crc = crc6_itu(data, len - 1);
			parity = data[ len - 1 ] & 0x3F;
			return ((crc == parity) ? 1 : -1);
		case CRC5_USB:
			crc = crc5_usb(data, len - 1);
			parity = data[ len - 1 ] & 0x1F;
			return ((crc == parity) ? 1 : -1);
		case CRC5_ITU:
			crc = crc5_itu(data, len - 1);
			parity = data[ len - 1 ] & 0x1F;
			return ((crc == parity) ? 1 : -1);
		case CRC5_EPC:
			crc = crc5_epc(data, len - 1);
			parity = data[ len - 1 ] & 0x1F;
			return ((crc == parity) ? 1 : -1);
		case CRC4_ITU:
			crc = crc4_itu(data, len - 1);
			parity = data[ len - 1 ] & 0x0F;
			return ((crc == parity) ? 1 : -1);
		case NO_CRC_CHECK:
			return 1;
		default:
			printf("Invalid CRC Check Mode Option!!\n");
			return -1;
	}
}

void __CRC4bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len)
{
	unsigned char crc4_bit;
	unsigned char *data_ptr = *data_ptr2;
	crc4_bit = crc4_itu(data_ptr, len);
	data_ptr[ len - 1 ] = crc4_bit & 0x0F;
	return;
}

void __CRC5bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len)
{
	unsigned char crc5_bit;
	unsigned char *data_ptr = *data_ptr2;
	switch(crc_name){
		case CRC5_USB:
			crc5_bit = crc5_usb(data_ptr, len);
			break;
		case CRC5_EPC:
			crc5_bit = crc5_epc(data_ptr, len);
			break;
		case CRC5_ITU:
			crc5_bit = crc5_itu(data_ptr, len);
			break;
		default:
			printf("CRC5 Invalid:");
			break;
	}
	data_ptr[ len - 1 ] = crc5_bit & 0x1F;
	return;
}

void __CRC6bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len)
{
	unsigned char crc6_bit;
	unsigned char *data_ptr = *data_ptr2;
	crc6_bit = crc6_itu(data_ptr, len);
	data_ptr[ len - 1 ] = crc6_bit & 0x3F;
	return;
}

void __CRC7bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len)
{
	unsigned char crc7_bit;
	unsigned char *data_ptr = *data_ptr2;
	crc7_bit = crc6_itu(data_ptr, len);
	data_ptr[ len - 1 ] = crc7_bit & 0x7F;
	return;
}

void __CRC8bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len)
{
	unsigned char crc8_bit;
	unsigned char *data_ptr = *data_ptr2;
	switch(crc_name){
		case CRC8:
			crc8_bit = crc8(data_ptr, len - 1);
			break;
		case CRC8_ITU:
			crc8_bit = crc8_itu(data_ptr, len - 1);
			break;
		case CRC8_ROHC:
			crc8_bit = crc8_rohc(data_ptr, len - 1);
			break;
		case CRC8_MAXIM:
			crc8_bit = crc8_maxim(data_ptr, len - 1);
			break;
		default:
			printf("CRC8 Invalid:");
			break;
	}
	data_ptr[ len - 1 ] = crc8_bit & 0xFF;
	return;
}

void __CRC16bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len)
{
	unsigned short crc16_bit;
	unsigned char *data_ptr = *data_ptr2;
	switch(crc_name){
		case CRC16_MODBUS:
			crc16_bit = crc16_modbus(data_ptr, len - 2);
			break;
		case CRC16_IBM:
			crc16_bit = crc16_ibm(data_ptr, len - 2);
			break;
		case CRC16_MAXIM:
			crc16_bit = crc16_maxim(data_ptr, len - 2);
			break;
		case CRC16_USB:
			crc16_bit = crc16_usb(data_ptr, len - 2);
			break;
		case CRC16_CCITT:
			crc16_bit = crc16_ccitt(data_ptr, len - 2);
			break;
		case CRC16_CCITT_FALSE:
			crc16_bit = crc16_ccitt_false(data_ptr, len - 2);
			break;
		case CRC16_X25:
			crc16_bit = crc16_x25(data_ptr, len - 2);
			break;
		case CRC16_XMODEM:
			crc16_bit = crc16_xmodem(data_ptr, len - 2);
			break;
		case CRC16_DNP:
			crc16_bit = crc16_dnp(data_ptr, len - 2);
			break;
		default:
			printf("CRC16 Invalid:");
			break;
	}
	if(storage_mode == HIGH_BIT_FIRST){		/*高位在前*/
		data_ptr[ len - 2 ] = (unsigned char)(crc16_bit >> 8);
		data_ptr[ len - 1 ] = (unsigned char)(crc16_bit & 0x00FF);
	}
	else if(storage_mode == LOW_BIT_FIRST){	/*低位在前*/
		data_ptr[ len - 1 ] = (unsigned char)(crc16_bit >> 8);
		data_ptr[ len - 2 ] = (unsigned char)(crc16_bit & 0x00FF);
	}else
		printf("Storage_Mode Failed!!\n");
	return;
}

void __CRC32bit_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data_ptr2, unsigned int len)
{
	unsigned short crc32_bit;
	unsigned char *data_ptr = *data_ptr2;
	switch(crc_name){
		case CRC32:
			crc32_bit = crc32(data_ptr, len - 4);
			break;
		case CRC32_MPEG_2:
			crc32_bit = crc32_mpeg_2(data_ptr, len - 4);
			break;
		default:
			printf("CRC32 Invalid:");
			break;
	}
	if(storage_mode == HIGH_BIT_FIRST){		/*高位在前*/
		data_ptr[ len - 4 ] = (unsigned char)(crc32_bit >> 24);
		data_ptr[ len - 3 ] = (unsigned char)(crc32_bit >> 16);
		data_ptr[ len - 2 ] = (unsigned char)(crc32_bit >> 8);
		data_ptr[ len - 1 ] = (unsigned char)(crc32_bit & 0x00FF);
	}
	else if(storage_mode == LOW_BIT_FIRST){	/*低位在前*/
		data_ptr[ len - 1 ] = (unsigned char)(crc32_bit >> 24);
		data_ptr[ len - 2 ] = (unsigned char)(crc32_bit >> 16);
		data_ptr[ len - 3 ] = (unsigned char)(crc32_bit >> 8);
		data_ptr[ len - 4 ] = (unsigned char)(crc32_bit & 0x00FF);
	}
	else
		printf("Storage_Mode Failed!!\n");
	return;
}

void __CRC_Generator(unsigned short crc_name, unsigned short storage_mode, unsigned char **data, unsigned int len)
{
	unsigned short crc_high = crc_name;
	crc_high &= 0xFF00;
	switch(crc_high){
		case CRC32 & 0xFF00:											//CRCwidth = 32bit
			__CRC32bit_Generator(crc_name, storage_mode, data, len);
			return;
		case CRC16_MODBUS & 0xFF00:										//CRCwidth = 16bit
			__CRC16bit_Generator(crc_name, storage_mode, data, len);
			return;
		case CRC8 & 0xFF00:												//CRCwidth = 8bit
			__CRC8bit_Generator(crc_name, storage_mode, data, len);
			return;
		case CRC7_MMC & 0xFF00:											//CRCwidth = 7bit
			__CRC7bit_Generator(crc_name, storage_mode, data, len);
			return;
		case CRC6_ITU & 0xFF00:											//CRCwidth = 6bit
			__CRC6bit_Generator(crc_name, storage_mode, data, len);
			return;
		case CRC5_USB & 0xFF00:											//CRCwidth = 5bit
			__CRC5bit_Generator(crc_name, storage_mode, data, len);
			return;
		case CRC4_ITU & 0xFF00:											//CRCwidth = 4bit
			__CRC4bit_Generator(crc_name, storage_mode, data, len);
			return;
		case NO_CRC_CHECK & 0xFF00:
			return;
		default:
			printf("Invalid CRC Generator Mode Option\n");
			return;
	}
}