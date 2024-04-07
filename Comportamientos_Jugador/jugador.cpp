#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

//DECLARAR METODOS AUXILIARES

/**
 * @brief Función para rellenar los precipicios predeterminados en todos los mapas.
 *
 * @param matriz La matriz que representa el mapa.
 */
void Rellenar_Precipicios_iniciales(vector<vector<unsigned char>>& matriz);

/**
 * @brief Función para dibujar el terreno en la matriz de resultado.
 *
 * @param terreno El vector que contiene el terreno.
 * @param st El estado del jugador.
 * @param matriz La matriz de resultado.
 * @param nivel El nivel actual.
 */
void PonerTerrenoEnMatriz(const vector<unsigned char> & terreno, const state &st, vector<vector<unsigned char>>&matriz,int nivel);

/**
 * @brief Función para determinar el movimiento a realizar en caso de encontrar un muro.
 *
 * @param terreno El vector que contiene el terreno.
 * @param accion La acción anterior.
 * @return La nueva acción a realizar.
 */
Action movimiento(const vector<unsigned char> & terreno,const Action &accion);

/**
 * @brief Función para buscar una casilla en el terreno según un caracter.
 *
 * @param terreno El vector que contiene el terreno.
 * @param caracter El caracter a buscar.
 * @return `true` si se encuentra el caracter, `false` en caso contrario.
 */
bool buscar_casilla(const vector<unsigned char> & terreno,char caracter);

/**
 * @brief Función para mover los sensores según la orientación y el caracter del terreno.
 *
 * @param sensores Los valores de los sensores.
 * @param st El estado del jugador.
 * @param caracter El caracter del terreno.
 * @return La nueva acción a realizar.
 */
Action Moverse_orientacion(const vector<unsigned char> & sensores,const state &st,char caracter);

/**
 * Determina si un conjunto de posiciones contiguas en el terreno conforman un muro.
 * @param terreno Vector que representa el terreno con tipos de terreno en posiciones específicas.
 * @return Devuelve true si el conjunto de posiciones conforma un muro, false en caso contrario.
 */
bool Son_muros(const vector<unsigned char> & terreno);


/**
 * Define una acción a tomar en función de la presencia de un muro frente al agente y los sensores proporcionados.
 * @param sensores Sensores proporcionados al agente, incluyendo información sobre el terreno circundante.
 * @param current_state Estado actual del agente, incluyendo su orientación.
 * @return Devuelve la acción a tomar por el agente (por ejemplo, girar, caminar) para entrar en el muro si es posible.
 */
Action EntrarMuro(Sensores sensores, const state &current_state);

/**
 * Verifica si hay un muro directamente delante del agente en su posición actual.
 * @param terreno Vector que representa el terreno con tipos de terreno en posiciones específicas.
 * @return Devuelve true si hay un muro delante del agente, false en caso contrario.
 */

bool HayMuroDelante(const vector <unsigned char> &terreno);

/**
 * Verifica si hay un hueco en el muro frente al agente, permitiendo su entrada.
 * @param terreno Vector que representa el terreno con tipos de terreno en posiciones específicas.
 * @param current_state Estado actual del agente, incluyendo su orientación.
 * @return Devuelve true si hay un hueco en el muro frente al agente, false en caso contrario.
 */
