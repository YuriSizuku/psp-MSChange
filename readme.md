# MS Change

For spoofing some game checking (such as "xxxのディスクがセットされていません" error),  this plugin can emulate inserting or removing Memory stick with key combo.  

The project is developed on `PSP3000 6.60proC2 CFW`.  

## Usage  

1. Add `ms:/seplugins/mschange/mschange.prx 1` in `ms:/seplugins/game.txt` config file and then `reset vsh`
2. Start a game, press `select+up` to emulate inserting memory stick, `select+down` to emulate removing memory stick
3. After that, most of games creates a save after, you can then disable this plugin

## Build

1. Download [pspsdk](https://github.com/pspdev/pspsdk) or [minpspw(windows)](https://sourceforge.net/projects/minpspw/)  
2. set env `PSPSDK` to the sdk path  
3. use `build_mschange.bat` or `build_mschange.sh` to compile the project  

## Tetesed Games  

ULJM06404 ヴァルプルガの詩  
ULJM06390 Flowers1  

## Known Issue

* ~~Game exit crash~~, solved by adding delay in the main thread

## Reference  

[npdecrypter](https://github.com/codestation/npdecrypter)  
[isochange](https://www.gamebrew.org/wiki/ISOchange_PSP)
[ppsspp sceIo](https://github.com/hrydgard/ppsspp/blob/60402909f8bf1e2f04a9aec5e81ae82a3e21aee3/Core/HLE/sceIo.cpp#L1884)
[ppsspp_niddb](https://github.com/pspdev/psp-ghidra-scripts/blob/master/ppsspp_niddb.xml)  

