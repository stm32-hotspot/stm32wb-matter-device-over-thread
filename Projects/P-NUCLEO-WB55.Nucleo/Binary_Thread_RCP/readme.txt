/**
  @page Thread_RCP application
  
  @verbatim
  ******************************************************************************
  * @file    Binary_Thread_RCP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Thread RCP application
  ******************************************************************************
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This binary does the communication between the Co-Processor wireless stack and the Host Processor
over the Spinel protocol.
You can flash this binary with STMCubeProgrammer at the address 0x08000000. 
The Host Processor (Raspberry Pi 3B or newer) has the role of a Thread border Router.
It connects a Thread network to other IP-based networks,such as Wi-Fi or Ethernet.
You will need to connect the P-Nucleo-WB55 board with the Raspberry via FTDI.
The communication with the Raspberry Pi is done through a serial connection. UART Rx/Tx are routed to 
external connector Cn10 pin35(Tx) and pin 37(Rx).


@par Keywords

THREAD, RCP, OTBR, Matter

