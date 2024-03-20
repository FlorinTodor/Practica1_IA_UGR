#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

//DECLARAR METODOS AUXILIARES

/**
 * @brief Metodo para dibujar los precipios predeterminados en todos los mapas 
 *
*/
void Rellenar_Precipicios_iniciales(vector<vector<unsigned char>>&matriz);

/**
 * @brief Metodo para dibujar en el MapaResultado
*/
void PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, const state &st, vector<vector<unsigned char>>&matriz,int nivel);

/**
 * @brief Movernos hacía otro lado (accion) en el caso de encontrar un muro
*/
Action movimiento(const vector<unsigned char> & terreno,const Action &accion);

/**
 * @brief Buscar casilla segun caracter
*/
bool buscar_casilla(const vector<unsigned char> & terreno,char caracter);

/**
 * @brief Mover sensores 1,2,3 según orientacion y caracter
*/
Action Moverse_orientacion(const vector<unsigned char> & sensores,const state &st,char caracter);





Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE; //AHora mismo no se mueve porque esto siempre es IDLE	
	int a; //Lo utilizamos para los giros, nos da igual su valor inicial
		
	Rellenar_Precipicios_iniciales(mapaResultado); //Rellenamos las casilas de precipios que comparten todos los mapas
	
	// Mostrar el valor de los sensores
	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC;
	switch (sensores.sentido)
	{
		case norte:	  cout << " Norte\n";	break;
		case noreste: cout << " Noreste\n";	break;
		case este:    cout << " Este\n";	break;
		case sureste: cout << " Sureste\n";	break;
		case sur:     cout << " Sur\n";	break;
		case suroeste:cout << " Suroeste\n";	break;
		case oeste:   cout << " Oeste\n";	break;
		case noroeste:cout << " Noroeste\n";	break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];

	cout << "  Agentes: ";
	for (int i=0; i<sensores.agentes.size(); i++)
		cout << sensores.agentes[i];

	cout << "\nColision: " << sensores.colision;
	cout << "  Reset: " << sensores.reset;
	cout << "  Vida: " << sensores.vida << endl<< endl;

	//Actualizar el mundo con la última accion
	//hacer otro case para correr
	
	switch (last_action){
		case actWALK:
			switch (current_state.brujula){
					case norte:current_state.fil--; break;
					case sur:current_state.fil++; break;
					case este: current_state.col++; break;
					case oeste:current_state.col--; break;
					case noreste: current_state.fil--; current_state.col++; break;
					case noroeste: current_state.fil--; current_state.col--; break;;
					case sureste: current_state.fil++; current_state.col++; break;
					case suroeste: current_state.fil++; current_state.col--; break;
			}
			break;		

		case actRUN:
			switch (current_state.brujula){
					case norte: current_state.fil -= 2; break;
					case sur: current_state.fil +=2; break;
					case este: current_state.col +=2; break;
					case oeste: current_state.col -=2; break;
					case noreste: current_state.fil-=2; current_state.col +=2; break;
					case noroeste: current_state.fil -=2; current_state.col -=2; break;
					case sureste: current_state.fil +=2; current_state.col +=2; break;
					case suroeste: current_state.fil +=2; current_state.col -=2; break;
			}
		break;
			
		case actTURN_SR: //Giro derecha -> actTurn_sr -> nueva = (antigua+1)%8;
				a = current_state.brujula;
				a = (a+1) %8;
				current_state.brujula = (Orientacion)a;
				sensores.sentido = current_state.brujula;
			break;
		case actTURN_L: //Giro izquiera --> actTurn_L  -> nueva =(antigua+6)%8
				a = current_state.brujula;
				a = (a+6) % 8; // +6 porque es giro de 90 grados para evitar negativos
				current_state.brujula = (Orientacion)a;
				sensores.sentido = current_state.brujula;
			break;

		break;
	}
	
	



	


	//Definir las reglas

	//Control de posicionamiento
	if (sensores.terreno[0]=='G' and !bien_situado){
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
		reseteado= false;
	}


	//Control de Mapa resultado
	if(sensores.reset){reseteado= true; bien_situado =false; colision_aldeano=false; zapatillas=false; bikini=false;}
	if (bien_situado && !reseteado){ //Nos hemos situado en una casilla de posicionamiento
		if(sensores.nivel  <= 3){//Para niveles 0,1 y 2
			PonerTerrenoEnMatriz(sensores.terreno,current_state,mapaResultado,sensores.nivel);
			sensores.posC = current_state.col;
			sensores.posF = current_state.fil;
			sensores.sentido = current_state.brujula;		
		} 
		
	}
	

	//Control de variables globales
	if (sensores.terreno[0] == 'D') {zapatillas=true;}
	if (sensores.terreno[0] == 'K') {bikini=true;}
	//Control de bateria
	if(sensores.nivel == 0 && sensores.bateria <= 4200){bateria_baja = true;}
	if(sensores.nivel == 1 && sensores.bateria <= 4600){bateria_baja = true;}
	if(sensores.nivel == 2 && sensores.bateria <= 4600){bateria_baja = true;}
	if(sensores.nivel == 3 && sensores.bateria <= 5000){bateria_baja = true;}
	//Control de colisión con aldeanos
	if((sensores.agentes[2] == 'a' or sensores.agentes[6] == 'a') && !colision_aldeano){colision_aldeano=true;}
	
	//Controles de casillas
	posicionamiento_encontrado =  buscar_casilla(sensores.terreno,'G');
	recarga_encontrada = buscar_casilla(sensores.terreno,'X');
	lobo_encontrado = buscar_casilla(sensores.terreno,'l');
	casilla_desconocida = buscar_casilla(sensores.terreno,'?');
	
	//Decision nueva accion andando (hay que diferenciar entre andar y correr)
	if( casilla_desconocida){accion = Moverse_orientacion(sensores.terreno,current_state,'?'); casilla_desconocida= false;}

	if(colision_aldeano){
		num_giros=0;
		accion=actTURN_SR; colision_aldeano=false;
	}
	else if (sensores.terreno[0] == 'X'){ //Nos situamos en una casilla de recarga
		if(bateria_baja){
			if( sensores.nivel == 0 && sensores.bateria >=4500){ bateria_baja=false;accion=actWALK;}
			if( sensores.nivel == 1 && sensores.bateria >=4800){ bateria_baja=false;accion=actWALK;}
			if( sensores.nivel == 2 && sensores.bateria >=4900){ bateria_baja=false;accion=actWALK;}
			if( sensores.nivel == 3 && sensores.bateria >=4500){ bateria_baja=false;accion=actWALK;}
			

			
		}
		if(bateria_baja && sensores.vida >= 1200){
			accion=actIDLE;
		}
		
	}
	else if(posicionamiento_encontrado && !bien_situado){
		accion = Moverse_orientacion(sensores.terreno,current_state,'G');
	}
	else if(lobo_encontrado){
		accion= Moverse_orientacion(sensores.terreno,current_state,'l');
	}
	else if(recarga_encontrada && bateria_baja){
		accion = Moverse_orientacion(sensores.terreno,current_state,'X');
	}
	else if(((bikini == true &&  sensores.terreno[2] == 'A')|| (zapatillas == true &&  sensores.terreno[2] == 'B')) and sensores.agentes[2]== '_' ){
		accion = actWALK;
	}
	else if((( sensores.terreno[2] == 'T' && sensores.terreno[6]=='T') || (sensores.terreno[2] == 'S' && sensores.terreno[6]=='S')) and sensores.agentes[2]== '_' && sensores.agentes[6] == '_'){
		accion = actRUN;
	}
	else if ((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' 
		or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'K') and sensores.agentes[2]=='_'){
		accion = actWALK;
	} 
	else if (!girar_derecha) {
		accion = actTURN_L;
		girar_derecha = (rand() % 2 == 0);
	}
	else {
		accion = actTURN_SR;
		girar_derecha = (rand() % 2 == 0);
	}

	//Salida de muros


	
	//Evita bucle
	if(accion == actTURN_L || accion == actTURN_SR){
		num_giros++;
	}
	else num_giros = 0;

	if(num_giros > 7 && sensores.terreno[2]!='M' && sensores.terreno[2]!='P')	accion = actWALK; 




