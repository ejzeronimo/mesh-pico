# MeshPico
A small project designed to be used for development of an IOT embedded device. This makes a Raspberry Pi Pico simulated a Json RPC service that can be connected to over hardware serial 1.
## Usage
This project has two RPC functions: **getChassisSerialNumber** and **setChassisSerialNumber**. These functions abide by a global named **SN_LENGTH** which defines the max length of the serial number. 

Any request made to the device must also be less than a 1023 characters delimitter to delimitter (If you hit that what are you even trying to do!).
### setChassisSerialNumber
This method allows you to set the serial number of the unit. The serial number must be less than SN_LENGTH. If there was an error (most likely too long of a serial number) then it will return an error message. If for some reason the device has issues with flashing the new serial number then it will return **false**.
>Normal Operation:\
>--> {"jsonrpc": "2.0", "method": "setChassisSerialNumber", "params": {"serialNumber": "8134534d-509c-45ae-b093-49c65c868c5f"}, "id": 1}\
><-- {"jsonrpc": "2.0", "result": true, "id": 1}\
>Failed Flashing:\
><-- {"jsonrpc": "2.0", "result": false, "id": 1}\
>Error Message:\
><--{"jsonrpc": "2.0", "error": {"code": -32602, "message": "Invalid Request"}, "id": "1"}
### getChassisSerialNumber
This method returns the stored serial number in the form of a string. The returned id string has a max length of SN_LENGTH. If there was an error with parsing the serial number (some sort of flash error) then it will return an error code
>Normal Operation:\
>--> {"jsonrpc": "2.0", "method": "getChassisSerialNumber", "params": {}, "id": 2}\
><-- {"jsonrpc": "2.0", "result": "8134534d-509c-45ae-b093-49c65c868c5f", "id": 2}\
>Error Message:\
><--{"jsonrpc": "2.0", "error": {"code": -32603, "message": "Internal Error"}, "id": "2"}
## Prerequisites
This was developed in an opensource platform in the [PlatformIO IDE](https://platformio.org/). This IDE can be used standalone or installed as a VS Code extension.

You need to have git installed for PlatformIO to be able to pull the custom platform.

This project uses a library that provides Json serialization and RPC bindings called [mjson](https://github.com/cesanta/mjson).

The custom platform can be found [here](https://github.com/Wiz-IO/wizio-pico) and is still in beta.
### Setup
- Open PlatformIO and navigate to the platforms pane
- Click on the **Advanced Installation** button and paste:
	>[https://github.com/Wiz-IO/wizio-pico](https://github.com/Wiz-IO/wizio-pico)
- Install the platform
- After installing the platform clone [this](https://github.com/ejzeronimo/MeshPico) repo into a sandbox folder
- Open the repo in the IDE and PlatformIO should detect the pio project and populate the build and upload options
- There are two properties you might need to change in the **platform.ini**
	>upload_port: this defines the path to the pico, the default is **E:\/**\
	>monitor_port: the comport that is used to debug the pico, default is **COM3**
### Building & Uploading
After setting up the repo there are two main PlatformIO functions that matter, building and uploading.
- In project tasks navigate to:
	>Tasks -> raspberry-pi-pico -> General
- When you are ready to build the project, use the **Build** button to compile the binaries
- From there hold down the button on the pico and plug the microcontroller into your laptop
- Once plugged in you can release the button, it should appear as an external storage device
	> This should be your upload_port property
- After compiling your binaries, use the **Upload** or **Upload and Monitor** buttons to upload the code

## Issues
Please refer to the wizio-pico wiki for help with issues involving environment setup.
