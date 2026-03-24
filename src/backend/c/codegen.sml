structure Codegen :> CODEGEN = struct
  fun emit _ =
    "#include \"sv0_runtime.h\"\n\
    \int main(void) { return 0; }\n"
end
