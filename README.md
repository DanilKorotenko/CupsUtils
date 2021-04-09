# CupsUtils

## Usage

Open terminal and execute following commands.

Go to the build dir:
```
cd <path-to-build-dir>
```

#### getPrinters
```
./getPrinters
```
Returns local printers list.

#### listOptionsForPrinterName
```
./listOptionsForPrinterName <printer-name>
```
Return list of printer options and values.

#### setOptionForPrinterWithName
```
./setOptionForPrinterWithName <printer-name> <option-name> <option-value>
```
Set option value for printer.

It is equivalent for:
```
lpadmin -p printer -o <option-name>=<option-value>
```
The command:
```
./setOptionForPrinterWithName <printer-name> device-uri <new-device-uri>
```
Is equvalent for:
```
lpadmin -p printername -v device-uri
```


Should print cups notifications but not working for osx.
```
./printersNotifications
```


Get the device_uri for specified printer name, which is obtained from getPrinters command.
```
./getURIForPrinterName <printer-name>
```
