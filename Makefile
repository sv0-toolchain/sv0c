SML      := sml
ML_BUILD := ml-build
HEAP     := sv0c
CC       ?= cc

.PHONY: build test check heap clean e2e test-contract-runtime integration integration-vm

build:
	echo 'CM.make "sources.cm"; OS.Process.exit OS.Process.success;' | $(SML)

test:
	mkdir -p build
	echo 'CM.make "sources.cm"; use "test/test_runner.sml"; OS.Process.exit OS.Process.success;' | $(SML)

check:
	@tmp=$$(mktemp); echo 'CM.make "sources.cm"; OS.Process.exit OS.Process.success;' | $(SML) >$$tmp 2>&1; \
	  if grep -q 'Error:' $$tmp; then tail -40 $$tmp; rm -f $$tmp; exit 1; fi; \
	  rm -f $$tmp

heap:
	mkdir -p build
	$(ML_BUILD) sources.cm Main.main build/$(HEAP)
	@cd build && \
	  if [ -L $(HEAP) ]; then rm -f $(HEAP); fi && \
	  if [ ! -f $(HEAP) ]; then \
	    for img in $(HEAP).*; do \
	      [ -f "$$img" ] || continue; \
	      ln -sf "$$img" "$(HEAP)"; \
	      break; \
	    done; \
	  fi

build/e2e_generated.c: scripts/export_e2e.sml sources.cm
	mkdir -p build
	$(SML) < scripts/export_e2e.sml

e2e: build/e2e_generated.c
	$(CC) -o build/e2e_run build/e2e_generated.c -Iruntime runtime/sv0_runtime.c
	./build/e2e_run; test $$? -eq 42

build/contract_requires_fail.c: scripts/export_requires_false.sml sources.cm
	mkdir -p build
	$(SML) < scripts/export_requires_false.sml

test-contract-runtime: build/contract_requires_fail.c
	$(CC) -o build/contract_requires_fail_run build/contract_requires_fail.c -Iruntime runtime/sv0_runtime.c
	./build/contract_requires_fail_run; test $$? -eq 1

integration: heap
	"$(CURDIR)/../task/sv0c-milestone-1/02-integration-test.sh"

integration-vm:
	bash "$(CURDIR)/../task/sv0vm-milestone-2/02-integration-test.sh"

clean:
	rm -rf .cm build
