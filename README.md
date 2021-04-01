# CupsUtils

## Usage

Open terminal and execute following commands.

```
cd <path-to-build-dir>
```

```
./printersNotifications
```
Should print cups notifications but not working.



```
./getPrinters
```
Returns local printers list.


```
./getURIForPrinterName <printer-name>
```
Get the device_uri for specified printer name, which is obtained from getPrinters command.


The pair of utilities getPrinters and getURIForPrinterName show the information like:
```
lpstat -v
``` 

```
./setURIForPrinterName <printer-name> <new-device-uri>
```
Set new device_uri for specified printer name.

This utility is equvalent for:
```
lpadmin -p printername -v device-uri
```

The code for this utility was taken from the original lpadmin code from CUPS.

This was done with purpose to investigate the functionality of lpadmin utility.
