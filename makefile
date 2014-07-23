objects= main.o commands.o send_recv.o incoming_conn.o

chat:	$(objects)
	gcc -Wall -o chat $(objects) -lpthread
main.o:	main.c main.h send_recv.h incoming_conn.h commands.h
	gcc -c main.c
commands.o:	commands.c main.h
	gcc -c commands.c
send_recv.o:	send_recv.c main.h
	gcc -c send_recv.c
incoming_conn.o:	incoming_conn.c main.h
	gcc -c incoming_conn.c

clean:
	rm $(objects) chat 