# modules_generic_carrier

SS-U06 / SPEC UP-020, AC-031: a single user-declared generic carrier
(`enum Res<T, E>` in `lib/carrier.sv0`) instantiated with three distinct
payload layouts (`Res<usize,i32>`, `Res<i32,usize>`, `Res<i32,i32>`) in
`lib/ops.sv0` and consumed in `main.sv0`. Each construct + `match` round-trips
to the same value on the C backend and the native VM -- `--project` emit + cc
+ run -> exit 42; also in `vm_behavioral_parity`.
