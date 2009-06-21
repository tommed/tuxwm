graphic_opts=`pkg-config --cflags --libs cairo-xlib`
font_opts=`pkg-config --cflags --libs freetype2`
all: program

program: state.h config.h main.o keyboard.o composite.o mouse.o desktop.o size_and_stack.o panel.o fonts.o root_window.o
	cc -g -o program -Wall -lX11 -lXcomposite -lXrender $(graphic_opts) $(font_opts) \
					 state.h config.h main.o keyboard.o composite.o mouse.o desktop.o size_and_stack.o panel.o fonts.o root_window.o
	
keyboard.o: config.h keyboard.h keyboard.c
	cc -g -o keyboard.o -Wall -lX11 -c keyboard.c
	
mouse.o: mouse.h mouse.c
	cc -g -o mouse.o -Wall -lX11 -c mouse.c
	
panel.o: config.h panel.h panel.c
	cc -g -o panel.o -Wall -lX11 $(graphic_opts) $(font_opts) -c panel.c
	
root_window.o: config.h root_window.h root_window.c
	cc -g -o root_window.o -Wall -lX11 $(graphic_opts) $(font_opts) -c root_window.c

fonts.o: fonts.h fonts.c
	cc -g -o fonts.o -Wall -lX11 $(graphic_opts) $(font_opts) -c fonts.c
	
size_and_stack.o: size_and_stack.h size_and_stack.c
	cc -g -o size_and_stack.o -Wall -lX11 -c size_and_stack.c
	
composite.o: composite.h composite.c
	cc -g -o composite.o -Wall -lX11 -lXcomposite -lXrender $(graphic_opts) -c composite.c
	
desktop.o: desktop.h desktop.c
	cc -g -o desktop.o -Wall -lX11 $(graphic_opts) -c desktop.c
	
main.o: main.c state.h
	cc -g -o main.o -Wall -lX11 -lXcomposite -c main.c
