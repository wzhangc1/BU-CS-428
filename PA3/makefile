PROG = dijkstra

all:	$(PROG)	$(PROG2)

$(PROG): 	$(PROG).o
			g++ $(PROG).o	-o $(PROG)

$(PROG).o:	$(PROG).cpp
			g++ -c $(PROG).cpp

clean:
	rm -f *.o $(PROG)