bool VeoHuecoMuro(const vector <unsigned char> &terreno,const state &current_state);

Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;
	int a; //Lo utilizamos para los giros, nos da igual su valor inicial
		
	//Rellenamos las casilas de precipios que comparten todos los mapas
	Rellenar_Precipicios_iniciales(mapaResultado); 
	
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
				
			break;
		case actTURN_L: //Giro izquiera --> actTurn_L  -> nueva =(antigua+6)%8
				a = current_state.brujula;
				a = (a+6) % 8; // +6 porque es giro de 90 grados para evitar negativos
				current_state.brujula = (Orientacion)a;
				
			break;



		break;
	}
	
	



	


	/*                   REGLAS                                   */

	//Control de posicionamiento
	if (sensores.terreno[0]=='G' and !bien_situado){
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
		reseteado= false;
	}



	//Detectamos colisión de muros



	//En el caso de que hubiera un reseteo se pierde todo
	if(sensores.reset){
	reseteado= true; bien_situado =false; colision_aldeano=false; zapatillas=false; bikini=false; hubo_colision=false; 
	girar_derecha=false; posicionamiento_encontrado=false; recarga_encontrada=false; num_giros=0;
	bateria_baja=false; colision_lobo=false; casilla_desconocida=false; colision_muro=false; son_muros=false;}


	
	
	/* SOLAMENTE PINTAMOS EN EL MAPARESULTADO SI ESTAMOS BIEN SITUADOS, NO HUBO COLISION CON MUROS(para evitar los errores de core por colision) Y NO HUBO RESETEO 
	*/
	if (bien_situado && !reseteado && !hubo_colision){ 	PonerTerrenoEnMatriz(sensores.terreno,current_state,mapaResultado,sensores.nivel);}
	


	

	//Control de variables globales
	if (sensores.terreno[0] == 'D') {zapatillas=true;}
	if (sensores.terreno[0] == 'K') {bikini=true;}


	//Control de bateria
	if(sensores.nivel == 0 && sensores.bateria <= 4200){bateria_baja = true;}
	if(sensores.nivel == 1 && sensores.bateria <= 4600){bateria_baja = true;}
	if(sensores.nivel == 2 && sensores.bateria <= 4600){bateria_baja = true;}
	if(sensores.nivel == 3 && sensores.bateria <= 5000){bateria_baja = true;}


	//Control de colisión con aldeanos
	if((sensores.agentes[2] == 'a' or sensores.agentes[6] == 'a' && sensores.colision) && !colision_aldeano){colision_aldeano=true;}
	
	//Controles de casillas, buscar en los 15 sensores de terreno
	posicionamiento_encontrado =  buscar_casilla(sensores.terreno,'G');
	recarga_encontrada = buscar_casilla(sensores.terreno,'X');
	casilla_desconocida = buscar_casilla(sensores.terreno,'?');
	
	//Detección de muros
	son_muros= Son_muros(sensores.terreno);

	

	//Determinar si en los tres sensores hay 2 o 3 muros
	if (son_muros){ colision_muro=true; accion=actTURN_SR;}
	
	if( casilla_desconocida && !son_muros){accion = Moverse_orientacion(sensores.terreno,current_state,'?'); casilla_desconocida= false;}

	if(sensores.colision &&colision_aldeano && !son_muros){
		num_giros=0;
		accion=actTURN_SR; colision_aldeano=false;
	}
	else if (sensores.terreno[0] == 'X' && bateria_baja){ //Nos situamos en una casilla de recarga y tenemos bateria baja
			if( sensores.nivel == 0 && sensores.bateria >=4500){ bateria_baja=false;accion=actWALK;}
			if( sensores.nivel == 1 && sensores.bateria >=4800){ bateria_baja=false;accion=actWALK;}
			if( sensores.nivel == 2 && sensores.bateria >=4900){ bateria_baja=false;accion=actWALK;}
			if( sensores.nivel == 3 && sensores.bateria >=4500){ bateria_baja=false;accion=actWALK;}
		if(bateria_baja && sensores.vida >= 700){ //No nos movemos hasta que se considere bastante bateria para seguir
			accion=actIDLE;
		}
		
	}
	else if(posicionamiento_encontrado && !bien_situado && !son_muros){ //Solo nos movemos hacia una casilla de posicionamiento si no estamos bien situados y no hay muros
		accion = Moverse_orientacion(sensores.terreno,current_state,'G');
	}
	else if(recarga_encontrada && bateria_baja && !son_muros){
		accion = Moverse_orientacion(sensores.terreno,current_state,'X');

	}
	else if(((bikini == true &&  sensores.terreno[2] == 'A')|| (zapatillas == true &&  sensores.terreno[2] == 'B')) and (sensores.agentes[2]== '_' && !son_muros)){
		accion = actWALK;
	}
	else if((( sensores.terreno[2] == 'T' && sensores.terreno[6]=='T') || (sensores.terreno[2] == 'S' && sensores.terreno[6]=='S')) and (sensores.agentes[2]== '_' && sensores.agentes[6] == '_' && !son_muros) and (!hubo_colision)){
		accion = actRUN;
	}
	else if ((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' 
		or sensores.terreno[2] == 'D' or sensores.terreno[2] == 'K') and sensores.agentes[2]=='_' && !son_muros){
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
	
	if (VeoHuecoMuro(sensores.terreno, current_state) && !HayMuroDelante(sensores.terreno)) {
    // Solamente tenemos en cuenta los ejes de orientación básicos

    // Revisión de posibles movimientos hacia la izquierda
    if (sensores.terreno[1] != 'M' && sensores.terreno[2] == sensores.terreno[3] && sensores.terreno[3] == 'M') {
        he_girado_izquierda = true;
    }

    // Revisión de posibles movimientos hacia adelante
    if (sensores.terreno[2] != 'M' && sensores.terreno[1] == sensores.terreno[3] && sensores.terreno[3] == 'M') {
        accion = actWALK;
    }

    // Revisión de posibles movimientos hacia la derecha
    if (sensores.terreno[3] != 'M' && sensores.terreno[2] == sensores.terreno[1] && sensores.terreno[1] == 'M') {
        accion = actTURN_SR;
    }

    // Revisión de otros movimientos hacia la izquierda
    if (sensores.terreno[4] != 'M' && sensores.terreno[5] == sensores.terreno[6] && sensores.terreno[6] == 'M') {
        he_girado_izquierda = true;
    }

    // Revisión de otros movimientos hacia adelante
    if (sensores.terreno[5] != 'M' && sensores.terreno[4] == sensores.terreno[6] && sensores.terreno[6] == 'M') {
        accion = actWALK;
    }

    // Más movimientos hacia adelante
    if (sensores.terreno[6] != 'M' && sensores.terreno[5] == sensores.terreno[7] && sensores.terreno[7] == 'M') {
        accion = actWALK;
    }

    // Revisión de movimientos hacia adelante con giro a la derecha
    if (sensores.terreno[7] != 'M' && sensores.terreno[6] == sensores.terreno[8] && sensores.terreno[8] == 'M') {
        accion = actWALK;
    }

    // Revisión de movimientos con giro a la derecha
    if (sensores.terreno[8] != 'M' && sensores.terreno[6] == sensores.terreno[7] && sensores.terreno[7] == 'M') {
        accion = actTURN_SR;
    }

    // Más movimientos hacia adelante (continuación)
    if (sensores.terreno[10] != 'M' && sensores.terreno[9] == sensores.terreno[11] && sensores.terreno[11] == 'M') {
        accion = actWALK;
    }

    // Más movimientos hacia adelante (continuación)
    if (sensores.terreno[11] != 'M' && sensores.terreno[10] == sensores.terreno[12] && sensores.terreno[12] == 'M') {
        accion = actWALK;
    }

    // Más movimientos hacia adelante (continuación)
    if (sensores.terreno[12] != 'M' && sensores.terreno[11] == sensores.terreno[13] && sensores.terreno[13] == 'M') {
        accion = actWALK;
    }

    // Más movimientos hacia adelante (continuación)
    if (sensores.terreno[13] != 'M' && sensores.terreno[12] == sensores.terreno[14] && sensores.terreno[14] == 'M') {
        accion = actWALK;
    }

    // Más movimientos hacia adelante (continuación)
    if (sensores.terreno[14] != 'M' && sensores.terreno[13] == sensores.terreno[15] && sensores.terreno[15] == 'M') {
        accion = actWALK;
    }

    // Revisión de movimientos hacia adelante con giro a la izquierda
    if (sensores.terreno[1] != 'M' && sensores.terreno[5] == sensores.terreno[11] && sensores.terreno[11] == 'M') {
        he_girado_izquierda = actTURN_SR;
    }

    // Revisión de movimientos con giro a la izquierda
    if (sensores.terreno[3] != 'M' && sensores.terreno[7] == sensores.terreno[13] && sensores.terreno[13] == 'M') {
        accion = actTURN_SR;
    }

    // Más movimientos hacia adelante (continuación)
    if (sensores.terreno[5] != 'M' && sensores.terreno[1] == sensores.terreno[11] && sensores.terreno[11] == 'M') {
        accion = actWALK;
    }

    // Más movimientos hacia adelante (continuación)
    if (sensores.terreno[7] != 'M' && sensores.terreno[3] == sensores.terreno[13] && sensores.terreno[13] == 'M') {
        accion = actWALK;
    }
}


	if(sensores.colision){
		int giro = rand()%3;
		hubo_colision=false;

		switch(giro){
			case 0: accion = actTURN_SR;
			case 1: he_girado_izquierda;
			case 2: accion = actTURN_L;

		}
	}
	
	//Evita bucle
	if(accion == actTURN_L || accion == actTURN_SR){
		num_giros++;
	}
	else num_giros = 0;


	if(num_giros > 7 && (sensores.terreno[2]!='M' && sensores.terreno[2]!='P') or sensores.agentes[2] !='_'){accion = actWALK; }


if(girar_izquierda_derecha && !he_girado_izquierda){
	accion = actTURN_SR;
	girar_izquierda_derecha = false;
}
	
if(he_girado_izquierda && !girar_izquierda_derecha){
	accion=actTURN_L;
	he_girado_izquierda=false;
	girar_izquierda_derecha=true;
	
}




//Para evitar colisciones con muros o precipicios cambiamos la accion
if(accion == actWALK  and( (sensores.terreno[2] == 'P' or  sensores.terreno[2] == 'M' ) or sensores.agentes[2] !='_')){accion=actTURN_SR; hubo_colision = false;}

last_action = accion;
return accion;
	
}


