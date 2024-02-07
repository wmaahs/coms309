CC = gcc
CXX = g++
ECHO = echo
RM = rm -f


poke_world_gen: poke_world_gen.o
	gcc -o poke_world_gen poke_world_gen.o
	
poke_world_gen.o: poke_world_gen.h poke_world_gen.c
	gcc -c poke_world_gen.c

# poke_world_gen.h: pokemon_map_gen

# pokemon_map_gen: pokemon_map_gen.c pokemon_map_gen.h
# 	gcc pokemon_map_gen.c pokemon_map_gen.h -o pokemon_map_gen

