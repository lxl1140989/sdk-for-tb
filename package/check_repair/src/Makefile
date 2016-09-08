TARGET = check_repair handle_repair

all: $(TARGET) install

check_repair: check_repair.o repair.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ 
	
handle_repair: handle_repair.o repair.o 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ 
	
install:
	mkdir bin
	cp -f $(TARGET) bin/
 
.PHONY: clean
clean:
	rm -f *.o $(TARGET)
