# modules_diamond

SS-U07 / SPEC UP-024: a shared type (`Cfg`) reached through two import paths
(`main` -> `left` -> `common` and `main` -> `right` -> `common`) must resolve
as one declaration on both project backends -- no duplicate-type diagnostic.
`--project` emit + cc + run -> exit 42; also in `vm_behavioral_parity`.
