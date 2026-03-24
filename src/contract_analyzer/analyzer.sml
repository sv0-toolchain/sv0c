(* Contract analysis: Checker.type-checks `requires` as bool when present.
   This pass is a no-op for now; later it can lower contracts to sv0_runtime calls. *)
structure ContractAnalyzer :> ANALYZER = struct
  fun analyze prog = prog
end