int ComportamientoJugador::interact(Action accion, int valor) //METODO IRRELEVANTE PARA ESTA PRACTICA
{
	return false;
}


//IMPLEMENTAR METODOS AUXILIARES
bool HayMuroDelante(const vector <unsigned char> &terreno){
	bool hay=false;
	if(terreno[2]=='M')
		hay=true;
	return hay;
}


bool VeoHuecoMuro(const vector <unsigned char> &terreno,const state &current_state){
	//Solamente tenemos en cuenta los ejes de orientación basicos
	if(current_state.brujula==norte or current_state.brujula==sur
	or current_state.brujula==este or current_state.brujula==oeste){
		if(terreno[1]!='M'){
			if(terreno[2] == 'M')
				if(terreno[3]=='M')
						return true;
		}
		if(terreno[2]!='M' ){
			if(terreno[1] == 'M')
				if(terreno[3]=='M')
						return true;
		}
		if(terreno[3]!='M' ){
			if(terreno[2] == 'M')
				if(terreno[1]=='M')
						return true;
		}	
		
		if(terreno[4]!='M' ){
			if(terreno[5] == 'M')
				if(terreno[6]=='M')
						return true;
		}

		if(terreno[5]!='M' ){
			if(terreno[4] == 'M')
				if(terreno[6]=='M')
						return true;
		}

		if(terreno[6]!='M' ){
			if(terreno[5] == 'M')
				if(terreno[7]=='M')
						return true;
		}

		if(terreno[7]!='M' ){
			if(terreno[6] == 'M')
				if(terreno[8]=='M')
						return true;
		}
	
		if(terreno[8]!='M' ){
			if(terreno[6] == 'M')
				if(terreno[7]=='M')
						return true;
		}

		if(terreno[10]!='M' ){
			if(terreno[11] == 'M')
				if(terreno[9]=='M')
						return true;
		}

		if(terreno[11]!='M' ){
			if(terreno[10] == 'M')
				if(terreno[12]=='M')
						return true;
		}

		if(terreno[12]!='M' ){
			if(terreno[11] == 'M')
				if(terreno[13]=='M')
						return true;
		}
		
		if(terreno[13]!='M' ){
			if(terreno[12] == 'M')
				if(terreno[14]=='M')
						return true;
		}

		if(terreno[14]!='M' ){
			if(terreno[13] == 'M')
				if(terreno[15]=='M')
						return true;
		}


		if(terreno[1]!='M'){
			if(terreno[5] == 'M')
				if(terreno[11]=='M')
						return true;
		}

		if(terreno[3]!='M' ){
			if(terreno[7] == 'M')
				if(terreno[13]=='M')
						return true;
		}

		if(terreno[5]!='M'){
			if(terreno[1] == 'M')
				if(terreno[11]=='M')
						return true;
		}

		if(terreno[7]!='M'){
			if(terreno[3] == 'M')
				if(terreno[13]=='M')
						return true;
		}
	}

	return false;
}

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

	for (int i=0; i< 16; ++i){
		if(terreno[i] == caracter ){ return true;}

	}
	return false;
}

