#
# This file is part of Zhockon Platform project.
# 
# Copyright (C) 2009-2010 William Oliveira de Lagos <williamx64@gmail.com>
#
# Zhockon is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Zhockon is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Zhockon.  If not, see <http://www.gnu.org/licenses/>.
#

import sys,os,ConfigParser,pickle,datetime
from optparse import OptionParser
from subprocess import Popen

parser = OptionParser()
parser.add_option("-p","--program",dest="program",help="Execute the selected program with Naanphea", metavar="FILE")
parser.add_option("-m","--micromodule",dest="mm",help="Select the micromodule to be activated or deactivated", metavar="FILE")
parser.add_option("-a","--core-activate",dest="cactive",action="store_true",help="Activate one or more Naanphea micro-modules")
parser.add_option("-d","--core-deactivate",dest="cdeactive",action="store_true",help="Deactivate one or more Naanphea micro-modules")
parser.add_option("-i","--information",dest="info",action="store_true",help="Show information about the engine.")
(options,args) = parser.parse_args()

def main(*argv):
    """Execute the tools for Naanphea Core."""
    z = Core(True)
    if options.cactive:
	if options.mm:
		z.configure_core(1,options.mm)
	else:
		z.configure_core(1)
    elif options.cdeactive:
	if options.mm:
		z.configure_core(mm=options.mm)
	else:
		z.configure_core()
    elif options.info:
	if os.path.exists(os.path.abspath("ver.dat")):
		z.show_info()
	else:
		now = datetime.datetime.now()
		z.create_info("0.2",now.year,now.month)
    elif options.program:
	cmd = "python %s" % options.program
	exct = Popen(cmd.split())
	rtrn = exct.wait()
    else:
        print ("Unrecognized option. Digit -h for help.")

class Version():
    def __init__(self,v,m,y):
        self.vers = v
        self.year = y
        self.month = m

class Core():
    """Naanphea Core Class"""
    def __init__(self,boolean=False):
        """Initialize the Core switch"""
        self.zkready = boolean
    def create_info(self,v,m,y):
        verfile = open("ver.dat","w")
        versnow = Version(v,m,y)
        pickle.dump(versnow, verfile)
        verfile.close()
    def show_info(self):
        """Show information about the platform"""
        verfile = open("ver.dat","r")
        versnow = pickle.load(verfile)
        version = versnow.vers
        month = versnow.month
        year = versnow.year
        print("Zhockon Platform Alpha %s")%(version)
        print("Updated in %s/%s.")%(month,year)
        verfile.close()
    def configure_core(self,option=0,mm="None"):
        """Configure the Naanphea Core with Conf"""
        cfg = Config()
        if option == 0:
            cfg.change_conf(micromodule=mm)
        else:
            cfg.change_conf("activate",mm)

class Config():
    """Naanphea Configuration Class"""
    def __init__(self):
        """Prepare the configuration of Naanphea"""
        conf = ConfigParser.ConfigParser()
        try:
            conf.read("naan.cfg")
        except ConfigParser.NoSectionError:
            print "Problems with Conf. Check the naan.cfg file."
    def change_conf(self,option="deactivate",micromodule="None"):
        """Turn off or on the switches on Naanphea"""
        conf = ConfigParser.ConfigParser()
 
        try:
            conf.read("naan.cfg")
            file = open("naan.cfg","w")
            items = conf.items("Core")
        except ConfigParser.NoSectionError:
            print "Problems with Conf. Check the naan.cfg file."
            sys.exit(1)
	if micromodule == "None":
		if option == "deactivate":
			for i in items:
				if i[1] == "True": 
					print i[0]+": "+i[1]
			choice = raw_input("Select the sector to deactivate:")
			conf.set("Core", choice, False)
		elif option == "activate":
			for i in items:
				if i[1] == "False":
					print i[0]+": "+i[1]
			choice = raw_input("Select the sector to activate:")
			conf.set("Core",choice,True)
	else:
		choice = micromodule
		if option == "deactivate":
			conf.set("Core",choice,False)
		elif option == "activate":
			conf.set("Core",choice,True)
	conf.write(file)

if __name__ == "__main__":
    sys.exit(main(*sys.argv))