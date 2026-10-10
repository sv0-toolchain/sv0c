#!/usr/bin/env python3
"""Producer resource tiers, end to end (sv0cov CV-211; SPEC 16.3.9, 16.4,
18.2; COV-FMT-019, COV-FMT-020, COV-FMT-023, AC-055, AC-056, BL-081).

A producer refuses a map or profile over the tier it was built or launched
for, before it starts or publishes. The tier is `standard` unless named:
`sv0 native-compile --coverage-tier=...` compiles the native runtime with
the ceilings, `sv0 vm-run --raw-profile-tier ...` gives them to sv0vm, and
both compilers refuse to write a map over the tier's counter or byte
ceiling. The SV0COV_* transport never carries a tier.

With the f0 fixture (9 counters; its run publishes a profile of exactly L
bytes: 104 fixed, 12 per counter that ran), through the real drivers:

1. Compiler, native and VM: a custom tier of exactly 9 counters builds; 8
   is refused (COV6001) and leaves no map, binding, or artifact. The map
   byte ceiling is exact too (the map's size builds, one byte less does
   not), checked at the compiler's request interface.
2. Native runtime: built for max-bytes L, the run publishes its L-byte
   profile; built for L - 1, the required run exits 1 with COV6001 and no
   file. A build for the protocol maxima works.
3. sv0vm: launched with max-counters 9 and max-bytes L it publishes; with
   8 counters it refuses before user code; with L - 1 bytes it runs but
   publishes nothing and fails (required). Both exact.
4. `large` on both backends gives the same counts as `standard`, the build
   record states the tier and its ceilings, and a profile from a larger
   tier is still an ordinary profile to sv0cov's reader.
5. Usage errors (AC-056): custom without both ceilings, ceilings with a
   built-in tier, zero, over-protocol, and non-integer ceilings, a tier
   with coverage off, and the same on `sv0 vm-run`; none invokes a compiler
   or a VM.

    python3 sv0c/test/coverage/tiers/run_tiers.py
"""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
F0 = ROOT / "sv0cov" / "tests" / "fixtures" / "semantic" / "f0"
NATIVE = ROOT / "build" / "sv0-megatu-compiler-native"
VM_EMIT = ROOT / "build" / "sv0-megatu-vm-native"
PROTOCOL_MAX = ("4294967295", "68719476736")

sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
sys.path.insert(0, str(ROOT / "sv0cov" / "src"))
from run_plan import ensure_built  # noqa: E402
from run_vm_profile import sv0, transport  # noqa: E402
from sv0cov.formats.rawprofile import RawProfileError, decode  # noqa: E402


def custom(counters: int | str, nbytes: int | str) -> list[str]:
    return ["--coverage-tier=custom", "--coverage-max-counters", str(counters), "--coverage-max-bytes", str(nbytes)]


def vm_tier(counters: int | str, nbytes: int | str) -> list[str]:
    return ["--raw-profile-tier", "custom", "--raw-profile-max-counters", str(counters), "--raw-profile-max-bytes", str(nbytes)]


