CC = gcc
MKDIR_P = mkdir -p
APP_NAME = argbColors

all: $(APP_NAME)

argbColors: ./src/$(APP_NAME).c
	$(MKDIR_P) ./bin
	$(CC) -std=c17 -I/usr/include/ ./src/$(APP_NAME).c -lusb -lusb-1.0 -o ./bin/$(APP_NAME)

clean:
	rm -rf ./bin/*.o \
		./bin/$(APP_NAME)
#install:
#	install ./bin/$(APP_NAME) /usr/bin
#
#uninstall:
#	rm /usr/bin/$(APP_NAME)
