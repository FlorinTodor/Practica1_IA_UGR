#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <iostream>
using namespace std;

struct state{
int fil;
int col;
Orientacion brujula = norte;
};



class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      current_state.fil=99;
      current_state.col=99;
      current_state.brujula = norte;
      last_action= actIDLE;
      girar_derecha = false;
      bien_situado = false;
      zapatillas=false;
      bikini=false;
      num_giros=0;
      posicionamiento_encontrado=false;
      bateria_baja = false;
      recarga_encontrada= false;
      reseteado = false;
      colision_lobo = false;
      colision_aldeano = false;
      casilla_desconocida = false;
      colision_muro=false;
      son_muros = true;
      hubo_colision=false;
      girar_izquierda_derecha=false; 
      he_girado_izquierda=false;

      
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);



  private:
  // Declarar aquí las variables de estado
  state current_state; //estado actual
  Action last_action; //Ultima acción realizada
  bool girar_derecha;
  bool he_girado_izquierda;
  bool girar_izquierda_derecha;
  bool bien_situado;
  bool zapatillas;
  bool bikini;
  int num_giros;
  bool posicionamiento_encontrado; //Para comprobar si hemos encotrado una casilla de posicionamiento
  bool bateria_baja;
  bool recarga_encontrada;
  bool reseteado;
  bool colision_lobo;
  bool colision_aldeano;
  bool casilla_desconocida;
  bool colision_muro;
  bool son_muros;


  bool hubo_colision;

};

#endif