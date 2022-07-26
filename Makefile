CC = gcc
CFLAGS = -std=c11 -Wall -ggdb -O0 `pkg-config --cflags json-c`
OBJS = chat.c netcode/server.o netcode/client.o netcode/socket.o netcode/message.o mpack/mpack.o
INCLUDE = `pkg-config --libs json-c`
DEPS =
OUT_FILE = chat

all: $(OUT_FILE) # build all listed targets: output_file, install

$(OUT_FILE): $(OBJS)       # target: dependency list of *.o files
	$(CC) $(CFLAGS) $^ $(INCLUDE) -o $@

build/%.o: %.c $(DEPS)           # for all *.o files: if its *.c or *.h file changed
	mkdir -p ${dir $@}
	$(CC) -ggdb -c $*.c -o $@    # compile corresponding .c file again, and give its the current target name

install: $(OUT_FILE)                      # make will build output_file first
	echo Install $(OUT_FILE) in /usr/local/bin
	sudo mv $(OUT_FILE) /usr/local/bin/

run: install                              # depend on install
	echo Run executable image $(OUT_FILE)
	$(OUT_FILE) || /bin/true              # No make error 10 if main() return non-zero

clean:
	rm -f `find . -iname "*.o"` > /dev/null				  # Remove all *.o files
	rm -f $(OUT_FILE)
	#sudo rm -f /usr/local/bin/$(OUT_FILE) # Remove the output_file
