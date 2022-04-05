
CFLAGS = /Oxs /Gz

i80test.exe:
	cl $(CFLAGS) i80test.c i8085.c emux.c

test: i80test.exe
	i80test.exe
