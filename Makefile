all: swordfish testfiller

swordfish: 
	scons -C daemon-src/

testfiller:
	cd daemon-src/misc/fill_func && g++ -c -fPIC filler.cpp -o filler.o && g++ -shared  -o libfiller.so filler.o && cd -

clean:
	scons -C daemon-src/ -c
	rm -rf daemon-src/config.log daemon-src/.scon* daemon-src/misc/fill_func/*filler.*o
