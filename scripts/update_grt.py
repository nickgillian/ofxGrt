import os
import re
import platform
import subprocess
from optparse import OptionParser
from os import walk

#Define the known build platforms
OS_X = "osx"
OS_LINUX_64 = "linux64"

def run( cmd ):
	if( os.system( cmd ) != 0 ):
		raise ValueError( "Failed to run command: " + cmd )
 
def findHeaders( grtSrcPath ):

    headers = []
    ext = '.h'
    n = len( grtSrcPath )
    for (dirpath, dirnames, filenames) in walk( grtSrcPath ):
        localDirName = dirpath[n:] + '/'
        for name in filenames:
            if name.lower().endswith(ext):
                headers.append( os.path.join(localDirName, name) )

    #return the file list
    return headers

def findDirectories( grtSrcPath ):

    directories = []
    ext = '.h'
    n = len( grtSrcPath )
    for (dirpath, dirnames, filenames) in walk( grtSrcPath ):
        localDirName = dirpath[n+1:] + '/'
        dirAdded = False
        for name in filenames:
            if name.lower().endswith(ext):
                if( dirAdded == False ):
                    directories.append( localDirName )
                    dirAdded = True

    #return the directories list
    return directories


#Parse the command line options, the first option should be the path to the main GRT directory
parser = OptionParser()
(options, args) = parser.parse_args()

if( len( args ) < 1 ):
    raise NameError( "Missing Argument: You need to specify the path to the GRT directory on your system as the first argument, check this file for more info" ) 

#Get the GRT path
grtDir = args[0]

#Remove any trailing slash
if grtDir.endswith('/'):
    grtDir = grtDir[:-1]

#Get the current working directory
rootDir = os.getcwd()

#Set the main directories
grtSourceDir = grtDir + '/GRT'
grtBuildDir = grtDir + '/build'
grtInstallDir = rootDir + '/../libs/grt'
installIncludeDir = grtInstallDir + '/include/GRT'
installLibDir = grtInstallDir + '/lib'
grtLibPath = grtBuildDir + '/build'
grtLibName = 'libgrt.a'

#Validate the grt source directory exists
if( os.path.exists( grtSourceDir ) == False ):
    raise AssertionError( "Failed to validate GRT directory. Make sure you are passing in the path to the main grt git repository" ) 

#Get the OS we are building for
buildPlatform = "Unknown"
if (platform.system() == "Darwin"):
    buildPlatform = OS_X
if (platform.system() == "Linux"):
    buildPlatform = OS_LINUX_64

#Throw an error if we do not know what this OS is
if( buildPlatform == "Unknown" ):
    print "build system OS: " + platform.system()
    raise AssertionError( "Unkown OS. The OS you are building for is not configured yet, please add a configuration option for your OS to this script" ) 

print "Building for " + buildPlatform

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
run( 'cmake .. -DBUILD_SHARED_LIB=OFF -DBUILD_EXAMPLES=ON -DBUILD_TOOLS=OFF -DBUILD_TESTS=OFF' )
run( 'make' )
print "Static library built"

#Copy the header files and static library to the ofxGRT directory

# Get a list of all the GRT source files
grtHeaders = findHeaders( grtSourceDir )
if( len( grtHeaders ) == 0 ):
    raise AssertionError( "Failed to locate any GRT headers from directory" + grtSourceDir ) 

# Get a list of all the GRT directories
grtDirectories = findDirectories( grtSourceDir )
if( len( grtDirectories ) == 0 ):
    raise AssertionError( "Failed to locate any GRT directories from main source directory" + grtDirectories ) 

#Change to the lib directory
os.chdir( grtInstallDir )

#Copy the headers to the source directory
if( os.path.exists( 'include' ) == False ):
    os.system( 'mkdir include' )

if( os.path.exists( 'lib' ) == False ):
    os.system( 'mkdir lib' )

#change to the include directory
os.chdir( 'include' )

#Make any sub directories that are needed
for d in grtDirectories:
    if( os.path.exists( d ) == False ):
        os.makedirs( d )

#Install the headers
print "Installing headers..."
for f in grtHeaders:
    x = f
    if( x[0] == '/' ):
        x = x[1:]
    run( 'cp ' + grtSourceDir + f + ' ' + x )

#Install the headers
print "Installing static library..."
os.chdir( grtInstallDir + '/lib' )
if( os.path.exists( buildPlatform ) == False ):
    os.makedirs( buildPlatform )
os.chdir( buildPlatform )
run( 'cp ' + grtLibPath + '/' + grtLibName + ' ' + grtLibName )

print "Update complete"
