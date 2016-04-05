# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file 
# system
# 
# The PG can be used to detect errors in this file, just create a new project with this addon 
# and the PG will write to the console the kind of error and in which line it is

meta:
	ADDON_NAME = ofxGrt
	ADDON_DESCRIPTION = Addon for Gesture Recognition Toolkit
	ADDON_AUTHOR = Nick Gillian
	ADDON_TAGS = "GRT"
	ADDON_URL = https://github.com/nickgillian/ofxGrt

common:
	ADDON_INCLUDES = src
	ADDON_INCLUDES += libs/grt
	ADDON_SOURCES_EXCLUDE = libs/grt/build/%
	ADDON_SOURCES_EXCLUDE += libs/grt/data/%
	ADDON_SOURCES_EXCLUDE += libs/grt/docs/%
	ADDON_SOURCES_EXCLUDE += libs/grt/examples/%
	ADDON_SOURCES_EXCLUDE += libs/grt/extensions/%
	ADDON_SOURCES_EXCLUDE += libs/grt/gui/%
	ADDON_SOURCES_EXCLUDE += libs/grt/jni/%
	ADDON_SOURCES_EXCLUDE += libs/grt/tests/%
	ADDON_SOURCES_EXCLUDE += libs/grt/third_party/%
	ADDON_SOURCES_EXCLUDE += libs/grt/tools/%
	ADDON_SOURCES_EXCLUDE += libs/grt/website/%
	ADDON_SOURCES_EXCLUDE += libs/grt/wiki/%
	ADDON_SOURCES_EXCLUDE += libs/grt/website/%

	# Uncomment the lines below to use the system installed GRT headers and libs
	# ADDON_INCLUDES += /usr/local/include/GRT
	# ADDON_LDFLAGS = -L/usr/local/lib -lgrt
	# ADDON_LIBS_EXCLUDE = libs/grt/%
	# ADDON_INCLUDES_EXCLUDE = libs/grt/%
	# ADDON_SOURCES_EXCLUDE += libs/grt/GRT/%

	
