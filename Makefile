default: prog

get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check

dictionary.o: dictionary.c
	gcc -Wall -c dictionary.c dictionary.h

spell.o: spell.c
	gcc -Wall -c spell.c


prog: dictionary.o spell.o
	gcc -Wall -o spell_check dictionary.o spell.o

clean:
	rm dictionary.o spell.o test_main.o check_spell.o

cleanall:clean
	rm spell_check
	
