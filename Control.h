#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "main.h"

class CControl
{
	public:
		CControl();
		~CControl();
		void inicializaControl();
		void actualizaControl();
		void controlTeclado();
		void controlJoystick();
		int obtieneEstadoTecla(int codigo);

	private:
		int tipoControl; //1: teclado, 2: joystick
		//0: up, 1: down, 2: left, 3: right, 4: z, 5: x, 6: enter,
		//variables del proyecto
		//7: camera.x-, 8: camera.x+, 9: camera.z+, 10: camera.z-, 
		//int estadoTecla[7]; //0: si no esta presionada, 1: si est� presionada
		//int estadoAntTecla[7];

		int estadoTecla[10]; //0: si no esta presionada, 1: si est� presionada
		int estadoAntTecla[10];
};

#endif