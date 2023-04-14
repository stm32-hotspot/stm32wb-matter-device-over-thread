---
pagetitle: Release Notes for STM32WB Copro Wireless Binaries
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="../../../_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for
# <mark>STM32WB Copro Wireless Binaries</mark>
Copyright &copy; 2023 STMicroelectronics\
   
[![ST logo](../../../_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# Known Limitations

**If Anti-Rollback needs to be activated**, please make sure to activate it only after installing the latest FUS version (>= V1.2.0) 
and after successfully installing a wireless stack (without deleting it). **Otherwise, further wireless stack installation will be blocked.**

# Purpose

This release covers the delivery of STM32WB Coprocessor binaries.

Here is the list of references to user documents:

-   [AN5185](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/dm00513965.pdf) : ST FW upgrade services for STM32WB
-   [UM2237](https://www.st.com/resource/en/user_manual/dm00403500-stm32cubeprogrammer-software-description-stmicroelectronics.pdf) : STM32CubeProgrammer User Manual

Here is the list of the supported binaries:

- stm32wb5x_BLE_Thread_ForMatter_fw.bin
  - Dynamic Concurrent Mode BLE Thread for Matter (Supports Full BLE Stack 5.2 certified and Minimal Thread Device ready v1.3)
- stm32wb5x_Thread_RCP_fw.bin
  - Openthread Radio Co-Processor (RCP) with Openthread 1.3
  
- For more details, please refer to: [AN5289](https://www.st.com/resource/en/application_note/an5289-building-wireless-applications-with-stm32wb-series-microcontrollers-stmicroelectronics.pdf) : Building wireless applications with STM32WB Series microcontrollers.

<mark>How to flash the Wireless Coprocessor Binary via SWD/JTAG by ST-LINK (STM32CubeProgrammer GUI)</mark>

- *Inside the below procedure, the references to binaries name and install address are provided in the section **Main Changes** of this file.*

- STEP 1: Use STM32CubeProgrammer GUI

  - Version 2.7.0 or higher.

  - It gives access to Firmware Upgrade Service (FUS) ([AN5185](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00513965.pdf) : ST firmware upgrade services for STM32WB Series.) through Bootloader.

  - It is currently available as Graphical User Interface (GUI) mode (or Command Line Interface (CLI) mode).

- STEP 2: Access to SWD Interface (system flash)

    - For P-NUCLEO-WB55.Nucleo : 
      - Power ON via ST-LINK and Jumper JP1(USB_STL)
    - For STM32WB5MM-DK :
      - Power ON via ST-LINK and Jumper JP2(USB_STL)         
      
  - open STM32CubeProgrammer GUI and select "ST-LINK"
  - in ST-LINK configuration: (Port: SWD) then select "Connect"
  
- STEP 3: select "Start FUS" in Firmware Upgrade Services
 
- STEP 4: Read and upgrade FUS Version

  - it can been obtained selecting "Read FUS infos"
    - *00050300*: FUSv0.5.3 => **Must be updated using STEP 5.**
    - *010X0Y00*: FUSv1.**x**.y => **Must be updated using STEP 6 (when x < 2).**
    - *01020000*: FUSv1.2.0 => **Up to date, you can download the new wireless stack using STEP 7.**    

- STEP 5: Download latest FUS for only FUSv0.5.3 upgrade

  - in Firmware Upgrade Services: (File Path: \[stm32wb5x_FUS_fw_for_fus_0_5_3.bin\], Start Address: \[Install@\])
  - then select "Firmware Upgrade"
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.
  
- STEP 6: Download latest FUS or Safeboot

  - in Firmware Upgrade Service: (File Path: \[FUS_Binary\], Start Address: \[Install@\])
  - then select "Firmware Upgrade"
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.
  
- STEP 7: Download new wireless stack (after "FW Delete" if requested)
 
  - in Firmware Upgrade Service: (File Path: \[Wireless_Coprocessor_Binary\], Start Address: \[Install@\])
  - then select "Firmware Upgrade" (with "Verify download" and "Start stack after upgrade")
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.
  (**optional**: activate "Anti-Rollback" on downloaded binary)
  If you need to perform FUS related manipulations, you always need to START the FUS in case the FUS is not running.

- STEP 8: Revert to default configuration: in Option Bytes menu for User Configuration set: nSWboot0=1 (checked) nboot1=1 nboot0=1 
   

<mark>How to flash the Wireless Coprocessor Binary via USB (STM32CubeProgrammer GUI)</mark>

- *Inside the below procedure, the references to binaries name and install address are provided in the section **Main Changes** of this file.*

- STEP 1: Use STM32CubeProgrammer GUI

  - Version 2.7.0 or higher.

  - It gives access to Firmware Upgrade Service (FUS) ([AN5185](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00513965.pdf) : ST firmware upgrade services for STM32WB Series.) through Bootloader.

  - It is currently available as Graphical User Interface (GUI) mode (or Command Line Interface (CLI) mode).

- STEP 2: Access to Bootloader USB Interface (system flash)

  - Boot mode selected by Boot0 pin set to VDD
    - For P-NUCLEO-WB55.Nucleo : 
      - Jumper between CN7.5(VDD) and CN7.7(Boot0)
      - Power ON via USB_USER and Jumper JP1(USB_MCU)
    - For STM32WB5MM-DK :
      - Jumper on CN13(VDD-Boot0) after Pins header soldering 
      - Power ON via USB_USER and Jumper JP2(USB_MCU)    
    
  - **Warning**: required Option Byte configuration: [nSWboot0=1]    
 
  - Open STM32CubeProgrammer GUI and select "USB"
    with (Port: USB1) in USB configuration then select "Connect"
  
- STEP 3: step bypassed via USB
    
- STEP 4: Read and upgrade FUS Version
  
  - it can been obtained selecting "Read FUS infos"
    - *00050300*: FUSv0.5.3 => **Must be updated using STEP 5.**
    - *010X0Y00*: FUSv1.**x**.y => **Must be updated using STEP 6 (when x < 2).**
    - *01020000*: FUSv1.2.0 => **Up to date, you can download the new wireless stack using STEP 7.**    

- STEP 5: Download latest FUS for only FUSv0.5.3 upgrade

  - in Firmware Upgrade Service: (File Path: \[stm32wb5x_FUS_fw_for_fus_0_5_3.bin\], Start Address: \[Install@\])
  - then select "Firmware Upgrade"
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.

- STEP 6: Download latest FUS or Safeboot

  - in Firmware Upgrade Service: (File Path: \[FUS_Binary\], Start Address: \[Install@\])
  - then select "Firmware Upgrade"
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.
  
- STEP 7: Download new wireless stack (after "FW Delete" if requested)
 
  - in Firmware Upgrade Service: (File Path: \[Wireless_Coprocessor_Binary\], Start Address: \[Install@\])
  - then select "Firmware Upgrade" (with "Verify download" and "Start stack after upgrade")
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.
  
- STEP 8: Revert STEP 2 procedure to put back device in normal mode.


<mark>How to flash the Wireless Coprocessor Binary via SWD/JTAG by ST-LINK (Command Line Interface)</mark>

- *Inside the below procedure, the references to binaries name and install address are provided in the section **Main Changes** of this file.*

- STEP 1: Use STM32CubeProgrammer

  - Version 2.7.0 or higher.

  - It gives access to Firmware Upgrade Service (FUS) ([AN5185](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00513965.pdf) : ST firmware upgrade services for STM32WB Series.) through Bootloader.

  - It is currently available as Command Line Interface (CLI) mode.

- STEP 2: Access to SWD Interface (system flash)

    - For P-NUCLEO-WB55.Nucleo : 
      - Power ON via ST-LINK and Jumper JP1(USB_STL)
    - For STM32WB5MM-DK :
      - Power ON via ST-LINK and Jumper JP2(USB_STL)  
      
- STEP 3: apply "Start FUS"

  - *STM32\_Programmer\_CLI.exe -c port=swd -startfus*
  
- STEP 4: Read and upgrade FUS Version

  - *STM32\_Programmer\_CLI.exe -c port=swd mode=UR -r32 0x20030030 1* 
    - *@0x20030030: 00050300*: FUSv0.5.3 => **Must be updated using STEP 5.**
    - *@0x20030030: 010X0Y00*: FUSv1.**x**.y => **Must be updated using STEP 6 (when x < 2).**
    - *@0x20030030: 01020X00*: FUSv1.2.0 => **Up to date, you can download the new wireless stack using STEP 7.**
  - Note: when FUS is not running but FUS operator used, you can read FUS version @0x20010010


- STEP 5: Download latest FUS for only FUSv0.5.3 upgrade

  - *STM32\_Programmer\_CLI.exe -c port=swd mode=UR -fwupgrade stm32wb5x_FUS_fw_for_fus_0_5_3.bin \[Install@\] firstinstall=0*
  
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.
  
- STEP 6: Download latest FUS or Safeboot
 
  - *STM32\_Programmer\_CLI.exe -c port=swd mode=UR -fwupgrade \[FUS_Binary\] \[Install@\] firstinstall=0*

  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.

- STEP 7: Download new wireless stack 

  - *STM32\_Programmer\_CLI.exe -c port=swd mode=UR -fwupgrade  \[Wireless_Coprocessor_Binary\] \[Install@\] firstinstall=0*
  
  Please check **Wireless Coprocessor Binary Table** for Install@ parameter depending of the binary.
  (**optional**: activate "Anti-Rollback" on downloaded binary: *STM32\_Programmer\_CLI.exe -c port=swd -antirollback*) 
  If you need to perform FUS related manipulations, you always need to START the FUS in case the FUS is not running.
  
- STEP 8: Revert to default OB configuration 

  - *STM32\_Programmer\_CLI.exe -c port=swd mode=UR -ob nSWboot0=1 nboot1=1 nboot0=1
  
 
<mark>How to flash the Wireless Coprocessor Binary via USB (Command Line Interface)</mark>

- *Inside the below procedure, the references to binaries name and install address are provided in the section **Main Changes** of this file.*

- STEP 1: Use STM32CubeProgrammer

  - Version 2.7 or higher.

  - It gives access to Firmware Upgrade Service (FUS) ([AN5185](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/application_note/DM00513965.pdf) : ST firmware upgrade services for STM32WB Series.) through Bootloader.

  - It is currently available as Command Line Interface (CLI) mode.

- STEP 2: Access to Bootloader USB Interface (system flash)

  - Boot mode selected by Boot0 pin set to VDD
    - For P-NUCLEO-WB55.Nucleo : 
      - Jumper between CN7.5(VDD) and CN7.7(Boot0)
      - Power ON via USB_USER and Jumper JP1(USB_MCU)
    - For STM32WB5MM-DK :
      - Jumper on CN13(VDD-Boot0) after Pins header soldering 
      - Power ON via USB_USER and Jumper JP2(USB_MCU)
      
  - **Warning**: required Option Byte configuration: [nSWboot0=1] 

- STEP 3: step bypassed via USB
  
- STEP 4: Read and upgrade FUS Version

  - *STM32\_Programmer\_CLI.exe -c port=usb1 -r32 0x20030030 1*
    - *@0x20030030: 00050300*: FUSv0.5.3 => **Must be updated using STEP 5.**
    - *@0x20030030: 010X0Y00*: FUSv1.**x**.y => **Must be updated using STEP 6 (when x < 2).**
    - *@0x20030030: 01020X00*: FUSv1.2.0 => **Up to date, you can download the new wireless stack using STEP 7.**

- STEP 5: Download latest FUS for only FUSv0.5.3 upgrade

  - *STM32\_Programmer\_CLI.exe -c port=usb1 -fwupgrade stm32wb5x_FUS_fw_for_fus_0_5_3.bin \[Install@\] firstinstall=0*
  
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.
  
- STEP 6: Download latest FUS or Safeboot

  - *STM32\_Programmer\_CLI.exe -c port=usb1 -fwupgrade \[FUS_Binary\] \[Install@\] firstinstall=0*
  Please check **Firmware Upgrade Services Binary Table** for Install@ parameter depending of the binary.

- STEP 7: Download new wireless stack 

  - *STM32\_Programmer\_CLI.exe -c port=usb1 -fwupgrade \[Wireless_Coprocessor_Binary\] \[Install@\] firstinstall=0*
  Please check **Wireless Coprocessor Binary Table** for Install@ parameter depending of the binary.
  
- STEP 8: Revert STEP 2 procedure to put back device in normal mode. 

  
<mark>How to compute available flash size </mark>

- The default linker file is provided in [\\Drivers\\CMSIS\\Device\\ST\STM32WBxx\\Source\\Templates].

  The maximum flash memory that can be used by the application is up to the Secure Flash Start Address (SFSA) that can be read from the option byte.

  The __ICFEDIT_region_ROM_end__ in the linker can be modified with a value up to : (0x08000000 + (SFSA << 12)) - 1.

- **Example**:
  When the SFSA option byte is set to 0xA0, the maximum value to be used for __ICFEDIT_region_ROM_end is 0x0809FFFF – which is 640KB of flash

- **Note**: 
  The SFSA option byte can only be set by the CPU2. The user cannot modify that value.

:::

::: {.col-sm-12 .col-lg-8}
# Update History
::: {.collapse}
<input type="checkbox" id="collapse-section2" checked aria-hidden="true">
<label for="collapse-section2" aria-hidden="false">V1.0.1 / 13-April-2023</label>
<div>

## Main Changes
- BLE THREAD:
  - Update Coprocessor binary according to STM32CubeWB firmware release V1.16.0

- THREAD:
  - Update Coprocessor binary according to STM32CubeWB firmware release V1.16.0  

  
**Firmware Upgrade Services Binary Table**: Provides Install address for the targeted binary to be used in flash procedure "STEP 5/6" via USB or via SWD/JTAG.

| Wireless Coprocessor Binary                | STM32WB5xxG(1M)  | STM32WB5xxY(640K) | STM32WB5xxE(512K) | STM32WB5xxC(256K) | Version  
| ------------------------------------------ | ---------------  | ---------------   |  ---------------  | ---------------   | -------  
| stm32wb5x_FUS_fw_for_fus_0_5_3.bin         | 0x080**EC**000   |   0x080**9A**000  | 0x080**7A**000    | 0x080**3A**000    | V1.2.0[]{.icon-st-unchanged}
| stm32wb5x_FUS_fw.bin                       | 0x080**EC**000   |   0x080**9A**000  | 0x080**7A**000    | 0x080**3A**000    | V1.2.0[]{.icon-st-unchanged}

**Wireless Coprocessor Binary Table**: Provides Install address for the targeted binary to be used in flash procedure "STEP 7" via USB or via SWD/JTAG.

| Wireless Coprocessor Binary                | STM32WB5xxG(1M)  | STM32WB5xxY(640k) | STM32WB5xxE(512K) | STM32WB5xxC(256K) | Version 
|--------------------------------------------|------------------|-------------------|-------------------|-------------------| -------
| stm32wb5x_BLE_Thread_ForMatter_fw.bin      | 0x080**81**000   | NA                | NA                | NA                | V1.0.1[]{.icon-st-update}
| stm32wb5x_Thread_RCP_fw.bin                | 0x080**CF**000   | NA                | NA                | NA                | V1.0.0[]{.icon-st-update}


**Firmware Safeboot Binary Table**: Provides Install address for the targeted binary to be used in flash procedure "STEP 5/6" via USB or via SWD/JTAG.

| Wireless Coprocessor Binary    | STM32WB5xxG(1M)  | STM32WB5xxY(640k) | STM32WB5xxE(512K) | STM32WB5xxC(256K) | Version  
| ------------------------------ | ---------------  | ---------------   |  ---------------  | ---------------   | -------  
| stm32wb5x_Safeboot_fw.bin      | 0x080**F0**000   |   0x080**9F**000  | 0x080**7F**000    | 0x080**3F**000    | V2.0.0[]{.icon-st-unchanged}

</div>
:::
::: {.collapse}
<input type="checkbox" id="collapse-section1"  aria-hidden="true">
<label for="collapse-section1" aria-hidden="false">V1.0.0 / 23-February-2023</label>
<div>

## Main Changes

# Initial Release
**Associated changes in Wireless Coprocessor Binary**:

- THREAD:
  - Thread 1.3 ready
  - OT stack updated to SHA-1: 32ac6ddfad2bc196e216c42249c73420542daac0 (July 26th 2022)

  
**Firmware Upgrade Services Binary Table**: Provides Install address for the targeted binary to be used in flash procedure "STEP 5/6" via USB or via SWD/JTAG.

| Wireless Coprocessor Binary                | STM32WB5xxG(1M)  | STM32WB5xxY(640K) | STM32WB5xxE(512K) | STM32WB5xxC(256K) | Version  
| ------------------------------------------ | ---------------  | ---------------   |  ---------------  | ---------------   | -------  
| stm32wb5x_FUS_fw_for_fus_0_5_3.bin         | 0x080**EC**000   |   0x080**9A**000  | 0x080**7A**000    | 0x080**3A**000    | V1.2.0[]{.icon-st-unchanged}
| stm32wb5x_FUS_fw.bin                       | 0x080**EC**000   |   0x080**9A**000  | 0x080**7A**000    | 0x080**3A**000    | V1.2.0[]{.icon-st-unchanged}

**Wireless Coprocessor Binary Table**: Provides Install address for the targeted binary to be used in flash procedure "STEP 7" via USB or via SWD/JTAG.

| Wireless Coprocessor Binary                | STM32WB5xxG(1M)  | STM32WB5xxY(640k) | STM32WB5xxE(512K) | STM32WB5xxC(256K) | Version 
|--------------------------------------------|------------------|-------------------|-------------------|-------------------| -------
| stm32wb5x_BLE_Thread_ForMatter_fw.bin      | 0x080**81**000   | NA                | NA                | NA                | V1.0.0[]{.icon-st-update}
| stm32wb5x_Thread_RCP_fw.bin                | 0x080**CF**000   | NA                | NA                | NA                | V1.0.0[]{.icon-st-update}


**Firmware Safeboot Binary Table**: Provides Install address for the targeted binary to be used in flash procedure "STEP 5/6" via USB or via SWD/JTAG.

| Wireless Coprocessor Binary    | STM32WB5xxG(1M)  | STM32WB5xxY(640k) | STM32WB5xxE(512K) | STM32WB5xxC(256K) | Version  
| ------------------------------ | ---------------  | ---------------   |  ---------------  | ---------------   | -------  
| stm32wb5x_Safeboot_fw.bin      | 0x080**F0**000   |   0x080**9F**000  | 0x080**7F**000    | 0x080**3F**000    | V2.0.0[]{.icon-st-unchanged}

</div>
:::

:::
:::

<footer class="sticky">
For complete documentation on STM32WBxx, visit: [[www.st.com/stm32wb](http://www.st.com/stm32wb)]

*This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.*
</footer>
