# MeshPico
A small project designed to be used for development of an IOT embedded device. This makes a Raspberry Pi Pico simulated a Json RPC service that can be connected to over hardware serial 1.
## Prerequisites
This was developed in an opensource platform in the [PlatformIO IDE](https://platformio.org/). This IDE can be used standalone or installed as a VS Code extension.

You need to have git installed for PlatformIO to be able to pull the custom platform.

This project uses a library that provides Json serialization and RPC bindings called [mjson](https://github.com/cesanta/mjson).

The custom platform can be found [here](https://github.com/Wiz-IO/wizio-pico) and is still in beta.
### Setup
- Open PlatformIO and navigate to the platforms pane
- Click on the **Advanced Installation** button and paste:
	> [https://github.com/Wiz-IO/wizio-pico](https://github.com/Wiz-IO/wizio-pico)
- Install the platform
- After installing the platform clone [this](https://github.com/ejzeronimo/MeshPico) repo into a sandbox folder
- Open the repo in the IDE and PlatformIO should detect the pio project and populate the build and upload options
- There are two properties you might need to change in the **platform.ini**
	>upload_port: this defines the path to the pico, the default is **E:\/**
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
