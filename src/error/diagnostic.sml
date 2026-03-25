structure Diagnostic :> DIAGNOSTIC = struct
  datatype severity = Error | Warning | Note

  type related = string * Span.span

  type diagnostic = {
    severity : severity,
    code : string,
    message : string,
    span : Span.span,
    related : related list,
    help : string list
  }

  exception Diag of diagnostic

  fun error (code, msg, sp) =
    {severity = Error, code = code, message = msg,
     span = sp, related = [], help = []}

  fun warning (code, msg, sp) =
    {severity = Warning, code = code, message = msg,
     span = sp, related = [], help = []}

  fun note (msg, sp) =
    {severity = Note, code = "", message = msg,
     span = sp, related = [], help = []}

  fun withRelated (d : diagnostic, rs) =
    {severity = #severity d, code = #code d, message = #message d,
     span = #span d, related = #related d @ rs, help = #help d}

  fun withHelp (d : diagnostic, hs) =
    {severity = #severity d, code = #code d, message = #message d,
     span = #span d, related = #related d, help = #help d @ hs}

  fun severity ({severity = s, ...} : diagnostic) = s
  fun message ({message = m, ...} : diagnostic) = m
  fun diagSpan ({span = sp, ...} : diagnostic) = sp

  fun severityToString Error = "error"
    | severityToString Warning = "warning"
    | severityToString Note = "note"

  fun getLine (source, lineNo) =
    let
      fun go (i, start, n) =
        if n = lineNo then
          let
            fun findEnd j =
              if j >= String.size source then j
              else if String.sub (source, j) = #"\n" then j
              else findEnd (j + 1)
          in SOME (String.substring (source, start, findEnd start - start)) end
        else if i >= String.size source then NONE
        else if String.sub (source, i) = #"\n" then go (i + 1, i + 1, n + 1)
        else go (i + 1, start, n)
    in
      if lineNo < 1 then NONE else go (0, 0, 1)
    end

  fun spaces n = CharVector.tabulate (n, fn _ => #" ")
  fun carets n = CharVector.tabulate (Int.max (n, 1), fn _ => #"^")

  fun padLeft (s, width) =
    let val pad = width - String.size s
    in if pad > 0 then spaces pad ^ s else s end

  fun format source (d : diagnostic) =
    let
      val sev = severityToString (#severity d)
      val codeStr =
        if #code d = "" then "" else "[" ^ #code d ^ "]"
      val header = sev ^ codeStr ^ ": " ^ #message d ^ "\n"

      val sp = #span d
      val file = Span.spanFile sp
      val startPos = Span.spanStart sp
      val line = Span.posLine startPos
      val col = Span.posCol startPos
      val loc = "  --> " ^
        (if file = "" then "" else file ^ ":") ^
        Int.toString line ^ ":" ^ Int.toString col ^ "\n"

      val lineStr = Int.toString line
      val gutterWidth = Int.max (String.size lineStr, 2)
      val gutter = spaces gutterWidth ^ " | "
      val blank = gutter ^ "\n"

      val snippet =
        case source of
          NONE => ""
        | SOME src =>
            (case getLine (src, line) of
               NONE => ""
             | SOME srcLine =>
                 let
                   val spanLen = Int.max (Span.length sp, 1)
                   val numbered = padLeft (lineStr, gutterWidth) ^
                                  " | " ^ srcLine ^ "\n"
                   val underline = gutter ^
                                   spaces (col - 1) ^
                                   carets spanLen ^ "\n"
                 in blank ^ numbered ^ underline end)

      val relatedStr = String.concat (
        List.map (fn (msg, rsp) =>
          let
            val rline = Span.posLine (Span.spanStart rsp)
            val rcol = Span.posCol (Span.spanStart rsp)
            val rfile = Span.spanFile rsp
          in
            gutter ^ "= note: " ^ msg ^
            " at " ^ rfile ^ ":" ^
            Int.toString rline ^ ":" ^
            Int.toString rcol ^ "\n"
          end) (#related d))

      val helpStr = String.concat (
        List.map (fn h => gutter ^ "= help: " ^ h ^ "\n") (#help d))
    in
      header ^ loc ^ snippet ^ relatedStr ^ helpStr ^ "\n"
    end

  fun report out source d =
    TextIO.output (out, format source d)

  fun reportAll out source ds =
    List.app (report out source) (sortBySpan ds)

  and sortBySpan ds =
    let
      fun cmp (a : diagnostic, b : diagnostic) =
        let
          val la = Span.posLine (Span.spanStart (#span a))
          val lb = Span.posLine (Span.spanStart (#span b))
        in
          if la <> lb then Int.compare (la, lb)
          else Int.compare (
            Span.posCol (Span.spanStart (#span a)),
            Span.posCol (Span.spanStart (#span b)))
        end
    in
      ListMergeSort.sort (fn (a, b) => cmp (a, b) = GREATER) ds
    end

  fun hasErrors ds = List.exists (fn d => severity d = Error) ds
  fun errorCount ds = length (List.filter (fn d => severity d = Error) ds)
end
