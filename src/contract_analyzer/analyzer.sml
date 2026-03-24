(* Contract analysis: requires/ensures are type-checked in Checker and lowered
   to sv0_requires / sv0_ensures in Lowering. This pass is still a pass-through. *)
structure ContractAnalyzer :> ANALYZER = struct
  fun analyze prog = prog
end
