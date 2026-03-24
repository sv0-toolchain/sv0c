signature DIAGNOSTIC = sig
  datatype severity = Error | Warning | Note

  type related
  type diagnostic

  val error : string * string * Span.span -> diagnostic
  val warning : string * string * Span.span -> diagnostic
  val note : string * Span.span -> diagnostic

  val withRelated : diagnostic * (string * Span.span) list -> diagnostic
  val withHelp : diagnostic * string list -> diagnostic

  val severity : diagnostic -> severity
  val message : diagnostic -> string
  val diagSpan : diagnostic -> Span.span

  val format : string option -> diagnostic -> string
  val report : TextIO.outstream -> string option -> diagnostic -> unit
  val reportAll : TextIO.outstream -> string option -> diagnostic list -> unit

  val hasErrors : diagnostic list -> bool
  val errorCount : diagnostic list -> int
  val sortBySpan : diagnostic list -> diagnostic list
end
