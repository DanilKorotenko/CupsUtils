# CupsUtils

## Usage

Open terminal and execute following commands.

```
cd <path-to-build-dir>
```



```
./getPrinters
```
Returns local printers list.



```
./printersNotifications
```
Should print cups notifications but not working.



```
./getURIForPrinterName <printer-name>
```
Get device_uri for specified printer name, which is obtained from getPrinters command.



```
./setURIForPrinterName <printer-name> <new-device-uri>
```
Set new device_uri for specified printer name.
