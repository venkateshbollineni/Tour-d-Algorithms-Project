CC = g++
CFLAGS = -fopenmp
TARGETS = bbs bbp qss qsp reference mss msp
SRCS = bbs.cpp bbp.cpp qss.cpp qsp.cpp reference.cpp mss.cpp msp.cpp

all: $(TARGETS)

$(TARGETS): $(SRCS)
	@echo "Compiling $@ from $@.cpp"
	$(CC) -o $@ $@.cpp $(CFLAGS)

clean:
	rm -f $(TARGETS)
