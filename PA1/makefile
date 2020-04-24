PROG = udpPingServer
PROG2 = udpPingClient

all:	$(PROG)	$(PROG2)

$(PROG): 	$(PROG).o
			g++ $(PROG).o	-o $(PROG)

$(PROG).o:	$(PROG).cpp
			g++ -c $(PROG).cpp
			
$(PROG2): 	$(PROG2).o
			g++ $(PROG2).o	-o $(PROG2)

$(PROG2).o:	$(PROG2).cpp
			g++ -c $(PROG2).cpp

clean:
	rm -f *.o $(PROG)
	rm -f *.o $(PROG2)
