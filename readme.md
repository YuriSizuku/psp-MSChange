# MS Change  

![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/YuriSizuku/psp-MSChange/build_mschange.yml)  

For spoofing some game checking (such as "xxxのディスクがセットされていません" error),  this plugin can emulate inserting or removing Memory stick with key combo.  

The project is developed on `PSP3000 6.60proC2 CFW`, also tested on `PSV2000 Adrenaline 6.61pro`.  

## Usage  

1. Add `ms:/seplugins/mschange/mschange.prx 1` in `ms:/seplugins/game.txt` config file and then `reset vsh`
2. Start a game, accroding to the game instruction  
   `select+up` to emulate inserting memory stick  
   `select+down` to emulate removing memory stick  
3. After that, most of games creates a save on the memorystick, then you can disable this plugin

## Build

1. Download [pspsdk](https://github.com/pspdev/pspsdk) or [minpspw(windows)](https://sourceforge.net/projects/minpspw/)  
2. set env `PSPSDK` to the sdk path  
3. use `build_mschange.bat` or `build_mschange.sh` to compilethe project

``` shell
# example of build with pspsdk docker
docker pull ghcr.io/pspdev/pspsdk:latest
docker run -d -it --rm -v $(pwd):/project --name pspsdk_run ghcr.io/pspdev/pspsdk
docker exec -i pspsdk_run bash -c "cd /project/script && export PSPSDK=$PSPDEV && ./build_mschange.sh"
```

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
