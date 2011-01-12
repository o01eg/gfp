(begin
	(define config (make-hash-table))
	(hashq-set! config 'population-size 50)
	(hashq-set! config 'max-step-unchanged 100000)
	(hashq-set! config 'max-object-depth 16)
	(hashq-set! config 'max-individual-adfs 36)
	(hashq-set! config 'max-stop-moves 4)
	(hashq-set! config 'max-evalation-loops 20000)
	(hashq-set! config 'max-individual-steps 12)
)

