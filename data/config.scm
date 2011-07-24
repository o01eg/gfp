(begin
	(define config (make-hash-table))
	(hashq-set! config 'population-size 75)
	(hashq-set! config 'max-step-unchanged 10000)
	(hashq-set! config 'max-object-depth 16)
	(hashq-set! config 'max-individual-adfs 32)
	(hashq-set! config 'max-stop-moves 3)
	(hashq-set! config 'max-evalation-loops 20000)
	(hashq-set! config 'max-individual-steps 12)
)

