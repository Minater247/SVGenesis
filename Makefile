SGDK_PATH = D:\SGDK
GENS_RUN = .\GenS\gens.exe

all: release

release:
	$(SGDK_PATH)\bin\make -f $(SGDK_PATH)\makefile.gen

debug:
	$(SGDK_PATH)\bin\make -f $(SGDK_PATH)\makefile.gen debug

run: release
	$(GENS_RUN) ..\out\rom.bin

clean:
	rm -rf ./out ./res/resources.h

crun: clean run