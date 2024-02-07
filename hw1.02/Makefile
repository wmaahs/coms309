CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

BIN = poke_world_gen
OBJS = poke_world_gen.o pokemon_map_gen.o

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CC) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

# poke_world_gen: poke_world_gen.o pokemon_map_gen.o
# 	gcc poke_world_gen.o -o poke_world_gen
	
# poke_world_gen.o: poke_world_gen.c poke_world_gen.h pokemon_map_gen.h
# 	gcc -c poke_world_gen.c poke_world_gen.h pokemon_map_gen.h

# pokemon_map_gen.o: pokemon_map_gen.c pokemon_map_gen.h
# 	gcc -c pokemon_map_gen.c pokemon_map_gen.h

# pokemon_map_gen: pokemon_map_gen.c pokemon_map_gen.h
# 	gcc pokemon_map_gen.c pokemon_map_gen.h -o pokemon_map_gen

clean:
	rm -rf poke_world_gen *.o *.d *.gch