// Para tener en cuenta la acción final
last_action = accion;


return accion;


	
}


int ComportamientoJugador::interact(Action accion, int valor) //METODO IRRELEVANTE PARA ESTA PRACTICA
{
	return false;
}


//IMPLEMENTAR METODOS AUXILIARES




//En el guión se nos indica que todos los mapas comparten las mismas dimensiones de los precipios (externos al mapa central)
//Aunque dentro del propio mapa habrá más precipios
void Rellenar_Precipicios_iniciales(vector<vector<unsigned char>>&matriz){
	
	for(int i=0; i<matriz.size(); i++){
		for(int j=0; j<matriz.size(); j++) {
			if(i==0 || i==1 || i==2 || i==matriz.size() ||i==matriz.size()-1 || i==matriz.size()-2)	//Indicamos las tres primeras filas y las tres ultimas
			matriz[i][j] = 'P';
		
			else if(j==0 || j==1 || j==2 || j==matriz.size() || j==matriz.size()-1 || j==matriz.size()-2) //Indicamos las tres primeras columnas y las tres ultimas
			matriz[i][j] = 'P';
		}
	}
}

Action movimiento(const vector<unsigned char> & terreno, const Action &accion){
	Action acc;
	if((terreno[1] == 'M' or terreno[1] == 'P') and (terreno[2] == 'M' or terreno[2] == 'P') and (terreno[3] == 'M' or terreno[3] == 'P')){ 
			acc = accion;
			}
	return acc;
}	


