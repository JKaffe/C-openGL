build: 01-first-triangle.c
	@gcc -Wall -Wextra -lglfw -lGL -lX11 -lXi -ldl -lpthread -Iinclude glad.c 01-first-triangle.c
