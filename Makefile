CC = gcc
CFLAGS = -Wall
TARGET = sed_simplified

all: $(TARGET)

$(TARGET): main.o sed_operations.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o sed_operations.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)




test: $(TARGET)
	./$(TARGET) input.txt -r "old text" "new text"
	./$(TARGET) input.txt -d "text to delete"
	./$(TARGET) input.txt -i -f "insert text front"
	./$(TARGET) input.txt -i -b "insert text back"


