# live timing on ares21 serial Data Handling (DH) port

## Description

This tool receives data of colorado Ares21 Data Handling (DH) port over a serial/usb on linux/mac/raspberry/windows to local MQTT topic. After this, it can be forwarded to a website for live timing.  See [ansibleswim](https://github.com/hias222/ansibleswim) repo for details about using all softwareparts on a raspberry.  

Requires an RS485-to-RS232 or RS422-to-USB serial converter cable when connecting from the GP port over long distances to a standard. Look on ares for pin mapping

## Testing

Use serial-sender/osm6sender to create some ares dh data.
<https://github.com/hias222/serial-sender/tree/master/osm6sender>

## Details

<https://github.com/hias222/documentation/tree/main/software>

## Build

```bash
rm -rf build && cmake -S . -B build && cmake --build build
```
