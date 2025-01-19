# (SMG) Simple Maze Game

The goal of this project was to remake an old school assignment (Dpt3.c) in a 
single day, using my newly created library, "dynamic_array.h"[^1], and experimenting 
with "raylib.h"[^2] – a truly user-friendly API for displaying graphics.

The game is quite simple: it's a maze where you fight monsters (represented by red dots),
open doors (brown dots) with keys (yellow dots), and navigate through the environment. 
Additionally, I implemented a feature that allows you to easily create your own maps 
using a simple .txt file, which can be provided as a parameter.

### Building SMG

Run the following commands to launch the game :
```
make build_game
export LD_LIBRARY_PATH=pathTo/SMG/lib/
make launch
```  

After build SMG, you can run your created map (can't exceed 40 x 40)
```
game your_map.txt
```  

ame your_map.txt

[^1] : [dynamic_array](https://github.com/Vortyy/dynamic_array) is small lib that use small buffer optimization combined with dynamic_array ideas to have growable array memory efficient 
[^2] : [raylib](https://github.com/raysan5/raylib) 
