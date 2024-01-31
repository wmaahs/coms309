
knights_tour: knights_tour.c
	gcc knights_tour.c -o knights_tour

knights_tour_r: knights_tour_r.c
	gcc knights_tour_r.c -o knights_tour_r

clean:
	rm -f knights_tour knights_tour_r



#for java exec
#hello.class: hello.java
#	javac hello.java


hello: hello.o
	gcc hello.o -o hello

hello.o: hello.class
	gcc hello.c -c