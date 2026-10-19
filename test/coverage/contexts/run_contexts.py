#!/usr/bin/env python3
"""Coverage contexts from real runs (sv0cov CV-308; SPEC 16.4, 17.4, 21.2;
AC-024, AC-058, COV-CTX-001, COV-CTX-004).

contexts.sv0 takes different paths and loop counts from the length of the
environment variable SEL, so each context counts differently. It is built
instrumented for the native C backend and the VM, and run under four
contexts, each with its own SEL:

    absent (SV0COV_CONTEXT unset)   empty ("")   "shard é"   256 bytes ("é" x 128)

1. every run publishes one profile; sv0cov decodes its context as None / ""
   / the value, and the raw header bytes are exact on both backends: the
   CONTEXT_PRESENT flag bit (bit0) and context_length (offset 80) differ
   between the absent and the empty profile (AC-058), the context bytes
   follow the header;
2. all eight profiles merge (`sv0cov merge`) and render (`sv0cov report
   --format json`): the report lists the contexts [None, "", "shard é",
   <256 bytes>] in that order; every aggregate count is the sum of its
   by_context counts; context_view(report, i) equals, unit for unit, the
   view of a report merged from that context's two profiles alone; the
   native and the VM profile of a context count the same, so the context's
   count is twice one run's;
3. COV-CTX-001: the same eight runs with no context at all give a report
   with one (absent) context and the same aggregate points, regions, lines
   and summary as the context-collecting one;
4. the terminal report names all four contexts and holds no raw control
   character; the HTML report (CV-309) of the same evidence passes its
   structural check, offers the four contexts and the aggregate in its
   filter, and shows the program's source.

    python3 sv0c/test/coverage/contexts/run_contexts.py
"""

from __future__ import annotations

import json
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
SV0C = HERE.parents[2]
ROOT = SV0C.parent
sys.dont_write_bytecode = True
sys.path.insert(0, str(SV0C / "test" / "coverage" / "plan"))
from run_plan import ensure_built, modern_python  # noqa: E402
from run_vm_profile import build_vm, sv0, transport, vm_run  # noqa: E402

# (name, SV0COV_CONTEXT or None for unset, SEL): SEL's length drives the program.
CONTEXTS = [
    ("absent", None, ""),
    ("empty", "", "abc"),
    ("unicode", "shard é", "abcdefghijkl"),
    ("max256", "é" * 128, "abcde"),
]
CONTEXT_PRESENT = 1
FLAGS_AT, CTXLEN_AT, HEADER = 12, 80, 84

