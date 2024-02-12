all:
	cd src && $(MAKE)
	cp src/main .

clean:
	cd src && $(MAKE) clean
	rm main
