# CupsUtils

## Usage

Open terminal and execute following commands.

Go to the build dir:
```
cd <path-to-build-dir>
```


Should print cups notifications but not working for osx.
```
./printersNotifications
```


Returns local printers list.
```
./getPrinters
```


Get the device_uri for specified printer name, which is obtained from getPrinters command.
```
./getURIForPrinterName <printer-name>
```


The pair of utilities getPrinters and getURIForPrinterName show the information like:
```
lpstat -v
``` 


Set new device_uri for specified printer name.
```
./setURIForPrinterName <printer-name> <new-device-uri>
```

This utility is equvalent for:
```
lpadmin -p printername -v device-uri
```

The code for this utility was taken from the original lpadmin code from CUPS.

This was done with purpose to investigate the functionality of lpadmin utility.



List of printer options:
```
./listOptionsForPrinterName <printer-name>
```