# Merge and render in directories laid out as the CLI wants them: <dir>/in
# holds the map and the profiles, <dir>/main.sv0 the source. Prints JSON with
# the reports, the views and every comparison result.
ANALYZE = r"""
import io, json, sys
from pathlib import Path
sys.path.insert(0, sys.argv[1])
from sv0cov import cli
from sv0cov.model.context import context_view
W = Path(sys.argv[2])
spec = json.loads(sys.argv[3])   # {"ctx": [[value|null, name]...], "profiles": [[backend, ctxname, collected, path]...]}

def scenario(name, items):
    d = W / "scen" / name
    (d / "in").mkdir(parents=True)
    (d / "main.sv0").write_bytes((W / "main.sv0").read_bytes())
    (d / "in" / "x.sv0covmap.json").write_bytes((W / "x.sv0covmap.json").read_bytes())
    for i, p in enumerate(items):
        (d / "in" / f"{i}-{Path(p).name}").write_bytes(Path(p).read_bytes())
    e = io.StringIO()
    rc = cli.main(["merge", "in", "--output", "m.sv0covdata.json"], stdout=io.StringIO(), stderr=e, cwd=d)
    if rc:
        sys.exit(f"{name}: merge exit {rc}: {e.getvalue()}")
    rc = cli.main(["report", "--format", "json", "--output", "r.json", "m.sv0covdata.json"], stdout=io.StringIO(), stderr=e, cwd=d)
    if rc:
        sys.exit(f"{name}: report exit {rc}: {e.getvalue()}")
    out = io.StringIO()
    rc = cli.main(["report", "--format", "term", "--output", "-", "m.sv0covdata.json"], stdout=out, stderr=e, cwd=d)
    if rc:
        sys.exit(f"{name}: terminal report exit {rc}: {e.getvalue()}")
    rc = cli.main(["report", "--format", "html", "--output", "r.html", "m.sv0covdata.json"], stdout=io.StringIO(), stderr=e, cwd=d)
    if rc:
        sys.exit(f"{name}: HTML report exit {rc}: {e.getvalue()}")
    return json.loads((d / "r.json").read_text(encoding="utf-8")), out.getvalue()

def strip(x):
    # a view with the context bookkeeping removed: counts and what derives from them.
    if isinstance(x, dict):
        return {k: strip(v) for k, v in x.items() if k not in ("by_context", "context")}
    if isinstance(x, list):
        return [strip(v) for v in x]
    return x

def total(rep):
    return {p["point_id"]: p["count"]["value"] for p in rep["points"]}

errors = []
P = spec["profiles"]
allp = [p[3] for p in P if p[2]]
full, term = scenario("all", allp)
# CV-309: the HTML report of the same evidence passes the structural check,
# offers every context in its filter and shows the real source.
from sv0cov.formats.html import check_html
page = (W / "scen" / "all" / "r.html").read_bytes()
try:
    check_html(page)
except ValueError as exc:
    errors.append(f"the HTML report fails its check: {exc}")
text = page.decode("utf-8")
if text.count("<option ") != len(spec["ctx"]) + 1:
    errors.append("the HTML context filter does not offer every context and the aggregate")
if "string_len(getenv(&quot;SEL&quot;))" not in text:
    errors.append("the HTML report does not show the program's source")
want = [c[0] for c in spec["ctx"]]
got = [c["value"] for c in full["contexts"]]
if got != want:
    errors.append(f"report contexts {got!r}, want {want!r}")
if [c["context_index"] for c in full["contexts"]] != list(range(len(want))):
    errors.append("context indices are not 0..n-1 in order")

def sums(units, what):
    for u in units:
        c = u["count"]
        s = sum(b["value"] for b in c["by_context"])
        if c["value"] != s or c["lower_bound"] or c["saturated"]:
            errors.append(f"{what}: aggregate {c['value']} != sum of by_context {s}")
        if [b["context_index"] for b in c["by_context"]] != sorted(b["context_index"] for b in c["by_context"]):
            errors.append(f"{what}: by_context not ordered by context index")
sums(full["points"], "point")
sums(full["regions"], "region")
for b in full["branches"]:
    sums(b["outcomes"], "branch outcome")
if not any(p["count"]["value"] for p in full["points"]):
    errors.append("no point was counted")

views = {}
for i, (value, name) in enumerate(spec["ctx"]):
    own = [p[3] for p in P if p[1] == name and p[2]]
    one, _ = scenario(f"ctx-{name}", own)
    if [c["value"] for c in one["contexts"]] != [value]:
        errors.append(f"{name}: its own report lists contexts {[c['value'] for c in one['contexts']]!r}")
    v_full, v_one = context_view(full, i), context_view(one, 0)
    if v_full["context"]["value"] != value:
        errors.append(f"{name}: context_view names context {v_full['context']['value']!r}")
    for key in sorted(set(v_full) | set(v_one)):
        if key == "context":
            continue
        if strip(v_full[key]) != strip(v_one[key]):
            errors.append(f"{name}: context_view(report, {i})[{key!r}] differs from the view of the report merged from only {name}")
    # the view's counts are this context's by_context counts, and the context's report counts them as its aggregate
    byctx = {p["point_id"]: next((b["value"] for b in p["count"]["by_context"] if b["context_index"] == i), 0) for p in full["points"]}
    if {p["point_id"]: p["count"]["value"] for p in v_full["points"]} != byctx or total(one) != byctx:
        errors.append(f"{name}: view / own report point counts differ from by_context[{i}]")
    # native and VM counted alike: each alone gives half
    halves = {}
    for backend in ("native", "vm"):
        rep, _ = scenario(f"half-{name}-{backend}", [p[3] for p in P if p[1] == name and p[2] and p[0] == backend])
        halves[backend] = total(rep)
    if halves["native"] != halves["vm"]:
        errors.append(f"{name}: native and VM profiles counted differently")
    if {k: 2 * v for k, v in halves["native"].items()} != byctx:
        errors.append(f"{name}: the context's counts are not twice one run's")
    views[name] = sum(byctx.values())
if len(set(views.values())) != len(views):
    errors.append(f"the contexts did not count differently: {views}")

# COV-CTX-001: the same runs with no context
none, _ = scenario("nocontext", [p[3] for p in P if not p[2]])
if [c["value"] for c in none["contexts"]] != [None]:
    errors.append(f"no-context report lists contexts {[c['value'] for c in none['contexts']]!r}")
for key in ("points", "regions", "lines", "summary"):
    a = strip(full[key]); b = strip(none[key])
    if a != b:
        errors.append(f"COV-CTX-001: aggregate {key} differ between the context-collecting and the no-context report")
for key in ("branches", "contracts", "entities", "sources"):
    if strip(full[key]) != strip(none[key]):
        errors.append(f"COV-CTX-001: aggregate {key} differ between the context-collecting and the no-context report")
if total(full) != total(none):
    errors.append("COV-CTX-001: point counts differ")

print(json.dumps({"errors": errors, "term": term, "summary": full["summary"]["line"] if "line" in full["summary"] else None,
                  "unit_counts": [len(full["points"]), len(full["regions"]), len(full["lines"])]}))
"""


