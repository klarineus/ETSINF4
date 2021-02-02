(deftemplate vivienda ; definición de la clase vivienda
    (slot categoria (type INTEGER))
    (slot edad_aparente (type INTEGER))
	(slot vue_momentum (type FLOAT))
	(slot vue_maximum (type FLOAT))
    (slot numero_ventanas (type INTEGER))
)
(deftemplate categoria_dif  ;definición de la variable difusa
	0 150 puntos ;universo
	(
		(economica (40 1) (70 0))
		(estandar (40 0) (70 1) (100 0))
		(intermedia (70 0) (100 1)(130 0))
		(alta (100 0)(130 1))
	)
)


(deftemplate edad_aparente_dif  ;definición de la variable difusa
	0 100 anys
	(
		(reciente (0 1) (12 0))
		(nuevo (0 0) (12 1) (24 0))
		(medio (24 0) (36 1)(48 0))
		(viejo (48 0)(60 1))
	)
)

(deftemplate vue_dif ; variable difusa
	0 10000 euros ;universo
	(
		(bajisimo (500 1) (1500 0))
		(bajo (500 0) (1500 1) (2500 0))
		(medio (2500 0) (3500 1)(4500 0))
		(alto (4500 0) (5500 1)(6500 0))
		(altisimo (5500 0)(6500 1))
	)
)

(defrule r1  ;estas creant un hecho difuso
	(categoria_dif alta)
	(edad_aparente_dif reciente)
	=>
	(assert (vue_dif altisimo))
)

(defrule r2
	(categoria_dif alta)
	(edad_aparente_dif not [ medio or viejo ])
	=>
	(assert (vue_dif alto))
)

(defrule r3
	(categoria_dif alta)
	(edad_aparente_dif not [ reciente or nuevo ])
	=>
	(assert (vue_dif medio))
)

(defrule r4
	(categoria_dif intermedia)
	(edad_aparente_dif nuevo)
	=>
	(assert (vue_dif not [ medio or alto ]))
)

(defrule r5
	(categoria_dif intermedia)
	(edad_aparente_dif not [ medio or viejo ]	)
	=>
	(assert (vue_dif bajo))
)

(defrule r6
	(categoria_dif estandar)
	(edad_aparente_dif nuevo)
	=>
	(assert (vue_dif medio))
)

(defrule r7
	(categoria estandar)
	(edad_aparente viejo)
	=>
	(assert (vuedif bajisimo))
)

(defrule r8
	(categoria_dif economica)
	(edad_aparente_dif nuevo)
	=>
	(assert (vue_dif not [ bajo or medio ]))
)

(defrule r9
	(categoria_dif economica)
	(edad_aparente_dif not [ reciente or nuevo ])
	=>
	(assert (vue_dif bajisimo))
)

(defrule finestres1
	(numero_ventanas ?f)
	(test (< ?f 3))
	=>
	(assert (vue_dif more-or-less bajo))
)

(defrule finestres2
	(numero_ventanas ?f)
	(test (> ?f 5))
	=>
	(assert (vue_dif very alto))
)

(deffunction fuzzify (?fztemplate ?value ?delta)
	(bind ?low (get-u-from ?fztemplate))
	(bind ?hi (get-u-to ?fztemplate))

	(if (<= ?value ?low)
		then
			(assert-string (format nil "(%s (%g 1.0) (%g 0.0))" ?fztemplate ?low ?delta))
		else
			(if (>= ?value ?hi)
				then
					(assert-string (format nil "(%s (%g 0.0) (%g 1.0))" ?fztemplate (- ?hi ?delta) ?hi))
				else
					(assert-string (format nil "(%s (%g 0.0) (%g 1.0) (%g 0.0))" ?fztemplate (max ?low (- ?value ?delta)) ?value (min ?hi (+ ?value ?delta)) ))
			)
	)
)

(deffunction proces ()
	(reset)
	(printout t "Introdueix puntuacio de la vivenda: " crlf)
	;lee de consola un valor crisp y aserta su valor fusificado
	(bind ?Redad (read))
	(fuzzify categoria_dif ?Redad 0)

	(printout t "Introdueix edad de la vivenda: " crlf)
	;lee de consola un valor que pot ser crisp o fuzzificat ja
	(bind ?Redad (read))
	(if (integerp ?Redad)
		then
			(fuzzify edad_aparente_dif ?Redad 0)
		else
			(assert-string (format nil "(edad_aparente_dif %s)" ?Redad))
	
	)
	(printout t "Introdueix nombre de finestres: " crlf)
	;llig de consola un valor crisp
	(bind ?Redad (read))
	(assert (numero_ventanas ?Redad))

	(run)
)

(defrule defuzzificar
	(declare (salience -1))
	(vue_dif ?val)
	=>
	(assert (crisp vue_moment (moment-defuzzify ?val)))
	(assert (crisp vue_maximum (maximum-defuzzify ?val)))
	
	(printout t "VUE moment es: " (moment-defuzzify ?val) crlf)
	(printout t "VUE maximum es: " (maximum-defuzzify ?val) crlf)
	

	(halt)
)