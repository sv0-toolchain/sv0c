(* Textual include expansion for .sv0 sources (transliteration / bootstrap helper).
   Top-level lines of the form:   include "relative/path.sv0";
   Paths are relative to the directory of the file containing the line; ".." rejected.
   Cycles are rejected (E0323). Bad paths (E0322). Processed before lexing. *)

structure IncludeExpand = struct

  fun isSpace (c : char) : bool =
    c = #" " orelse c = #"\t" orelse c = #"\n" orelse c = #"\v" orelse c = #"\f"

  fun trim (s : string) : string =
    let
      val n = String.size s
      fun left (i : int) : int =
        if i < n andalso isSpace (String.sub (s, i)) then left (i + 1) else i
      fun right (j : int) : int =
        if j > 0 andalso isSpace (String.sub (s, j - 1)) then right (j - 1) else j
      val lo = left 0
      val hi = right n
    in
      if lo >= hi then "" else String.substring (s, lo, hi - lo)
    end

  fun chompCR (line : string) : string =
    let val n = String.size line
    in
      if n > 0 andalso String.sub (line, n - 1) = #"\r" then
        String.substring (line, 0, n - 1)
      else
        line
    end

  fun pathOk (rel : string) : bool =
    rel <> ""
    andalso not (String.isSubstring ".." rel)
    andalso
      (case String.sub (rel, 0) of
         #"/" => false
       | _ => true)

  fun parseIncludeLine (line : string) : string option =
    let
      val s = trim (chompCR line)
      val prefix = "include \""
      val plen = String.size prefix
    in
      if String.isPrefix prefix s then
        let val after = String.extract (s, plen, NONE)
            fun findQuote (i : int) : int option =
              if i >= String.size after then
                NONE
              else if String.sub (after, i) = #"\"" then
                SOME i
              else
                findQuote (i + 1)
        in
          case findQuote 0 of
            NONE => NONE
          | SOME q =>
              let
                val p = String.substring (after, 0, q)
                val rest = trim (String.extract (after, q + 1, NONE))
              in
                if rest = ";" andalso pathOk p then SOME p else NONE
              end
        end
      else
        NONE
    end

  fun expand (visited : string list) (hostAbs : string) (source : string) : string =
    let
      val lines = String.fields (fn c => c = #"\n") source
      fun oneLine (line : string) : string =
        case parseIncludeLine line of
          NONE => line
        | SOME rel =>
            let
              val hostDir = OS.Path.dir hostAbs
              val merged = OS.Path.concat (hostDir, rel)
              val nestedAbs = OS.FileSys.fullPath merged
              val stack = hostAbs :: visited
            in
              if List.exists (fn p => p = nestedAbs) stack then
                raise Fail ("E0323: include cycle involving `" ^ nestedAbs ^ "`")
              else ();
              let
                val ins = TextIO.openIn nestedAbs
                val raw = TextIO.inputAll ins
                val () = TextIO.closeIn ins
              in
                expand stack nestedAbs raw
              end
            end
    in
      String.concatWith "\n" (map oneLine lines)
    end

  fun expandFile (hostPath : string) (source : string) : string =
    expand [] (OS.FileSys.fullPath hostPath) source

end
