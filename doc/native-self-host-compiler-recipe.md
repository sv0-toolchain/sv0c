# Native self-host compiler — build recipe (M3-S-047)

## Interface contract

Tools invoke **`SV0_SELF_HOST_COMPILER`** with **one argument**: an **absolute** path to a **`.sv0`** file under **`sv0c/`**. The program must print **C source** for that compilation unit on **stdout** (same contract as **`./scripts/sv0 emit-c <rel>`** / SML **`Main.main`** default mode).

## P2 state (2026-07-03)

**`build-sv0-self-host-compiler.sh`** now produces **three** artifacts:

| Path | What | Used by |
|------|------|---------|
| `build/sv0-self-host-compiler` | SML-heap wrapper (bootstrap delegate) | CI default, `SV0_SELF_HOST_COMPILER` |
| `build/sv0-driver-native` | Native binary from `lib/driver.sv0` via SML→C→cc | Manual testing, P2 validation |
| `build/sv0-self-host-compiler-native` | Thin wrapper around `sv0-driver-native` | `self-host-native.yml` P2 step |

The native binary reads its input path from **`/tmp/.sv0_drv_path`** (written by the native wrapper before each invocation). An empty file → test mode (49 self-host tests, exit 0). The build script initialises the file as empty.

**Parity gap (P3 prerequisite):** The native binary's C emitter uses inline expressions rather than SML's temporary-variable IR, so `diff` vs SML is non-trivial. The `self-host-native.yml` native step runs with `SV0_SKIP_SELF_HOST_COMPILER_DIFF=1` until P3 parity work lands. The SML-backed default continues to pass a clean diff in CI.

## Bootstrap path (today)

**M3-S-052:** **`make -C sv0c check`** runs **`scripts/smoke-self-host-compiler.sh`** (**heap** + one-file emit). **`./scripts/sv0 check`** (meta-repo) **`exec`**s that script. **`emit-c`**, stage0 goldens, and related **`scripts/sv0`** stages use **`SV0_SELF_HOST_COMPILER`** (**`build/sv0-self-host-compiler`** after **`build-sv0-self-host-compiler.sh`**).

1. **`./scripts/build-sv0-self-host-compiler.sh`** — writes **`build/sv0-self-host-compiler`** (SML-heap wrapper) **and** **`build/sv0-driver-native`** (native binary from `lib/driver.sv0`). Previously only wrote the SML wrapper.

2. Point **`SV0_SELF_HOST_COMPILER`** at that path and run the pilot loop or full test:

```bash
./scripts/build-sv0-self-host-compiler.sh
export SV0_SELF_HOST_COMPILER="$PWD/build/sv0-self-host-compiler"
./scripts/sv0 self-host-sv0-loop
```

3. For **`./scripts/sv0 test`**, omit **`SV0_SKIP_SELF_HOST_COMPILER_DIFF`** so the third leg **`diff`** runs against **`SV0_SELF_HOST_COMPILER`** when set:

```bash
SV0_SELF_HOST_COMPILER="$PWD/build/sv0-self-host-compiler" ./scripts/sv0 test
```

**Prerequisites:** **`cc`** (or **`CC`**), **SML/NJ**, **`sv0c`** **`make heap`** succeeds — same as **`doc/self-host-sv0-loop.md`**.

## Forward path (semantic native compiler)

Replace **`build/sv0-self-host-compiler`** with a **native binary** built from sv0-emitted C (or an agreed link step), honoring the same argv/stdout contract. **`scripts/sv0-self-host-emit-c.sh`** remains the reference behavior until **`diff`** against that binary is green on **`lib/self-host-sv0-loop.list`**.

## L0 prerequisite chain (orientation)

A **native** **`SV0_SELF_HOST_COMPILER`** that can compile real compiler sources implies an **integrated emit entrypoint** (lexer → … → C on stdout) in **native** code. Today **`lib/main.sv0`** is a **staging** module — full composition is **deferred** per **`doc/driver-pipeline-composition.md`** (**Deferred engineering**: single mega-TU **or** multi-unit sv0 linking). Until **(A)** or **(B)** lands, keeping **`build/sv0-self-host-compiler`** as a **bootstrap delegate** is **correct**; **`task/sv0-toolchain-milestone-3-self-host.Rmd`** **## L0 closure (blocking prerequisites)** tracks stakeholder **L0** vs slice backlog **Done**.

## Related

- **`doc/archive/l0-closure-roadmap.md`** — phased **L0** engineering path (merge orchestration → native composition → parity → evidence).
- **`doc/self-host-sv0-loop.md`** — pilot loop, **`SV0_SKIP_SELF_HOST_COMPILER_DIFF`**, retirement guardrails.
- **`scripts/sv0`** **`build-self-host-compiler`** — alias that runs **`scripts/build-sv0-self-host-compiler.sh`**.