def main() -> int:
    ensure_built()
    errors: list[str] = []
    src = F0 / "main.sv0"
    expected = json.loads((F0 / "expected-counts.json").read_bytes())["counts"]
    ran = sum(1 for c in expected if c["count"])
    size = 104 + 12 * ran
    by_id = {c["point_id"]: c["count"] for c in expected}

    def counts_of(profile: Path, m: dict) -> dict[str, int] | None:
        try:
            prof = decode(profile.read_bytes(), map_counter_count=m["program_counter_count"],
                          expected_map_id=bytes.fromhex(m["map_id"]))
        except RawProfileError as exc:
            errors.append(f"{profile.name}: {exc}")
            return None
        dense = dict(prof.counts)
        return {p["point_id"]: dense.get(p["counter_index"], 0) for p in m["points"] if p["counter_index"] is not None}

    with tempfile.TemporaryDirectory() as td:
        t = Path(td)

        def native(name: str, *flags: str) -> tuple[subprocess.CompletedProcess, Path]:
            d = t / name
            d.mkdir()
            return sv0("native-compile", "--coverage=instrument", *flags, "-o", str(d / "f0"), str(src)), d

        def run_native(d: Path) -> tuple[int, str, list[Path]]:
            pdir = d / "profiles"
            pdir.mkdir()
            r = subprocess.run([str(d / "f0")], capture_output=True, text=True, env=transport(pdir), timeout=120)
            return r.returncode, r.stderr, sorted(pdir.iterdir())

        # 1. The compiler refuses a map over the tier, exactly.
        p, d = native("n-counters-9", *custom(9, 4096))
        if p.returncode or not (d / "f0.sv0covmap.json").is_file():
            errors.append(f"native: a 9-counter tier did not build the 9-counter program: {p.stderr.strip()}")
        p, d = native("n-counters-8", *custom(8, 4096))
        if p.returncode == 0 or "COV6001" not in p.stderr or "plans 9 coverage counters" not in p.stderr or any(d.iterdir()):
            errors.append(f"native: an 8-counter tier was not refused cleanly (rc={p.returncode}, {sorted(x.name for x in d.iterdir())}): {p.stderr.strip()[-200:]}")
        for n, want in ((9, True), (8, False)):
            d = t / f"v-counters-{n}"
            d.mkdir()
            q = sv0("vm-native-compile", "--coverage=instrument", *custom(n, 4096), str(src), str(d / "f0.sv0b"))
            built = q.returncode == 0 and (d / "f0.sv0b").is_file() and (d / "f0.sv0covbind.json").is_file()
            if want and not built:
                errors.append(f"VM: a 9-counter tier did not build: {q.stderr.strip()}")
            if not want and (q.returncode == 0 or "COV6001" not in q.stderr or any(d.iterdir())):
                errors.append(f"VM: an 8-counter tier was not refused cleanly (rc={q.returncode}, {sorted(x.name for x in d.iterdir())})")
        # The map byte ceiling, at the compiler's request interface (line 6:
        # "<max counters> <max map bytes>").
        ref = t / "ref-map.json"
        for binary in (NATIVE, VM_EMIT):
            def emit(limit: str, out: Path) -> subprocess.CompletedProcess:
                env = {k: v for k, v in os.environ.items() if k != "SV0_COVERAGE_LINK_ORDER"}
                env.update(SV0_DRV_REQUEST=str(src), SV0_COVERAGE_REQUEST=f"map\n{out}\nf0\nsv0c+test\n\n{limit}")
                return subprocess.run([str(binary)] + ([str(src)] if binary == NATIVE else []), capture_output=True, env=env, timeout=300)
            if emit("9 536870912", ref).returncode or not ref.is_file():
                errors.append(f"{binary.name}: map build with explicit standard limits failed")
                continue
            n = len(ref.read_bytes())
            at, under = t / f"at-{binary.name}.json", t / f"under-{binary.name}.json"
            a, b = emit(f"9 {n}", at), emit(f"9 {n - 1}", under)
            if a.returncode or not at.is_file() or at.read_bytes() != ref.read_bytes():
                errors.append(f"{binary.name}: a map of exactly the byte ceiling ({n}) was not written")
            if b.returncode != 9 or under.exists() or b"COV6001" not in b.stderr:
                errors.append(f"{binary.name}: a map one byte over its ceiling was not refused (rc={b.returncode})")

        # 2. Native runtime byte ceiling, exactly; and the protocol maxima.
        std_counts = None
        for name, flags, want_ok in (("n-std", [], True), ("n-bytes-at", custom(9, size), True),
                                     ("n-bytes-under", custom(9, size - 1), False),
                                     ("n-large", ["--coverage-tier=large"], True),
                                     ("n-protocol-max", custom(*PROTOCOL_MAX), True)):
            p, d = native(name, *flags)
            if p.returncode:
                errors.append(f"{name}: build failed: {p.stderr.strip()[-300:]}")
                continue
            rc, err, profiles = run_native(d)
            if want_ok:
                if rc != 0 or len(profiles) != 1 or len(profiles[0].read_bytes()) != size:
                    errors.append(f"{name}: exit {rc}, profiles {[len(x.read_bytes()) for x in profiles]} (want one of {size} bytes): {err.strip()[-200:]}")
                    continue
                got = counts_of(profiles[0], json.loads((d / "f0.sv0covmap.json").read_bytes()))
                if got != by_id:
                    errors.append(f"{name}: counts differ from expected-counts.json")
                std_counts = std_counts or got
            elif rc != 1 or profiles or "error[COV6001]" not in err or f"would be {size} bytes" not in err:
                errors.append(f"{name}: an over-tier profile was not refused (exit {rc}, {len(profiles)} files): {err.strip()[-200:]}")

        # 3. sv0vm: the tier comes from `sv0 vm-run`.
        v = t / "vm"
        v.mkdir()
        q = sv0("vm-native-compile", "--coverage=instrument", str(src), str(v / "f0.sv0b"))
        if q.returncode:
            errors.append(f"VM build failed: {q.stderr.strip()}")
        else:
            vm_map = json.loads((v / "f0.sv0covmap.json").read_bytes())
            cases = (("standard", [], "ok"), ("large", ["--raw-profile-tier", "large"], "ok"),
                     ("at both limits", vm_tier(9, size), "ok"), ("protocol maxima", vm_tier(*PROTOCOL_MAX), "ok"),
                     ("8 counters", vm_tier(8, size), "start"), ("one byte short", vm_tier(9, size - 1), "publish"))
            for label, flags, want in cases:
                pdir = v / ("p-" + label.replace(" ", "-"))
                pdir.mkdir()
                r = subprocess.run([str(ROOT / "scripts" / "sv0"), "vm-run", "--coverage-binding", str(v / "f0.sv0covbind.json"),
                                    *flags, str(v / "f0.sv0b")], capture_output=True, text=True, env=transport(pdir), timeout=300)
                out = r.stdout + r.stderr
                profiles = sorted(pdir.iterdir())
                if want == "ok":
                    if "vm_exit:0" not in out or len(profiles) != 1 or len(profiles[0].read_bytes()) != size \
                            or counts_of(profiles[0], vm_map) != by_id:
                        errors.append(f"VM {label}: no {size}-byte profile with the expected counts: {out[-300:]}")
                elif want == "start":
                    if r.returncode == 0 or "error[COV6001]" not in out or "vm_exit:" in out or profiles \
                            or "the custom raw-profile tier allows 8" not in out:
                        errors.append(f"VM {label}: not refused before user code: {out[-300:]}")
                else:
                    if r.returncode == 0 or "error[COV6001]" not in out or profiles or f"would be {size} bytes" not in out:
                        errors.append(f"VM {label}: an over-tier profile was not refused: {out[-300:]}")

        # 4. The build record states the tier.
        for flags, want in (([], {"name": "standard", "max_counters": 4194304, "max_bytes": 67108864, "map_max_bytes": 536870912}),
                            (["--coverage-tier=large"], {"name": "large", "max_counters": 16777216, "max_bytes": 268435456, "map_max_bytes": 2147483648}),
                            (custom(9, size), {"name": "custom", "max_counters": 9, "max_bytes": size, "map_max_bytes": 2147483648})):
            rec = t / f"rec-{want['name']}.json"
            p = sv0("native-compile", "--coverage=instrument", *flags, "--build-record=" + str(rec),
                    "-o", str(t / f"rec-{want['name']}"), str(src))
            got = json.loads(rec.read_text())["coverage"].get("tier") if p.returncode == 0 and rec.is_file() else None
            if got != want:
                errors.append(f"build record tier for {want['name']}: {got!r}")
        rec = t / "rec-off.json"
        p = sv0("native-compile", "--build-record=" + str(rec), "-o", str(t / "rec-off"), str(src))
        if p.returncode or json.loads(rec.read_text())["coverage"].get("tier") is not None:
            errors.append("build record: an off build states a tier")

        # 5. Usage errors never reach a compiler or a VM.
        out = str(t / "usage")
        usage = [
            (["native-compile", "--coverage=instrument", "--coverage-tier=custom", "-o", out, str(src)], "requires both"),
            (["native-compile", "--coverage=instrument", "--coverage-tier=custom", "--coverage-max-counters", "9", "-o", out, str(src)], "requires both"),
            (["native-compile", "--coverage=instrument", "--coverage-tier=large", "--coverage-max-bytes", "9", "-o", out, str(src)], "only for"),
            (["native-compile", "--coverage=instrument", "--coverage-max-counters", "9", "--coverage-max-bytes", "9", "-o", out, str(src)], "need --coverage-tier=custom"),
            (["native-compile", "--coverage=instrument", *custom(0, 4096), "-o", out, str(src)], "out of range"),
            (["native-compile", "--coverage=instrument", *custom(4294967296, 4096), "-o", out, str(src)], "out of range"),
            (["native-compile", "--coverage=instrument", *custom(9, 68719476737), "-o", out, str(src)], "out of range"),
            (["native-compile", "--coverage=instrument", *custom("lots", 4096), "-o", out, str(src)], "positive integer"),
            (["native-compile", "--coverage=instrument", "--coverage-tier=huge", "-o", out, str(src)], "is invalid"),
            (["native-compile", "--coverage-tier=large", "-o", out, str(src)], "require --coverage=map"),
            (["vm-native-compile", "--coverage=instrument", "--coverage-tier=custom", str(src), out + ".sv0b"], "requires both"),
            (["vm-native-compile", "--coverage-tier=large", str(src), out + ".sv0b"], "require --coverage=map"),
            (["vm-run", "--raw-profile-tier", "custom", str(v / "f0.sv0b")], "requires both"),
            (["vm-run", "--raw-profile-tier", "large", "--raw-profile-max-bytes", "9", str(v / "f0.sv0b")], "only for"),
            (["vm-run", "--raw-profile-tier", "custom", "--raw-profile-max-counters", "0", "--raw-profile-max-bytes", "9", str(v / "f0.sv0b")], "out of range"),
            (["vm-run", "--raw-profile-tier", "huge", str(v / "f0.sv0b")], "is invalid"),
        ]
        for args, needle in usage:
            r = sv0(*args)
            if r.returncode != 2 or needle not in r.stderr or "SV0VM_RUN_BEGIN" in r.stdout or Path(out).exists() \
                    or Path(out + ".sv0b").exists():
                errors.append(f"usage {' '.join(args[:4])}: rc={r.returncode}, stderr {r.stderr.strip()[-160:]!r} (want exit 2 with {needle!r})")

    if errors:
        print("coverage tiers: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print(f"coverage tiers: OK (compilers refuse a map over the tier at the exact counter and byte limits; native runtime "
          f"and sv0vm publish a {size}-byte profile at the byte ceiling and refuse at one less; sv0vm refuses 8 counters "
          f"before user code; large and the protocol maxima give the standard counts; build record states the tier; "
          f"{len(usage)} usage errors)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
