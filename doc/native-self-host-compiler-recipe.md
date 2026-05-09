# Native self-host compiler — build recipe (M3-S-047)

## Interface contract

Tools invoke **`SV0_SELF_HOST_COMPILER`** with **one argument**: an **absolute** path to a **`.sv0`** file under **`sv0c/`**. The program must print **C source** for that compilation unit on **stdout** (same contract as **`./scripts/sv0 emit-c <rel>`** / SML **`Main.main`** default mode).

## Bootstrap path (today)

There is not yet a **single native executable** compiled entirely from sv0 that implements the full SML driver CLI. Until that lands (see **`doc/driver-pipeline-composition.md`**), the supported workflow is:

**M3-S-052:** **`make -C sv0c check`** runs **`scripts/smoke-self-host-compiler.sh`** (**heap** + one-file emit). **`./scripts/sv0 check`** (meta-repo) **`exec`**s that script. **`emit-c`**, stage0 goldens, and related **`scripts/sv0`** stages use **`SV0_SELF_HOST_COMPILER`** (**`build/sv0-self-host-compiler`** after **`build-sv0-self-host-compiler.sh`**).

1. **`./scripts/build-sv0-self-host-compiler.sh`** — writes **`build/sv0-self-host-compiler`**, a thin **`exec`** wrapper around **`scripts/sv0-self-host-emit-c.sh`** (SML heap image **`build/sv0c`**).

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

## Related

- **`doc/self-host-sv0-loop.md`** — pilot loop, **`SV0_SKIP_SELF_HOST_COMPILER_DIFF`**, retirement guardrails.
- **`scripts/sv0`** **`build-self-host-compiler`** — alias that runs **`scripts/build-sv0-self-host-compiler.sh`**.