def main() -> int:
    ensure_built()
    py = modern_python()
    if py is None:
        print("coverage contexts: OK (skipped: no Python >= 3.10)")
        return 0
    sys.path.insert(0, str(ROOT / "sv0cov" / "src"))
    from sv0cov.formats.rawprofile import decode

    errors: list[str] = []
    with tempfile.TemporaryDirectory() as td:
        w = Path(td)
        (w / "vm").mkdir()
        (w / "main.sv0").write_bytes((HERE / "contexts.sv0").read_bytes())
        exe, sv0b = w / "x", w / "vm" / "x.sv0b"
        p = sv0("native-compile", "--coverage=instrument", "-o", str(exe), str(w / "main.sv0"))
        err = build_vm([str(w / "main.sv0")], sv0b)
        if p.returncode or err:
            print(f"coverage contexts: FAIL: build failed: {p.stderr.strip()[-300:]} {err or ''}", file=sys.stderr)
            return 1
        if (w / "vm" / "x.sv0covmap.json").read_bytes() != (w / "x.sv0covmap.json").read_bytes():
            errors.append("the C and VM maps differ")
        m = json.loads((w / "x.sv0covmap.json").read_bytes())
        profiles: list[list] = []  # [backend, context name, collected, path]

        def run(backend: str, name: str, ctx: str | None, sel: str, collected: bool) -> Path | None:
            pdir = w / f"p-{backend}-{name}-{'ctx' if collected else 'none'}"
            pdir.mkdir()
            env = transport(pdir, SV0COV_CONTEXT=ctx if collected else None, SEL=sel)
            if backend == "native":
                rc = subprocess.run([str(exe)], capture_output=True, env=env, timeout=60).returncode
            else:
                rc, out = vm_run(sv0b, w / "vm" / "x.sv0covbind.json", env)
            files = sorted(pdir.iterdir())
            if rc != 0 or len(files) != 1:
                errors.append(f"{backend} {name}: exit {rc}, {len(files)} profiles")
                return None
            return files[0]

        for backend in ("native", "vm"):
            for name, ctx, sel in CONTEXTS:
                for collected in (True, False):
                    f = run(backend, name, ctx, sel, collected)
                    if f is None:
                        continue
                    profiles.append([backend, name, collected, str(f)])
                    if not collected:
                        continue
                    # 1. decoded context and the raw header bytes
                    data = f.read_bytes()
                    prof = decode(data, map_counter_count=m["program_counter_count"], expected_map_id=bytes.fromhex(m["map_id"]))
                    label = f"{backend} {name}"
                    if prof.context != ctx:
                        errors.append(f"{label}: decoded context {prof.context!r}, want {ctx!r}")
                    flags = struct.unpack_from("<I", data, FLAGS_AT)[0]
                    clen = struct.unpack_from("<I", data, CTXLEN_AT)[0]
                    raw = b"" if ctx is None else ctx.encode("utf-8")
                    if bool(flags & CONTEXT_PRESENT) != (ctx is not None):
                        errors.append(f"{label}: CONTEXT_PRESENT is {bool(flags & CONTEXT_PRESENT)} (flags {flags:#x})")
                    if clen != len(raw):
                        errors.append(f"{label}: context_length {clen}, want {len(raw)}")
                    if data[HEADER:HEADER + clen] != raw:
                        errors.append(f"{label}: the context bytes are not the label's UTF-8")
                    if prof.backend != ("native" if backend == "native" else "vm-v1"):
                        errors.append(f"{label}: backend {prof.backend}")
        if len(CONTEXTS[3][1].encode()) != 256:
            errors.append("the long label is not 256 bytes")

        # AC-058: absent and empty profiles differ in exactly the flag (and
        # nothing else in the context fields), on each backend.
        for backend in ("native", "vm"):
            heads = {}
            for b, name, collected, path in profiles:
                if b == backend and collected and name in ("absent", "empty"):
                    d = Path(path).read_bytes()
                    heads[name] = (struct.unpack_from("<I", d, FLAGS_AT)[0], struct.unpack_from("<I", d, CTXLEN_AT)[0])
            if len(heads) == 2:
                (fa, la), (fe, le) = heads["absent"], heads["empty"]
                if not (la == 0 and le == 0 and fe == fa | CONTEXT_PRESENT and not fa & CONTEXT_PRESENT):
                    errors.append(f"{backend}: AC-058 absent flags/len {fa:#x}/{la}, empty {fe:#x}/{le}")

        # 2-4. merge and report through the sv0cov CLI.
        if len(profiles) == 16 and not errors:
            spec = {"ctx": [[c, n] for n, c, _ in CONTEXTS], "profiles": profiles}
            r = subprocess.run([*py, "-c", ANALYZE, str(ROOT / "sv0cov" / "src"), str(w), json.dumps(spec)],
                               capture_output=True, text=True, timeout=900)
            if r.returncode:
                errors.append(f"merge/report: {r.stderr.strip()[-1500:]}")
            else:
                res = json.loads(r.stdout)
                errors.extend(res["errors"])
                term = res["term"]
                for want in ("(default)", '""', '"shard é"', '"' + "é" * 128 + '"'):
                    if want not in term:
                        errors.append(f"the terminal report does not name the context {want[:20]!r}")
                bad = sorted({hex(ord(c)) for c in term if (ord(c) < 0x20 and c != "\n") or 0x7F <= ord(c) <= 0x9F})
                if bad:
                    errors.append(f"the terminal report holds raw control characters {bad}")
                units = res["unit_counts"]
    if errors:
        print("coverage contexts: FAIL", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    print("coverage contexts: OK (native + VM: absent / empty / unicode / 256-byte contexts decode and sit in the raw header exactly "
          f"(AC-058); merged report lists them in order, aggregate = sum of by_context over {units[0]} points / {units[1]} regions, "
          "context_view = per-context merge, native = VM per context, COV-CTX-001 aggregate unchanged, terminal names all four, HTML checked)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
