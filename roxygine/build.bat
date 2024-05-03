SET render=src\engine\render\render.c src\engine\render\render_init.c src\engine\render\render_util.c
SET io=src\engine\io\io.c
SET files=src\glad.c src\main.c src\engine\global.c %render% %io%
SET libs=..\lib\SDL2main.lib ..\lib\SDL2.lib ..\lib\freetype.lib 
 
CL /Zi /I ..\include %files% /link %libs% /OUT:roxygine.exe 
