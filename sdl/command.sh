g++.exe -std=c++11 src/*.cpp src/ECS/*.cpp src/utils/*.cpp -I"C:\dev\SDL\64\include" -L"C:\dev\SDL\64\lib" -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lws2_32 -o bin/main.exe && cd bin && ./main.exe