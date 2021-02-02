(define 
  (domain transportemultimodal)
  (:requirements :durative-actions :typing :fluents)

  (:types usuario ciudad - object)
  (:predicates (at ?x - usuario ?c - ciudad)
               (conexion-bus ?c1 - ciudad ?c2 - ciudad)
               (conexion-tren ?c1 - ciudad ?c2 - ciudad)
               (conexion-metro ?c1 - ciudad ?c2 - ciudad)
               (tiene-ticket-tren ?x - usuario)
               (residente ?u - usuario)
  )

  (:functions (distancia ?c1 - ciudad ?c2 - ciudad)
              (velocidad-bus)
              (velocidad-metro)
              (velocidad-tren)
              (precio-metro)
              (precio-tren)
              (saldo ?x - usuario)
              (bonometro ?x - usuario)
              (recarga-bono)
              (compra-tren)
              (total-coste)
  )


  (:durative-action viajabus
     :parameters (?u - usuario ?c1 - ciudad ?c2 - ciudad)
     :duration (= ?duration (/
        (distancia ?c1 ?c2)
        (velocidad-bus)
      ))
     :condition (and (at start (at ?u ?c1))
					 (at start (residente ?u))
                     (over all (residente ?u))
					 (at start (conexion-bus ?c1 ?c2))
                     (over all (conexion-bus ?c1 ?c2)))
     :effect (and (at start (not (at ?u ?c1)))
                  (at end (at ?u ?c2))))

  (:durative-action viajatren 
   :parameters (?u - usuario ?c1 - ciudad ?c2 - ciudad)
   :duration (= ?duration (/ 
      (distancia ?c1 ?c2) (velocidad-tren)))
   :condition (and 	(at start (at ?u ?c1))
					(at start (tiene-ticket-tren ?u))
					(over all (tiene-ticket-tren ?u))
					(at start (conexion-tren ?c1 ?c2))
					(over all (conexion-tren ?c1 ?c2)))
   :effect (and (at start (not (at ?u ?c1)))
                (at end (at ?u ?c2))
                (at end 
                (not(tiene-ticket-tren ?u)))))

  (:durative-action viajametro 
   :parameters (?u - usuario ?c1 - ciudad ?c2 - ciudad)
   :duration (= ?duration (/ 
      (distancia ?c1 ?c2) (velocidad-metro)))
   :condition (and 
					(at start (at ?u ?c1))
					(at start (> (bonometro ?u) 
                           0))
					(over all (> (bonometro ?u) 
                           0))
					(at start (conexion-metro ?c1 ?c2))	   
					(over all (conexion-metro ?c1 ?c2)))
   :effect (and (at start (not (at ?u ?c1)))
					(at end (at ?u ?c2))
					(at end 
					(decrease (bonometro ?u) 
                              1))))
                                    
  (:durative-action recargar-bono
   :parameters (?u - usuario)
   :duration (= ?duration (recarga-bono))
   :condition (and 
					(at start (>= (saldo ?u) 
                           (precio-metro))))
   :effect (and
					(at start (decrease (saldo ?u) (precio-metro)))
					(at start (increase (total-coste) (precio-metro)))
					(at end (increase (bonometro ?u) 10))
   )
  )
  (:durative-action comprar-ticket
   :parameters (?u - usuario)
   :duration (= ?duration (compra-tren))
   :condition (and 
                  (at start (>= (saldo ?u) precio-tren)))
   :effect (and
                  (at end (tiene-ticket-tren ?u))
                  (at start (decrease (saldo ?u) (precio-tren)))
                  (at start (increase (total-coste) (precio-tren)))
   )))