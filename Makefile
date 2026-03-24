SML      := sml
ML_BUILD := ml-build
HEAP     := sv0c
CC       ?= cc

.PHONY: build test heap clean e2e

build:
	echo 'CM.make "sources.cm";' | $(SML)

test:
	echo 'CM.make "sources.cm"; use "test/test_runner.sml";' | $(SML)

heap:
	mkdir -p build
	$(ML_BUILD) sources.cm Main.main build/$(HEAP)

build/e2e_generated.c: scripts/export_e2e.sml sources.cm
	mkdir -p build
	$(SML) < scripts/export_e2e.sml

e2e: build/e2e_generated.c
	$(CC) -o build/e2e_run build/e2e_generated.c -Iruntime runtime/sv0_runtime.c
	./build/e2e_run; test $$? -eq 42

clean:
	rm -rf .cm build
