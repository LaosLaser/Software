all: 
	g++ -o cups-laos main.cpp Config.cpp LogEvent.cpp 
install:
	cp cups-laos /usr/lib/cups/backend
	mkdir -p /usr/share/ppd/cups-laos
	cp CUPS-LAOS.ppd /usr/share/ppd/cups-laos/CUPS-LAOS.ppd
