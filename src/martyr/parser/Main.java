package martyr.parser;


import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

import org.w3c.dom.NodeList;

public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String tbox = "/home/tesla/Documentos/Tesis/TBOX2.owl";
		String option;
		Scanner scan = new Scanner(System.in);
		boolean exit = false;
		
		String q3 ="PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> "+
			"PREFIX owl: <http://www.w3.org/2002/07/owl#> " +
			"PREFIX tbox: <http://localhost/TBOX2.owl#>"+
			"SELECT ?class ?p ?o " +
			"WHERE {?class ?p ?o .} WHEN{ Serial{ [HuboMuerte,SubeIntensidad], huracan, HuboPerdidaHumana}}";
		try{
		Parser p2 = new Parser();
		//cargar el objeto serializado
		p2.getAction().showNumbers();
		while(!exit){
			System.out.println("Ingrese: \n1.- c para cargar el documento \n2.- q para ejecutar la consulta \n3- d para borrar el TDB \n4.- s para salir de Martyr \n5.- i Consultar Status\n");
			option = scan.nextLine();
			System.out.println(option);
			
			if(option.equalsIgnoreCase("c")){
				System.out.println("Cargando");
				p2.load(tbox,false);
			}else if(option.equalsIgnoreCase("q")){
				System.out.println("Ejecutando");
				p2.execute(q3);
			}else if(option.equalsIgnoreCase("d")){
				System.out.println("Borrando");
				p2.getQuery().clearOnt();
			}else if(option.equalsIgnoreCase("s")){
				System.out.println("Exiting");
				exit = true;
			}else if(option.equalsIgnoreCase("i")){
				System.out.println("Status Martyr");
				p2.getAction().showNumbers();
			}else{
				System.out.println("Presiono una opcion no valida intente de nuevo");
			}
			p2.getQuery().save();
		}
		//cargar tbox y abox de prueba
		
		
		
		}
		catch(ParserConfigurationException e){e.printStackTrace();} 
		catch (TransformerException e) {e.printStackTrace();}
		catch (IOException e){e.printStackTrace();}			
	}

}
