#!/usr/bin/python

import sys, getopt
from enum import Enum

class WhatToDo(Enum):
	nothing = 1
	check = 2
	install = 3
	uninstall = 4

whatToDo = WhatToDo.nothing

dlpPolicyName = "<Policy printingdlp>"
closePolicyTag = "</Policy>"

# Creating a multiline string
dlpPolicy = """
<Policy printingdlp>
  JobPrivateAccess default
  JobPrivateValues default
  SubscriptionPrivateAccess default
  SubscriptionPrivateValues default
  <Limit Create-Job Print-Job Print-URI Validate-Job>
    Order deny,allow
  </Limit>
  <Limit Send-Document Send-URI Hold-Job Purge-Jobs Create-Job-Subscription Renew-Subscription Cancel-Subscription Get-Notifications Reprocess-Job Cancel-Current-Job Suspend-Current-Job Cancel-My-Jobs Close-Job CUPS-Move-Job CUPS-Get-Document>
    Require user @OWNER @SYSTEM
    Order deny,allow
  </Limit>
  <Limit Release-Job Restart-Job Resume-Job Set-Job-Attributes>
    AuthType Default
    Require user root
    Order deny,allow
  </Limit>
  <Limit CUPS-Add-Modify-Printer CUPS-Delete-Printer CUPS-Add-Modify-Class CUPS-Delete-Class CUPS-Set-Default CUPS-Get-Devices>
    AuthType Default
    Require user @SYSTEM
    Order deny,allow
  </Limit>
  <Limit Pause-Printer Resume-Printer Enable-Printer Disable-Printer Pause-Printer-After-Current-Job Hold-New-Jobs Release-Held-New-Jobs Deactivate-Printer Activate-Printer Restart-Printer Shutdown-Printer Startup-Printer Promote-Job Schedule-Job-After Cancel-Jobs CUPS-Accept-Jobs CUPS-Reject-Jobs>
    AuthType Default
    Require user @AUTHKEY(system.print.operator) @admin @lpadmin
    Order deny,allow
  </Limit>
  <Limit Cancel-Job CUPS-Authenticate-Job>
    Require user @OWNER @AUTHKEY(system.print.operator) @admin @lpadmin
    Order deny,allow
  </Limit>
  <Limit All>
    Order deny,allow
  </Limit>
</Policy>
"""

def readConfig(filename):
	cupsdConf = ""
	# Open a file: file
	file = open(filename,mode='r')
	# read all lines at once
	cupsdConf = file.read()
	# close the file
	file.close()
	return cupsdConf

def check(filename):
	cupsdConf = ""
	cupsdConf = readConfig(filename)
	dlpPolicyFound = cupsdConf.find(dlpPolicyName)
	if dlpPolicyFound >= 0:
		print "Dlp policy is installed"
		dlpPolicyFound = 0
	else:
		print "Dlp policy is not installed"

	return dlpPolicyFound

def install(filename):
	cupsdConf = ""
	cupsdConf = readConfig(filename)
	#find last </Policy> tag
	lastPolicyCloseTag = cupsdConf.rfind(closePolicyTag)
	cupsdConf = cupsdConf[:lastPolicyCloseTag + len(closePolicyTag)] + dlpPolicy + cupsdConf[lastPolicyCloseTag + len(closePolicyTag):]

	text_file = open(filename, "w")
	text_file.write(cupsdConf)
	text_file.close()

def uninstall(filename):
	cupsdConf = ""
	cupsdConf = readConfig(filename)
	cupsdConf = cupsdConf.replace(dlpPolicy, "")
	text_file = open(filename, "w")
	text_file.write(cupsdConf)
	text_file.close()

def main(argv):
	cupsdConfFile=""
	try:
		opts, args = getopt.getopt(argv,"hf:ciu", ["file=", "check", "install", "uninstall"])
	except getopt.GetoptError:
		print 'Usage: %s' % argv[0]
		print 'Check if dlp policy installed: %s -f <cupsd-conf-file> -c' % argv[0]
		print 'Install dlp policy: %s -f <cupsd-conf-file> -i' % argv[0]
		print 'Uninstall dlp policy: %s -f <cupsd-conf-file> -u' % argv[0]
		sys.exit(-1)
	for opt, arg in opts:
		if opt == '-h':
			print 'Usage: %s' % argv[0]
			print 'Check if dlp policy installed: %s -f <cupsd-conf-file> -c' % argv[0]
			print 'Install dlp policy: %s -f <cupsd-conf-file> -i' % argv[0]
			print 'Uninstall dlp policy: %s -f <cupsd-conf-file> -u' % argv[0]
			sys.exit(0)
		elif opt in ("-f", "--file"):
			cupsdConfFile = arg
		elif opt in ("-c", "--check"):
			whatToDo = WhatToDo.check
		elif opt in ("-i", "--install"):
			whatToDo = WhatToDo.install
		elif opt in ("-u", "--uninstall"):
			whatToDo = WhatToDo.uninstall

	if whatToDo == WhatToDo.check:
		checkResult=check(cupsdConfFile)
		sys.exit(checkResult)
	elif whatToDo == WhatToDo.install:
		checkResult=check(cupsdConfFile)
		if checkResult != 0:
			install(cupsdConfFile)
	elif whatToDo == WhatToDo.uninstall:
		checkResult=check(cupsdConfFile)
		if checkResult == 0:
			uninstall(cupsdConfFile)

if __name__ == "__main__":
	main(sys.argv[1:])
