# TurnFix

TurnFix is a program to manage gymnastics competition data. It was build mainly for competitions held in Germany and is currently available in German only.

## Build

Builds with Qt5.13 and MinGW on Windows. To Build it successful https://github.com/abhijitkundu/QtPropertyBrowser.git must be cloned to libs. 

## Execute 

To execute the program some additional files are necesarry and must be copied to execution folder. 
From Qt\5.13.0\mingw73_64\bin folder: 
- libgcc_s_seh-1.dll 
- libstdc++-6.dll 
- libwinpthread-1.dll 
- Qt5Cored.dll
- Qt5Guid.dll
- QtNetworkd.dll 
- QtPrintSupportd.dll
- QtSqld.dll
- QtWidgetsd.dll 

From PostGreSQL 11 folder: 
- libeay32.dll
- libpq.dll
- ssleay32.dll
- libiconv-2.dll 
- libintl-8.dll 


## License

Apache 2.0
