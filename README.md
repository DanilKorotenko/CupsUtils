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
Set printer hold new jobs until indefinite
```
lpadmin -p printer -o job-hold-until-default=indefinite
```



Get the device_uri for specified printer name, which is obtained from getPrinters command.
```
./getURIForPrinterName <printer-name>
```

#### Listening to CUPS notifications


## Usage Scenario: tea4cups alternative

1. Install printing operations policy:
```
./policyManager.py -f /etc/cups/cupsd.conf -i
```
This policy will specify that only root can release, continue or set job attributes.

2. Get list of printers
```
./getPrinters
```
3. Make desired printers hold job until indefinite by default
```
./setPrinterJobHoldUntilDefaultIndefinite <printer-name>
```
4. Make desired printers use custom printing operation policy that was installed by policyManager
```
./setOptionForPrinterName <printer-name> printer-op-policy printingdlp
```
5. You can be notified when new print job added:
```
./cupsNotificationsListenerOSX
```
6. Once print job job is added, it becames on pause.
7. You can obtain printing document from job:
```
./getDocument
```
8. You can do your activity with the job: read attributes, inspect print data. The job will be holded.
9. You can release job or cancel it:
```
./releaseJob
```
For canceling, see CupsUtils.hpp
