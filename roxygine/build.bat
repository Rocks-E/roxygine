SET render=src\engine\render\render.c src\engine\render\render_init.c src\engine\render\render_util.c
SET io=src\engine\io\io.c
SET config=src\engine\config\config.c
SET input=src\engine\input\input.c
SET time=src\engine\time\time.c
SET ds=src\engine\data_structures
SET array_list=%ds%\array_list.c
SET physics=src\engine\physics\physics.c
SET modules=%render% %io% %config% %input% %time% %array_list% %physics%
SET files=src\glad.c src\main.c src\engine\global.c %modules%
SET libs=..\lib\SDL2main.lib ..\lib\SDL2.lib
 
CL /Zi /I ..\include %files% /link %libs% /OUT:roxygine.exe 
