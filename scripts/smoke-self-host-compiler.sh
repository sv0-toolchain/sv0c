#!/usr/bin/env bash
# M3-S-052: heap + one-file C emit smoke (runs inside sv0c repo — standalone CI + meta submodule).
# Uses SML heap image (same behavior as SV0_SELF_HOST_COMPILER wrapper). Full CM.make: make legacy-bootstrap-check.
set -euo pipefail
SV0C_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$SV0C_ROOT"
make heap >/dev/null
first=""
if [[ -f "$SV0C_ROOT/lib/bootstrap-sources.list" ]]; then
  while IFS= read -r line || [[ -n "$line" ]]; do
    [[ "$line" =~ ^[[:space:]]*# ]] && continue
    [[ -z "${line//[[:space:]]/}" ]] && continue
    first="${line//[[:space:]]/}"
    break
  done <"$SV0C_ROOT/lib/bootstrap-sources.list"
fi
if [[ -z "$first" ]]; then
  first="lib/main.sv0"
fi
if [[ ! -f "$SV0C_ROOT/$first" ]]; then
  echo "smoke-self-host-compiler: missing $SV0C_ROOT/$first" >&2
  exit 1
fi
tmp="$(mktemp)"
log="$(mktemp)"
cleanup() {
  rm -f "$tmp" "$log"
}
trap cleanup EXIT
set +e
(cd "$SV0C_ROOT" && sml "@SMLload=build/sv0c" "$first" >"$tmp" 2>"$log")
ec=$?
set -e
if [[ "$ec" -ne 0 ]] || grep -q 'Error:' "$log"; then
  tail -40 "$log" >&2
  exit 1
fi
if ! grep -q '#include' "$tmp"; then
  echo "smoke-self-host-compiler: emitted C missing #include" >&2
  exit 1
fi
exit 0
