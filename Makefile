CFLAGS=-Wall -Wextra -lglfw -lGL -lX11 -lXi -ldl -lpthread -Iinclude

all: 01-First-triangle.c 02-EBO-first-rectangle.c
	@ls | sed -n '/^[0-9]\+.\+\.c$$/ p' | sed 's_\(.*\).c_glad.c \1\.c -o \1_' | xargs -L 1 gcc $(CFLAGS)

01: 01-First-triangle.c
	@gcc $(CFLAGS) glad.c 01-first-triangle.c

02: 02-EBO-first-rectangle.c
	@gcc $(CFLAGS) glad.c 02-EBO-first-rectangle.c

03: 03-Two-VAO-triangles_and_passing_data_between_shaders.c
	@gcc $(CFLAGS) glad.c 03-Two-VAO-triangles_and_passing_data_between_shaders.c
