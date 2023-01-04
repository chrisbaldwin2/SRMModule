
CPPSTD = -std=c++20
CFLAGS = -Wall $(CPPSTD)

LINK_TARGET = main

OBJS = \
	main.o

SHARED_OBJS = \
	placement.so

REBUILDABLES = $(OBJS) $(SHARED_OBJS) $(LINK_TARGET)

.PHONY: all clean

all: $(LINK_TARGET)
	echo All done

clean : 
	rm -f $(REBUILDABLES)
	echo Clean done

%.o : %.cpp
	g++ -g -o $@ -c $< $(CFLAGS)

%.so : %.o
	g++ -shared $< -o $@

$(LINK_TARGET) : $(OBJS) $(SHARED_OBJS)
	g++ -g -o $@ $^ $(CFLAGS)
