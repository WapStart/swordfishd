all: swordfish

swordfish: 
	scons -C daemon-src/

clean:
	scons -C daemon-src/ -c
	rm -rf daemon-src/config.log daemon-src/.scon*
