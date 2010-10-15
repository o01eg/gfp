( 2
  ( %0 ( %1 ( CONS 1 $ ) ) )
  ( %1 ( IF ( CDR ( CDR $ ) )
    ( IF ( EQ
        ( CAR ( CDR $ ) )
        ( CAR ( CDR ( CDR $ ) ) ) )
      ( CAR $ )
      ( %1 ( CONS
        ( + 1 ( CAR $ ) )
        ( CONS ( CAR ( CDR $ ) )
          ( CDR ( CDR ( CDR $ ) ) ) ) ) ) ) 
    -1 ) )
)
