SML      := sml
ML_BUILD := ml-build
HEAP     := sv0c

.PHONY: build test heap clean

build:
	echo 'CM.make "sources.cm";' | $(SML)

test:
	echo 'CM.make "sources.cm"; use "test/test_runner.sml";' | $(SML)

heap:
	mkdir -p build
	$(ML_BUILD) sources.cm Main.main build/$(HEAP)

clean:
	rm -rf .cm build
