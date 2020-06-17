CFLAGS=-Wall -Wextra -lglfw -lGL -lX11 -lXi -ldl -lpthread -Iinclude

all: 01-First-triangle.c 02-EBO-first-rectangle.c
	@gcc $(CFLAGS) glad.c 01-First-triangle.c -o 01-First-triangle
	@gcc $(CFLAGS) glad.c 02-EBO-first-rectangle.c -o 02-EBO-first-rectangle

01: 01-First-triangle.c
	@gcc $(CFLAGS) glad.c 01-first-triangle.c

02: 02-EBO-first-rectangle.c
	@gcc $(CFLAGS) glad.c 02-EBO-first-rectangle.c
