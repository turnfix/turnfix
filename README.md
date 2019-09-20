# TurnFix

TurnFix is a program to manage gymnastics competition data. It was build mainly for competitions
held in Germany and is currently available in German only.

## Build

Make sure to install the git submodules before builduing the project.

Execute the following commands to do this:

```
git submodule init
git git submodule update
```

### Windows
Builds with Qt5 and MinGW or MSVC++ on Windows.

## Execute 
### Windows
To execute the program some additional files are necesarry and must be copied to execution folder. 
From `Qt\<version>\mingw73_64\bin` folder:
- libgcc_s_seh-1.dll 
- libstdc++-6.dll 
- libwinpthread-1.dll 
- Qt5Cored.dll
- Qt5Guid.dll
- QtNetworkd.dll 
- QtPrintSupportd.dll
- QtSqld.dll
- QtWidgetsd.dll 

From PostgreSQL 11 folder:
- libeay32.dll
- libpq.dll
- ssleay32.dll
- libiconv-2.dll 
- libintl-9.dll

## License

Apache 2.0
