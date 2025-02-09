MKDIR_P = mkdir -p
CFLAGS += -std=c17 -I/usr/include/ -O2 -Wno-unused-result
MKDIR_P = mkdir -p
APP_NAME = argb-colors
INSTALL_DIR = /usr/bin/

all: $(APP_NAME)

$(APP_NAME): ./src/$(APP_NAME).c
	$(MKDIR_P) ./bin
	$(CC) $(CFLAGS) ./src/$(APP_NAME).c -lusb-1.0 -o ./bin/$(APP_NAME)

clean:
	rm -f ./bin/*.o \
		./bin/$(APP_NAME)

install:
	install ./bin/$(APP_NAME) $(INSTALL_DIR)

uninstall:
	rm $(INSTALL_DIR)$(APP_NAME)