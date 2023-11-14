MAIN := main.c
DEPENDENCIES := game.c graphics.c
EXECUTABLE := bin/game.exe
INCLUDE := include
LIB := lib
FLOC := lib
FRAMEWORKS := SDL2 SDL2_ttf

# pattern substitution to generate object files
object_files := $(patsubst %.c, bin/%.o, $(DEPENDENCIES))

CFLAGS:= -I $(INCLUDE)
# LDFLAGS:= -framework $(FRAMEWORKS) -F $(FLOC) -rpath $(LIB)
LDFLAGS := $(addprefix -framework , $(FRAMEWORKS)) -F $(FLOC) -rpath $(LIB)


default: run

run: link
	./$(EXECUTABLE)

link: $(object_files)
	gcc src/$(MAIN) $(object_files) -o $(EXECUTABLE) $(CFLAGS) $(LDFLAGS)

bin/%.o: src/%.c
	gcc -c $< -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(object_files) $(EXECUTABLE)
