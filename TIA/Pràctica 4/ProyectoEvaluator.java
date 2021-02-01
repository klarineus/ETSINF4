package proyecto;

import java.util.ArrayList;
import java.util.Collections;

import org.opt4j.core.Objectives;
import org.opt4j.core.Objective.Sign;
import org.opt4j.core.problem.Evaluator;


public class ProyectoEvaluator implements Evaluator<ArrayList<Integer>>
{
	public Objectives evaluate (ArrayList<Integer> phenotype)
	{
		int resultat = 0;
		int numVoluntaris = 0;
		
		ArrayList<Integer> vacunas = new ArrayList<Integer>();
		vacunas.add(1);vacunas.add(2);vacunas.add(0);
		
		ArrayList<Integer> jovenes = new ArrayList<Integer>(phenotype.subList(0, 4));
		ArrayList<Integer> adultos = new ArrayList<Integer>(phenotype.subList(4, 8));
		ArrayList<Integer> mayores = new ArrayList<Integer>(phenotype.subList(8, 13));
		
		int jovensVac1 = Collections.frequency(jovenes, 1);
		int jovensVac0 = Collections.frequency(jovenes, 0);
		int jovensVac2 = Collections.frequency(jovenes, 2);
		//System.out.println(jovensVac2);
		
		// per als adults
		
		int adults1 = Collections.frequency(adultos, 1);
		int adults0 = Collections.frequency(adultos, 0);
		int adults2 = Collections.frequency(adultos, 2);
		//System.out.println(adults2);
		
		// per als majors
		
		int majors1 = Collections.frequency(mayores, 1);
		int majors0 = Collections.frequency(mayores, 0);
		int majors2 = Collections.frequency(mayores, 2);
				
		//if(!jovenes.containsAll(vacunas) ||!adultos.containsAll(vacunas) || !mayores.containsAll(vacunas)) {
		if(jovensVac1<1 || jovensVac2<1 || jovensVac0 < 1 || majors0< 1 || majors1<1 
				|| majors2<1 || adults1<1 || adults0<1 || adults2 < 1) {
			resultat = Integer.MAX_VALUE;
		}
		else 
		{
			//System.out.println("he entrat");
			for (int id = 0; id < phenotype.size(); id++)
			{
				resultat += DatosVacunas.matrizCostes[phenotype.get(id)][id];
				if(phenotype.get(id)!= 2) numVoluntaris += DatosVacunas.numeroVoluntarios[id];
			}
		}
		
		Objectives objectives = new Objectives();
		objectives.add("Coste total-MIN", Sign.MIN, resultat);
		// opcional
		objectives.add("Numero de voluntaris-MAX", Sign.MAX,numVoluntaris);
		return objectives;
	}
}