Action Moverse_orientacion(const vector<unsigned char> & terreno,const state &st,char caracter){
    Action accion = actIDLE;


    if (caracter == 'a' && terreno[2] == caracter) {
        accion = actTURN_SR;
    }
    else if (caracter != 'l' && caracter != 'a' && terreno[2] == caracter) {
        accion = actWALK;
    }
    else {
        // Determinar la acción basada en la brújula
			switch (st.brujula) {
				case norte:
					// Girar a la izquierda si el caracter está en la posición 1, girar a la derecha si está en la posición 3, avanzar en otro caso
					if (terreno[1] == caracter) { accion = actTURN_L;} else if((terreno[3] == caracter)){  accion = actTURN_SR;} else{ if(!Son_muros(terreno)){  accion = actWALK;}else{  accion = actTURN_SR;}}break;
				case sur:
					// Girar a la derecha si el caracter está en la posición 1, girar a la izquierda si está en la posición 3, avanzar en otro caso
					if (terreno[3] == caracter) {  accion =  actTURN_SR;} else if((terreno[1] == caracter)){  accion = actTURN_L;} else{ if(!Son_muros(terreno)){  accion = actWALK;}else{  accion = actTURN_SR;}}break;
				case este:
				case oeste:
				case noreste:
				case noroeste:
				case sureste:
				case suroeste:
					// Girar a la izquierda si el caracter está en la posición 1, girar a la derecha si está en la posición 3, avanzar en otro caso
					if (terreno[1] == caracter) {  accion =  actTURN_L;} else if((terreno[3] == caracter)){  accion = actTURN_SR;} else{ if(!Son_muros(terreno)){  accion = actWALK;}else{ accion = actTURN_SR;}}break;
			}
		
    	}

    return accion;
}

bool Son_muros(const vector<unsigned char> & terreno){
	bool variable = false;
	if ((terreno[1] == 'M' and terreno[2]== 'M') or (terreno[1] == 'M' and terreno[2] == 'M' and terreno[3]== 'M') or (terreno[2] == 'M' and terreno[3]== 'M') or ( terreno[1] == 'M' and terreno[3]== 'M')) { variable = true;}
	return variable;
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