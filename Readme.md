# Instructions to Build and Flash to STM32 Microcontroller

## Prerequisites
- ARM GCC toolchain (`arm-none-eabi-gcc`)
- CMake
- ST-Link tools (`stlink-tools`)

## Build Process

### 1. Create Build Directory
```shell
mkdir -p build
cd build
```

### 2. Configure CMake with ARM GCC Toolchain
```shell 
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake ..
```

### 3. Compile the Project
```shell
make -j$(nproc)
```

## Flashing to STM32

### 4. Convert ELF to Binary Format
```shell
arm-none-eabi-objcopy -O binary ThreadX_Os.elf ThreadX_Os.bin
```

### 5. Verify ST-Link Connection
```shell
st-info --probe
```

Expected output should show your STM32 device information.

### 6. Erase Flash (Optional but Recommended)
```shell
st-flash erase
```

### 7. Write Binary to Flash Memory
```shell
st-flash --reset write ThreadX_Os.bin 0x08000000
```

**Parameters:**
- `0x08000000` - Flash memory start address for STM32
- `--reset` - Automatically reset the microcontroller after flashing

---

## Quick Rebuild and Flash
After initial setup, you only need:
```shell
cd build
make -j$(nproc)
arm-none-eabi-objcopy -O binary ThreadX_Os.elf ThreadX_Os.bin
st-flash --reset write ThreadX_Os.bin 0x08000000
```
