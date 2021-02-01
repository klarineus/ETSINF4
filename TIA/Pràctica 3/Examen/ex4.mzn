% Variables 
%número de tasques
int: n=7;

var 0..24: TIA; %temps inicial adan
var 0..24: TFA; %temps final adan

var 4..24: TIE; %temps inicial eva
var 4..24: TFE; %temps final eva

var 0..24: holgura;

array [1 ..n] of var 0..24: temps_inicial;
array [1 ..n] of var 0..24: temps_final;
array [1 ..n] of var 0..24: duracio;

duracio=[1,2,1,4,4,2,6]; %en la reestricció de farmacia, ja canviem la duracio

include "alldifferent.mzn";


% REESTRICCIONS!!
% temps en la tintoreria
constraint ((temps_inicial[1]>=-2) /\ (temps_final[1]<=7)) \/ ((temps_inicial[1]>=14) /\ (temps_final[1]<=21));
constraint temps_final[1] = temps_inicial[1] + duracio[1];

% temps en la perruqueria
constraint ((temps_inicial[2] >= 4) /\ (temps_inicial[2] <= 9))
            \/ ((temps_inicial[2] >= 13) /\ (temps_inicial[2] <= 17));
constraint temps_final[2] = temps_inicial[2] + duracio[2];

% temps arreplegant el paquet
constraint ((temps_inicial[3] >= 8) /\ (temps_inicial[3] <= 14))
            \/ ((temps_inicial[3] >= 16) /\ (temps_inicial[3] <= 23));
constraint temps_final[3] = temps_inicial[3] + duracio[3];

% Temps comprant el regal
constraint ((temps_inicial[4] >= -2) /\ (temps_inicial[4] <= 22));
constraint temps_final[4] = temps_inicial[4] + duracio[4];

% temps en la farmacia
constraint if temps_inicial[5] <= 14 then temps_final[5] = temps_inicial[5] + duracio[5] + 1
            else temps_final[5] = temps_inicial[5] + duracio[5] endif;
            
% ADAN va a la farmacia
constraint if (temps_inicial[7] == temps_inicial[5] /\ temps_final[7] == temps_final[5]) then duracio[7]=duracio[5]
           else duracio[7]>= 6 endif;

% Temps amb l.amiga
constraint ((temps_inicial[6] >= 4) /\ (temps_final[6] <= 9)) \/ ((temps_inicial[6] >= 15) /\ (temps_final[6] <= 19));
constraint temps_final[6] = temps_inicial[6] + duracio[6];

% reestriccions per als trajectes d'Adan
constraint forall(i in 1..4) (
      forall(j in 1..4) (
            if i != j then
                  (temps_inicial[i] >= temps_final[j] + 1) \/ 
                  (temps_final[i] + 1 <= temps_inicial[j])
            endif 
      )
);
constraint forall(i in 1..4) (
      TIA + 1 <= temps_inicial[i] /\
      temps_final[i] + 1 <= TFA
);
% reestriccions per a la farmacia de adan
constraint forall(i in 1..4) (
            (temps_inicial[i] >= temps_final[7] + 1) \/ 
            (temps_final[i] + 1 <= temps_inicial[7])
);
constraint(
      TIA + 1 <= temps_inicial[7] /\
      temps_final[7] + 1 <= TFA
);

constraint temps_final[7] = temps_inicial[7] + duracio[7];

% reestriccions per als trajectes de Eva
constraint forall(i in 4..6) (
      forall(j in 4..6) (
            if i != j then
                  (temps_inicial[i] >= temps_final[j] + 1) \/ 
                  (temps_final[i] + 1 <= temps_inicial[j])
            endif 
      )
);
constraint forall(i in 4..6) (
      TIE + 1 <= temps_inicial[i] /\
      temps_final[i] + 1 <= TFE
);

% Eixida, arribada
constraint TFA < 24;
constraint TIE >= 4;

%descomentem linies depenent dels requisits de la pregunta de l'enunciat

% cual es la maxima holgura que puede tener Adan respecto a su llegada al aeropuerto y salida del vuelo?
constraint holgura = 24-TFA; 

% ¿Es posible que Eva acabe la vista a la amiga 4 horas antes, al menos, que Adan entre en la peluqueria? 
%constraint temps_final[6] <= temps_inicial[2] - 8;

% ¿Y al menos, 5 horas antes?
%constraint temps_final[6] <= temps_inicial[2] - 10;

solve maximize holgura;
%solve maximize TIA;
%solve minimize TFE;
%solve satisfy;

%eixida del programa
output[ "Tasques Adan: \n" ] ++ [show(temps_inicial[i])++"-"++show(temps_final[i]) ++"\n" | i in 1..4] ++ [ "Ixir de casa: " ++ show(TIA) ++ "\nAeroport: " ++ show(TFA) ++ "\n\n"] ++ [ "Holgura: " ++ show(holgura) ++ "\n\n"] ++ [ "Tasques Eva: \n"  ] ++ [show(temps_inicial[i])++"-"++show(temps_final[i]) ++"\n" | i in 4..6] ++ [ "Ixir de casa: " ++ show(TIE) ++ "\nArribar a casa: " ++ show(TFE)];