SML      := sml
ML_BUILD := ml-build
HEAP     := sv0c
CC       ?= cc

# Every .sml/.sig file ml-build can actually read (sources.cm's Group lists
# a subset of these plus $/basis.cm etc., which don't change locally) --
# used below so `make heap`/`make check` skip the (slow) ml-build step
# entirely when nothing sv0c compiles from has changed since the last build.
SML_SOURCES := $(shell find sml-legacy \( -name '*.sml' -o -name '*.sig' \) -not -path '*/.cm/*' 2>/dev/null)

.PHONY: build test check clean e2e test-contract-runtime integration integration-vm legacy-bootstrap-check legacy-bootstrap-heap

# M3-S-052: default `check` = heap + one-file emit smoke (scripts/smoke-self-host-compiler.sh — works standalone + as submodule).
# Full CM.make compile of sml-legacy is legacy-bootstrap-check (CI keeps both).
legacy-bootstrap-heap: heap

build:
	echo 'CM.make "sources.cm"; OS.Process.exit OS.Process.success;' | $(SML)

test: coverage-identifiers
	mkdir -p build
	echo 'CM.make "sources.cm"; use "test/test_runner.sml"; OS.Process.exit OS.Process.success;' | $(SML)

check: heap
	bash "$(CURDIR)/scripts/smoke-self-host-compiler.sh"

legacy-bootstrap-check:
	@tmp=$$(mktemp); echo 'CM.make "sources.cm"; OS.Process.exit OS.Process.success;' | $(SML) >$$tmp 2>&1; \
	  if grep -q 'Error:' $$tmp; then tail -40 $$tmp; rm -f $$tmp; exit 1; fi; \
	  rm -f $$tmp

# `heap` is a thin phony alias for the real file target below, so
# `make heap`/`make check` (check: heap) skip ml-build whenever
# build/$(HEAP) is already newer than sources.cm and every .sml/.sig file.
.PHONY: heap
heap: build/$(HEAP)

build/$(HEAP): sources.cm $(SML_SOURCES)
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
	touch build/$(HEAP)

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

# CV-102: opcode 119 and coverage identifiers agree with the sv0doc registry copy.
.PHONY: coverage-identifiers
coverage-identifiers:
	python3 scripts/check_coverage_identifiers.py --selftest
	python3 scripts/check_coverage_identifiers.py --repo sv0c
