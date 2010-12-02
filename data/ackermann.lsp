( 1
  ( %0 ( IF ( > ( CAR $ ) 0 )
    ( IF ( > ( CDR $ ) 0 )
      ( %0 ( CONS ( - ( CAR $ ) 1 ) ( %0 ( CONS ( CAR $ ) ( - ( CDR $ ) 1 ) ) ) ) )
      ( %0 ( CONS ( - ( CAR $ ) 1 ) 1 ) )
     )
    ( + 1 ( CDR $ ) )
   ) )
)
