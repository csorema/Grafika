all:
    gcc -Iinclude/ src/ball.c src/game.c src/main.c src/pad.c src/pong.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lm -o pong.exe -Wall -Wextra -Wpedantic

linux:
    gcc -Iinclude/ src/ball.c src/game.c src/main.c src/pad.c src/pong.c -lSDL2 -lSDL2_image -lGL -lm -o pong -Wall -Wextra -Wpedantic