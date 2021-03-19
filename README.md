Config notes
-------------
Arduino IDE
--Code built off original BLE_DevStone-3.ino file from 2017
--Config:
-----Board: "Generic nRF51"
-----Chip: "16 kB RAM, 256 kB flash (xxaa)"
-----Softdevice: "S130"
-----Low Frequency Clock: "Crystal Oscillator"
-----Port: "/dev/cu.usbmodem1411 (Nordic Semiconductor nRF52 Dev Kit"
-----Programmer: "J-Link"
--Verify and Upload to test on board

--Create the hex file:
-----In preferences check "compilation" box
-----"Sketch" menu; "Verify/Compile"
-----In Arduino log, find 4th to last line with "ihex" in it to find folder path
-----In terminal go to folder, copy "cp" folder to easier location (myoware folder on desktop for example)
-----May want to delete previous copies of the file to ensure rewrite?

nRF Connect
--APP: "Programmer"
-----Select device
-----Read device
-----Add Hex file
-----Write hex file

nRF Toolbox (on phone)
--UART
-----Connect to device / devstone
-----View log
