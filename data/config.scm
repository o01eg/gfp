(begin
	(define config (make-hash-table))
	(hashq-set! config 'population-size 32)
	(hashq-set! config 'max-step-unchanged 20000)
	(hashq-set! config 'max-object-depth 16)
	(hashq-set! config 'max-individual-adfs 32)
	(hashq-set! config 'max-stop-moves 4)
	(hashq-set! config 'max-evalation-loops 10000)
	(hashq-set! config 'max-individual-steps 16)
)

