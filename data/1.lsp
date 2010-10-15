( 2
  ( %0 ( %1 ( CONS $ 0 ) ) )
  ( %1 ( IF ( CAR $ )
    ( %1 ( CONS ( CDR ( CAR $ ) ) ( + 1 ( CDR $ ) ) ) )
    ( CDR $ ) ) )
)

