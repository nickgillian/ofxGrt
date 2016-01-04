import os
import re
import subprocess
from optparse import OptionParser

def run( cmd ):
	if( os.system( cmd ) != 0 ):
		raise ValueError( "Failed to run command: " + cmd )
 
def step(ext, dirname, names):
    ext = ext.lower()
    files = []
    for name in names:
        if name.lower().endswith(ext):
            files.append( os.path.join(dirname, name) )

    #return the file list
    files

#Parse the command line options, the first option should be the path to the main GRT directory
parser = OptionParser()
(options, args) = parser.parse_args()

if( len( args ) < 1 ):
    raise NameError( "Missing Argument: You need to specify the path to the GRT directory on your system as the first argument, check this file for more info" ) 

#Get the GRT path
grtDir = args[0]
grtSourceDir = grtDir + '/GRT'
grtBuildDir = grtDir + '/build'

#Get the current working directory
rootDir = os.getcwd()

#Switch to the GRT build directory
os.chdir( grtBuildDir )

#If the build directory exists, then remove it to ensure a clean build
if( os.path.exists( 'build' ) ):
	run( 'rm -r build' )

#Create the temporary build directory
os.system( 'mkdir build' )  

#Build the GRT library
print "Building GRT static library..."
os.chdir( 'build' )
run( 'cmake .. -DBUILD_SHARED_LIB=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TOOLS=OFF' )
run( 'make -j4' )
print "Static library built"

#Copy the header files to the ofxGRT directory
# Get a list of all the GRT source files
grtHeaders = os.path.walk(grtSourceDir, step, '.h')

for f in grtHeaders:
	print(f)