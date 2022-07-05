CC = gcc
CFLAGS = -std=c11 -Wall -g -O0
OBJS = chat.c server.o socket.o
INCLUDE =
DEPS = socket.h
OUT_FILE = chat

all: $(OUT_FILE) install   # build all listed targets: output_file, install

$(OUT_FILE): $(OBJS)       # target: dependency list of *.o files
	$(CC) $(CFLAGS) $^ $(INCLUDE) -o $@

%.o: %.c $(DEPS)           # for all *.o files: if its *.c or *.h file changed
	$(CC) -c $*.c -o $@    # compile corresponding .c file again, and give its the current target name

install: $(OUT_FILE)                      # make will build output_file first
	echo Install $(OUT_FILE) in /usr/local/bin
	sudo mv $(OUT_FILE) /usr/local/bin/

run: install                              # depend on install
	echo Run executable image $(OUT_FILE)
	$(OUT_FILE) || /bin/true              # No make error 10 if main() return non-zero

clean:
	rm -f *.o 2> /dev/null				  # Remove all *.o files
	sudo rm -f /usr/local/bin/$(OUT_FILE) # Remove the output_file
