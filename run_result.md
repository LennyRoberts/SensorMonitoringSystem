# 运行结果示例(Debug)
```
sudo ./arm_device.o 
Hardware Initalizing...
Hardware Initalization Finish!

Device layer para Init Start...
Get Device base info...
Get Device information...
Get Device function module...
Get device serial port...
Get device serial port...
Get device serial port...
Get device serial port...
Get device serial port...
Get device serial port...
Get device serial port...
Get device serial port...
Get Action infor...
Get Action infor...
Get serial port interact infor...
Get Sensor Type info...
Get Sensor Emergency info...
Get Sensor item info...
Device layer para Init End!

                    Device Network Information
====================================================================
|   Device_IP    |IpPort|    Net_Mask    |   Net_Gateway  |cycleNum|
--------------------------------------------------------------------
 192.168.1.138    9001   255.255.255.0    192.168.1.230    3        
====================================================================

            Equipment Information
==================================================
       Manual_name      Manual_information 
--------------------------------------------------
     Device_Module       AllMonitor
         Device_No       2022000000000000
   Production Date       2022/04/25
  Headware_Version       Armv7+v4.3
 SofetWare_Version       DeMeiDev 1.0
    Kernel_Version       Linux4.4
 Manufacture_Infor       DeMeiCS
==================================================

   Function Switch
==========================
  Function_name    state  
--------------------------
       Action_1     ON   
       Action_1     OFF  
       Action_1     OFF  
       Action_1     OFF  
       Action_1     OFF   
       Action_1     OFF   
       Action_1     OFF   
       Action_1     ON   
==========================

    Inter serial port communication informations
=========================================================
|NO|   ISPC_Name    |Enabel|Monopoly|      Use_port     |
---------------------------------------------------------
 1  GetDataByModbus  OFF    OFF      SERIAL_485_1         
=========================================================

        Controller Parameter Information
=================================================
|No|   Ctrl_Name   |       Use_Port       |state|
-------------------------------------------------
 0  Control_1       SERIAL_485_1           OFF
ON_cmd   : 0xFE 0x05 0x00 0x00 0xFF 0x00 0x98 0x35 
OFF_cmd  : 0xFE 0x05 0x00 0x00 0x00 0x00 0xD9 0xC5 
-------------------------------------------------
 1  Control_2       SERIAL_485_2           OFF
ON_cmd   : 0xFE 0x05 0x00 0x01 0xFF 0x00 0xC9 0xF5 
OFF_cmd  : 0xFE 0x05 0x00 0x01 0x00 0x00 0x88 0x05 
-------------------------------------------------
=================================================

                      Serial Port Configuration Information
=====================================================================================
|No|     Port_Name      |     Device_Name    |SpeedBaud|DataBit|StopBit|Parity|State|
-------------------------------------------------------------------------------------
 0  KERNEL_DEBUGGING     /dev/ttymxc0         9600      8       1       N      U    
 1  SERIAL_GPS_INTERNAL  /dev/ttymxc1         9600      8       1       N      U    
 2  SERIAL_485_0         /dev/ttymxc2         9600      8       1       N      U    
 3  SERIAL_485_1         /dev/ttymxc3         9600      8       1       N      U    
 4  SERIAL_485_2         /dev/ttymxc4         9600      8       1       N      U    
 5  SERIAL_232_0         /dev/ttymxc5         9600      8       1       N      U    
 6  SERIAL_232_1         /dev/ttymxc6         9600      8       1       N      U    
 7  SERIAL_232_2         /dev/ttymxc7         9600      8       1       N      U    
=====================================================================================

                                                      Sensor Type Informations
==========================================================================================================================================================
|NO|  TypeName  |readMode|      Use_Port      |     CRC_Module     |   CRC_Turn  |LenReply|response|       Read_Sensor_Type_Cmd       | Reply_Start_Flag |
----------------------------------------------------------------------------------------------------------------------------------------------------------
 1  Noise        passive  SERIAL_485_2         CRC16_MODBUS         LowByteFirst  7        10       0x[ 04 03 00 00 00 01 84 5F ]    0x[ 04 03 ]
==========================================================================================================================================================

                         Emergency Informations
======================================================================================
|NO| Type_Name  | Emer_Name  |Start|Len|EmerCode|           Emergency_Cmd            |
--------------------------------------------------------------------------------------
 1  Noise        DataError    3     4   0x0791   0x[ 01 03 04 00 00 00 03 00 00 ]
======================================================================================

                                                                Sensor Item Informations
=============================================================================================================================================================
|NO| Type_Name  | Item_Name  |Start|Len|DataType |byteOrder|  Data_Parser  |buffpos|ItemCode|UnitOrigin|Ratio |UnitTrans| Min |Maximum |AlgoNum| Algo_Code...
-------------------------------------------------------------------------------------------------------------------------------------------------------------
 1  Noise        Noise        3     2   un_short  ABCD      y=0.100*x+0.000 0       0x0101   dB         1.000  dB        0.000 200.000 1       0x4701; 
=============================================================================================================================================================

========================#Serial port configure#========================
The KERNEL_DEBUGGING serial port open fail!!!
The SERIAL_GPS_INTERNAL serial port open fail!!!
The SERIAL_485_0 serial port open fail!!!
The SERIAL_485_1 serial port open fail!!!
The SERIAL_485_2 serial port open fail!!!
The SERIAL_232_0 serial port open fail!!!
The SERIAL_232_1 serial port open fail!!!
The SERIAL_232_2 serial port open fail!!!

--------------------------#File descriptor allocation#--------------------------
SERIAL_485_2     Not initialized
!!!Cannot assign Fd for Noise!!!
SERIAL_485_1     Not initialized
!!!Cannot assign Fd for Control_1!!!
SERIAL_485_2     Not initialized
!!!Cannot assign Fd for Control_2!!!

--------------------------#Set_Local_NetWork#--------------------------
set into "/etc/resolv.conf":
        nameserver 192.168.1.230
        nameserver 114.114.114.114
set ip success: ifconfig eth0 192.168.1.138
set mask success: ifconfig eth0 netmask 255.255.255.0
set gateway success: route add default gw 192.168.1.230
Parameters Configure Successfully!

-----------------------------------------------------------------------
 Enter The Device Main Manage System; Device System Initialize Start...
-----------------------------------------------------------------------
pthread for perceive Layer: tid= 548207047136 (0x7FA3AFB1E0)
Enter The Perceive Layer Manage Programme
Sensor Handling Initializing...
Sensor Handling Initialize Finish!

pthread for IPC One: tid= 548198654432 (0x7FA32FA1E0)
Function Realization and Processing Initializing...
pthread for Algorithm: tid= 548190261728 (0x7FA2AF91E0)
Enter Optimization Algorithm Manage Programme
pthread for Local Moduel: tid= 548181869024 (0x7FA22F81E0)
Enter Local Module Manage Programme
Function Realization and Processing Initialize Finish!

Device Main Manage Controller Initialized Successfully!

Enter The Self-Test_And-Case Programme: Device Self-Test&Case Initializing...
Device Self-Test Successfully

IPC device listening...
====================================================================
Enter the Business logic layer program; Business layer init start...
--------------------------------------------------------------------
Get Business Parameters start...
Get Server Infomation...
Get Item information...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Control parameters...
Get Business Parameters finish!

                                         center_1 Server Paramaters informations
===========================================================================================================================================
|NO|   Name  |  Type   |S_Y|A_Y|       IP       | Port |i_num|     Packet_Format_Code    |real|MinOne|MinTwo|HourOne|HourTwo|DayOne|DayTwo|
-------------------------------------------------------------------------------------------------------------------------------------------
 0  center_1  osen      ON  ON  127.0.0.1        6003   1     air_quality                 ON   1      0      1       0       1      0      
ST=22; PW=123456; Flag=5; 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
|  Item  |  Label  |Precision|Enable| Code |ModeStatistics|
-----------------------------------------------------------
 noise    a50001    2         ON     0X0101 Average        
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
===========================================================================================================================================

                                                      Control Paramaters informations
=============================================================================================================================================
|  Slot   |Enable|Mode|Warning|Alarm  |Cycle|HitRate|Delay|Hold |Dormant|Cancle| code  | W_Act | A_Act | C_Act |     DST     | StandardTime |
---------------------------------------------------------------------------------------------------------------------------------------------
 slot_1    ON     L    60      65      10    0.8     2     30    60      50     0X0101  0X01C2  0X02F0  0XF300  06:00~22:00   08:00~21:00
 slot_2    ON     L    50      60      10    0.8     2     30    60      50     0X0101  0XFFFE  0XFFFF  0X3400  22:00~06:00   21:00~08:00
=============================================================================================================================================
------------------------------
Business layer pthread init...
------------------------------
pthread for IPC Interact device: tid= 547944157632 (0x7F940451C0)
pthread for Interact Centre: tid= 547935764928 (0x7F938441C0)
~~~~~~~~~~~~~~~~~TCP connection status~~~~~~~~~~~~~~~~~~
center_1: IP:127.0.0.1; port:6003
Tcp connect: Connection refused
errno=111; center_1: Server link fali!!!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
pthread for Interact WinCC: tid= 547927372224 (0x7F930431C0)
pthread for Interact local as server: tid= 547918979520 (0x7F928421C0)
weak up Device interact thread!
weak up Centre thread!
weak up Wincc thread!
weak up Server thread!

        ==============================================
        =======| Welcome DeMeiDev 1.0 System |========
        ==============================================

weak up System Function Layer Thread!
weak up Algorithm Handling Thread!
weak up Sensor Perceive Layer Thread!
weak up Local Module Thread!
Accept client filename: business.sock
Cmd_Parse: module_addr=0x0F; func_code=0x02; addr1:0xFEFE; addr2:0xFEFE; cmd_data=1.123
--------------------------------------------------
Sensor use fd is invalid!!!
Noise:Unable to get sensor data!!!Get all sensor data...
Send Json data failed!!! please try again...
revice data is:
[ Noise ] origin data: Noise[0]=0.000000; Noise[1]=0.000000; Noise[2]=0.000000; 
 Noise ] current Use Algo is: Arithmetic_Average
Current time: 7/14 02:13
[ Noise ] optimize result is : 0.000 dB

--------------------------------------------------
                                                      Control Paramaters informations
=============================================================================================================================================
|  Slot   |Enable|Mode|Warning|Alarm  |Cycle|HitRate|Delay|Hold |Dormant|Cancle| code  | W_Act | A_Act | C_Act |     DST     | StandardTime |
---------------------------------------------------------------------------------------------------------------------------------------------
=============================================================================================================================================
Local Module get data is:Noise=0.000000
center_1 time minute one data packet(len=128):
##0116QN=20220814021304000;ST=22;CN=2011;PW=123456;MN=;Flag=5;CP=&&DataTime=20220814021304;a50001-Rtd=0.00,a50001-Flag=B&&06C0
The connection to the 0is not established or unavailable
center_1 send fail,Try again...
The connection to the 0is not established or unavailable
center_1 send over time, Abandon this connection sending!!!

Sensor use fd is invalid!!!
Business layer get cJSON Data failed!!!
Noise:Unable to get sensor data!!!Sensor use fd is invalid!!!
Noise:Unable to get sensor data!!!Buffer full && wake up pthread_AL!
Sensor use fd is invalid!!!
Noise:Unable to get sensor data!!!
====================#!From SFL_pull_buffer_2 to SPL_push_buffer_1!#=======================
--------------------------------------------------
[ Noise ] origin data: Noise[0]=0.000000; Noise[1]=0.000000; Noise[2]=0.000000; 
[ Noise ] current Use Algo is: Arithmetic_Average
[ Noise ] optimize result is : 0.000 dB
--------------------------------------------------
Accept client filename: business.sock
Cmd_Parse: module_addr=0x0F; func_code=0x02; addr1:0xFEFE; addr2:0xFEFE; cmd_data=1.123
Get all sensor data...
revice data is:
{
        "devno":        "2022000000000000",
        "s01":  {
                "code": 257,
                "data": 0,
                "flag": "N\u0001\n",
                "unit": "dB"
        }
}
code:0X101 ;data:0dB;status:N
...
```