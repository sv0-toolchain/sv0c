structure Span :> SPAN = struct
  type pos = {line : int, col : int, offset : int}
  type span = {file : string, start : pos, stop : pos}

  fun pos (l, c, off) = {line = l, col = c, offset = off}
  fun span (f, s, e) = {file = f, start = s, stop = e}

  val bogus =
    {file = "", start = {line = 0, col = 0, offset = 0},
                stop  = {line = 0, col = 0, offset = 0}}

  fun posLine ({line, ...} : pos) = line
  fun posCol ({col, ...} : pos) = col
  fun posOffset ({offset, ...} : pos) = offset

  fun spanFile ({file, ...} : span) = file
  fun spanStart ({start, ...} : span) = start
  fun spanStop ({stop, ...} : span) = stop

  fun posToString ({line, col, ...} : pos) =
    Int.toString line ^ ":" ^ Int.toString col

  fun toString ({file, start, stop} : span) =
    let val loc = posToString start ^ "-" ^ posToString stop
    in if file = "" then loc else file ^ ":" ^ loc end

  fun merge (a : span, b : span) =
    {file = #file a, start = #start a, stop = #stop b}

  fun length ({start, stop, ...} : span) =
    #offset stop - #offset start
end
