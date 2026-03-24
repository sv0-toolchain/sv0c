signature SPAN = sig
  type pos
  type span

  val pos : int * int * int -> pos
  val span : string * pos * pos -> span
  val bogus : span

  val posLine : pos -> int
  val posCol : pos -> int
  val posOffset : pos -> int

  val spanFile : span -> string
  val spanStart : span -> pos
  val spanStop : span -> pos

  val toString : span -> string
  val posToString : pos -> string
  val merge : span * span -> span
  val length : span -> int
end