bool buscar_casilla(const vector<unsigned char> & terreno,char caracter){
	int encontrado = false;

	for (int i=0; i< 16; ++i){
		if(terreno[i] == caracter ){ encontrado = true;}

	}
	return encontrado;
}

Action Moverse_orientacion(const vector<unsigned char> & terreno,const state &st,char caracter){
    Action accion;

    // Si el caracter es 'l' y el terreno en la posición 2 es igual a 'l', girar a la izquierda
    if (caracter == 'l' &&  caracter == 'a' && terreno[2] == caracter) {
        accion = actTURN_SR;
    }
    // Si el caracter no es 'l' y el terreno en la posición 2 es igual al caracter, avanzar
    else if (caracter != 'l' && caracter != 'a' && terreno[2] == caracter) {
        accion = actWALK;
    }
    else {
        // Determinar la acción basada en la brújula
        switch (st.brujula) {
            case norte:
                // Girar a la izquierda si el caracter está en la posición 1, girar a la derecha si está en la posición 3, avanzar en otro caso
                accion = (terreno[1] == caracter) ? actTURN_L : ((terreno[3] == caracter) ? actTURN_SR : actWALK);
                break;
            case sur:
                // Girar a la derecha si el caracter está en la posición 1, girar a la izquierda si está en la posición 3, avanzar en otro caso
                accion = (terreno[3] == caracter) ? actTURN_SR : ((terreno[1] == caracter) ? actTURN_L : actWALK);
                break;
            case este:
            case oeste:
            case noreste:
            case noroeste:
            case sureste:
            case suroeste:
                // Girar a la izquierda si el caracter está en la posición 1, girar a la derecha si está en la posición 3, avanzar en otro caso
                accion = (terreno[1] == caracter) ? actTURN_L : ((terreno[3] == caracter) ? actTURN_SR : actWALK);
                break;
        }
    }

    return accion;
}



void PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, const state &st, vector<vector<unsigned char>>&matriz, int nivel) {

switch(st.brujula){
	case norte:
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil-1][st.col-1] = terreno[1];
			matriz[st.fil-1][st.col] = terreno[2];
			matriz[st.fil-1][st.col+1] = terreno[3];
			matriz[st.fil-2][st.col-2] = terreno[4];
			matriz[st.fil-2][st.col-1] = terreno[5];
			if (nivel != 3) matriz[st.fil-2][st.col] = terreno[6];
			matriz[st.fil-2][st.col+1] = terreno[7];
			matriz[st.fil-2][st.col+2] = terreno[8];
			matriz[st.fil-3][st.col-3] = terreno[9];
			matriz[st.fil-3][st.col-2] = terreno[10];
			if(nivel != 3 ) matriz[st.fil-3][st.col-1] = terreno[11];
			if(nivel != 3) matriz[st.fil-3][st.col] = terreno[12];
			if(nivel != 3)  matriz[st.fil-3][st.col+1] = terreno[13];
			matriz[st.fil-3][st.col+2] = terreno[14];
			matriz[st.fil-3][st.col+3] = terreno[15];
	break;

	case este:
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil-1][st.col+1] = terreno[1];
			matriz[st.fil][st.col+1] = terreno[2];
			matriz[st.fil+1][st.col+1] = terreno[3];
			matriz[st.fil-2][st.col+2] = terreno[4];
			matriz[st.fil-1][st.col+2] = terreno[5];
			if (nivel != 3)matriz[st.fil][st.col+2] = terreno[6];
			matriz[st.fil+1][st.col+2] = terreno[7];
			matriz[st.fil+2][st.col+2] = terreno[8];
			matriz[st.fil-3][st.col+3] = terreno[9];
			matriz[st.fil-2][st.col+3] = terreno[10];
			if (nivel != 3)matriz[st.fil-1][st.col+3] = terreno[11];
			if (nivel != 3)matriz[st.fil][st.col+3] = terreno[12];
			if (nivel != 3)matriz[st.fil+1][st.col+3] = terreno[13];
			matriz[st.fil+2][st.col+3] = terreno[14];
			matriz[st.fil+3][st.col+3] = terreno[15];
	break;

	case oeste:
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil+1][st.col-1] = terreno[1];
			matriz[st.fil][st.col-1] = terreno[2];
			matriz[st.fil-1][st.col-1] = terreno[3];
			matriz[st.fil+2][st.col-2] = terreno[4];
			matriz[st.fil+1][st.col-2] = terreno[5];
			if (nivel != 3)matriz[st.fil][st.col-2] = terreno[6];
			matriz[st.fil-1][st.col-2] = terreno[7];
			matriz[st.fil-2][st.col-2] = terreno[8];
			matriz[st.fil+3][st.col-3] = terreno[9];
			matriz[st.fil+2][st.col-3] = terreno[10];
			if (nivel != 3)matriz[st.fil+1][st.col-3] = terreno[11];
			if (nivel != 3)matriz[st.fil][st.col-3] = terreno[12];
			if (nivel != 3)matriz[st.fil-1][st.col-3] = terreno[13];
			matriz[st.fil-2][st.col-3] = terreno[14];
			matriz[st.fil-3][st.col-3] = terreno[15];
	break;

	case sur:
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil+1][st.col+1] = terreno[1];
			matriz[st.fil+1][st.col] = terreno[2];
			matriz[st.fil+1][st.col-1] = terreno[3];
			matriz[st.fil+2][st.col+2] = terreno[4];
			matriz[st.fil+2][st.col+1] = terreno[5];
			if (nivel != 3)matriz[st.fil+2][st.col] = terreno[6];
			matriz[st.fil+2][st.col-1] = terreno[7];
			matriz[st.fil+2][st.col-2] = terreno[8];
			matriz[st.fil+3][st.col+3] = terreno[9];
			matriz[st.fil+3][st.col+2] = terreno[10];
			if (nivel != 3)matriz[st.fil+3][st.col+1] = terreno[11];
			if (nivel != 3)matriz[st.fil+3][st.col] = terreno[12];
			if (nivel != 3)matriz[st.fil+3][st.col-1] = terreno[13];
			matriz[st.fil+3][st.col-2] = terreno[14];
			matriz[st.fil+3][st.col-3] = terreno[15];
	break;

	case noreste:
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil-1][st.col] = terreno[1];
			matriz[st.fil-1][st.col+1] = terreno[2];
			matriz[st.fil][st.col+1] = terreno[3];
			matriz[st.fil-2][st.col] = terreno[4];
			matriz[st.fil-2][st.col+1] = terreno[5];
			if (nivel != 3)matriz[st.fil-2][st.col+2] = terreno[6];
			matriz[st.fil-1][st.col+2] = terreno[7];
			matriz[st.fil][st.col+2] = terreno[8];
			matriz[st.fil-3][st.col] = terreno[9];
			matriz[st.fil-3][st.col+1] = terreno[10];
			if (nivel != 3)matriz[st.fil-3][st.col+2] = terreno[11];
			if (nivel != 3)matriz[st.fil-3][st.col+3] = terreno[12];
			if (nivel != 3)matriz[st.fil-2][st.col+3] = terreno[13];
			matriz[st.fil-1][st.col+3] = terreno[14];
			matriz[st.fil][st.col+3] = terreno[15];
	break;	

	case noroeste:
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil][st.col-1] = terreno[1];
			matriz[st.fil-1][st.col-1] = terreno[2];
			matriz[st.fil-1][st.col] = terreno[3];
			matriz[st.fil][st.col-2] = terreno[4];
			matriz[st.fil-1][st.col-2] = terreno[5];
			if (nivel != 3)matriz[st.fil-2][st.col-2] = terreno[6];
			matriz[st.fil-2][st.col-1] = terreno[7];
			matriz[st.fil-2][st.col] = terreno[8];
			matriz[st.fil][st.col-3] = terreno[9];
			matriz[st.fil-1][st.col-3] = terreno[10];
			if (nivel != 3)matriz[st.fil-2][st.col-3] = terreno[11];
			if (nivel != 3)matriz[st.fil-3][st.col-3] = terreno[12];
			if (nivel != 3)matriz[st.fil-3][st.col-2] = terreno[13];
			matriz[st.fil-3][st.col-1] = terreno[14];
			matriz[st.fil-3][st.col] = terreno[15];
	break;

	case sureste:
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil][st.col+1] = terreno[1];
			matriz[st.fil+1][st.col+1] = terreno[2];
			matriz[st.fil+1][st.col] = terreno[3];
			matriz[st.fil][st.col+2] = terreno[4];
			matriz[st.fil+1][st.col+2] = terreno[5];
			if (nivel != 3)matriz[st.fil+2][st.col+2] = terreno[6];
			matriz[st.fil+2][st.col+1] = terreno[7];
			matriz[st.fil+2][st.col] = terreno[8];
			matriz[st.fil][st.col+3] = terreno[9];
			matriz[st.fil+1][st.col+3] = terreno[10];
			if (nivel != 3)matriz[st.fil+2][st.col+3] = terreno[11];
			if (nivel != 3)matriz[st.fil+3][st.col+3] = terreno[12];
			if (nivel != 3)matriz[st.fil+3][st.col+2] = terreno[13];
			matriz[st.fil+3][st.col+1] = terreno[14];
			matriz[st.fil+3][st.col] = terreno[15];
	break;

	case suroeste:
			matriz[st.fil][st.col] = terreno[0];
			matriz[st.fil+1][st.col] = terreno[1];
			matriz[st.fil+1][st.col-1] = terreno[2];
			matriz[st.fil][st.col-1] = terreno[3];
			matriz[st.fil+2][st.col] = terreno[4];
			matriz[st.fil+2][st.col-1] = terreno[5];
			if (nivel != 3)matriz[st.fil+2][st.col-2] = terreno[6];
			matriz[st.fil+1][st.col-2] = terreno[7];
			matriz[st.fil][st.col-2] = terreno[8];
			matriz[st.fil+3][st.col] = terreno[9];
			matriz[st.fil+3][st.col-1] = terreno[10];
			if (nivel != 3)matriz[st.fil+3][st.col-2] = terreno[11];
			if (nivel != 3)matriz[st.fil+3][st.col-3] = terreno[12];
			if (nivel != 3)matriz[st.fil+2][st.col-3] = terreno[13];
			matriz[st.fil+1][st.col-3] = terreno[14];
			matriz[st.fil][st.col-3] = terreno[15];
	break;
	}
}