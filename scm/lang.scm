#|
\param x A list of constraints as described with the following syntax
<dl>
<dt>(&lt; a1 a2 ... an)</dt><dd>Puts an ordering on the given values. The values can either be strings (representing courses) or integers (representing a zero-indexed term number).</dd>
<dt>(!= a1 a2 ... an)</dt><dd>Require that the given values are not equal. The values can be either strings or integers.</dd>
<dt>(cap i m)</dt><dd>Cap term number <em>i</em> at <em>m</em> courses.</dd>
<dt>(req a1 a2 ... an)</dt><dd>Require that the given courses be taken. The values should be strings.</dd>
<dt>(oneof a1 a2 ... an)</dt><dd>Require that at least one of the given courses be taken. The values should be strings.</dd>
<dt>(season a1 (s1 s2 ... sn))</dt><dd>Restrict the terms in which the course a1 can be given to terms in the given seasons s1, s2, ..., sn.</dd>
</dl>

\param study-terms A list of symbols representing the seasons of each of the user's upcoming study terms.
|#

(use-modules (srfi srfi-1) (ice-9 match))

(define subsets-size-n
 (lambda (n l)
  (letrec
   [(subsets/k (lambda (n l k)
    (cond
     [(zero? n) (k '(()))]
     [(null? l) (k '())]
     [else (subsets/k (- n 1) (cdr l) (lambda (n-1-subsets) 
      (subsets/k n (cdr l) (lambda (n-cdr-subsets)
       (k (append (map (lambda (s) (cons (car l) s)) n-1-subsets)
                  n-cdr-subsets           
       ))
      ) ; end 2nd lambda
     ) ; end 2nd com/k
    ) ; end 1st lambda
   ) ; end 1st com/k
  ])))]
 (subsets/k n l identity)))
 )

(define parse
 (lambda (x study-terms)
  (letrec
   [(no-terms (length study-terms)) 
   (parse/k (lambda (x k)
    (cond
     [(null? x) (k '())]
     [else
      (match (car x) 
       [('< ai ...) (parse/k (cdr x) (lambda (rst) (k  
(fold (lambda (x y acc) (cons (list no-terms "prereq" x y) acc)) rst ai (cdr ai))
)))]
       [('!= ai ...) (parse/k (cdr x) (lambda (rst) (k (append rst
(call-with-values (lambda () (partition string? ai))
  (lambda (strings ints)
   (let
    ; The actual order of the elements within the tails is not important, as long as tails is a chain ordered by subsequence inclusion
    [(tails (fold (lambda (x acc) (cons (cons x (car acc)) acc)) '(()) strings))]
    ; For each string (course), set up an inequality constraint with all the other values, avoiding repeats. Do not set up inequality constraints between numbers (terms) as it'd be superfluous.
    (fold (lambda (tail acc) 
           (cond
            [(null? tail) acc]
            [else (append acc (fold 
                   (lambda (x acc2) 
                    (cons (list no-terms "bad term" (car tail) x) acc2)
                   ) 
                   '()
                   (append (cdr tail) ints)
                  ))
            ]))
     '() 
     tails)
   )
  )
 )
))))] ; end !=
       [('!! ai ...) (parse/k (cdr x) (lambda (rst) (k 
(append rst (fold (lambda (aj acc) (cons (list no-terms "bad term" aj no-terms) acc)) '() ai))
)))] ; end !!
       [(1 (? string? ai) ...) (parse/k (cdr x) (lambda (rst) (k (cons (list no-terms "oneof" (car ai) (cdr ai)) rst))))] ; end oneof
       [('ssn (? string? a1) (? list? seasons)) (parse/k (cdr x) (lambda (rst) (k (append rst (fold (lambda (n acc) (cons (list no-terms "bad term" a1 n) acc)) ; end lambda
'()
; The below finds the terms occurring in seasons that are not in the list "seasons"
(fold 
(lambda (n season acc) 
(cond
[(not (member season seasons)) (cons n acc)]
[else acc])
 ) ; end lambda
 '() (iota no-terms) study-terms) ; end inner fold
 ) ; end fold
) ; end append
) ; end k call
) ; end lambda
) ; end parse/k
] ; end ssn
      ) ; end match
     ] ; end else
    )
   ))]
   (parse/k x identity)
  )
 )
)

#|
\param constraint-lang A list of constraints specified in the microlanguage 
\param study-terms A list of the symbols 'S, 'W, 'F representing the seasons of remaining study terms.
\param  course-load-cap An integer specifying the maximum number of courses to enrol in in any given term. 
|# 
(define solve 
(lambda (constraints study-terms course-load-cap)
 (begin 
  (for-each (lambda (x) (apply add-course-constraint x)) (parse constraints study-terms))
  (for-each (lambda (n) (add-load-cap n course-load-cap)) (iota (length study-terms)))
  (catch 'no-more 
(lambda () (begin (while #t (display (find-solution)) (newline)))) 
(lambda (key . args) (begin (display (car args)) (newline)))
) ; end catch
) ; end begin
))
