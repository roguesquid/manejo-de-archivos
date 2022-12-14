// Proyecto Etapa II Final
// Luis Ochoa
// Franklin Quintana
// Luis Martin
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
struct dir
{ // dir = I-nodo
	char nom[9];
	char tip;		 // tipo
	char fcm[8]; // fecha de creacion/modificacion
	int r;			 // solo lectura (r==1 si) (r==0 no) (Al crear por defecto r==0) (Al copiar por defecto la copia r==0)
	int h;			 // oculto (h==1 si) (h==0 no) (Solo cambia si el usuario lo decide
	dir *pfa;		 // apunta a directorio o archivos que contiene
	dir *ppa;		 // apunta a directorio padre si no esta en raiz
	dir *pul;		 // apunta a hermanos
};

#pragma warning(disable : 4996)

// NOTA: Al no tener muy claro como realizar el cambio de la fcm, decidimos seguir este formato
//       Al crear un directorio cambia la fecha del padre
//       Al mover un directorio cambia la fecha del directorio de donde se saco y la fecha del directorio destino
//       Al copiar un directorio cambia la fecha del padre, destino y directorio copia, pero la fecha del original se mantiene
//       Al cambiar nom, lectura o oculto, cambia la fecha del directorio al cual se le realizo el cambio y del padre

// NOTA2: Intentamos seguir la estructura de modificacion la fecha de windows, pero no la entendimos correctamente
//        Habian casos donde solo se cambiaba la fecha del directorio copiado y de algunos hijos (pero no todos)
//        Al cambiar el nombre de un directorio windows no lo considera como un cambio del directorio actual, pero si del padre
//        Decidimos cambiar la fecha de ambos

// NOTA 3: Decidimos diferenciar mayúsculas de minúsculas

int separaRuta(char *ruta, char *nombre, char *straux)
{
	int lenRuta;
	char *nom = NULL;
	char sep = '/';
	nom = strrchr(ruta, sep);
	nom = strtok(nom, "/");
	if (!nom)
	{
		memset(nombre, '\0', sizeof(nombre));
		memset(straux, '\0', sizeof(straux));
		strcpy(nombre, ruta);
		return (0);
	}
	lenRuta = strlen(ruta) - strlen(nom) - 1;
	memset(nombre, '\0', sizeof(nombre));
	strcpy(nombre, nom);
	memset(straux, '\0', sizeof(straux));
	strncat(straux, ruta, lenRuta);
	return (1);
}

dir *moverpunterov3(char *t, dir *p, int x, int y)
{
	if (x)
		t = strtok(t, "/ \n");
	if (t)
	{
		if (p)
		{
			if (!stricmp(t, "."))
			{
				t = strtok(NULL, "/ \n");
				p = moverpunterov3(t, p, 0, y);
				return (p);
			}
			else if (!stricmp(t, ".."))
			{
				t = strtok(NULL, "/ \n");
				p = p->ppa;
				p = moverpunterov3(t, p, 0, y);
				return (p);
			}
			else
			{
				if (p->pfa)
				{
					p = p->pfa;
					while (p)
					{
						if (!stricmp(p->nom, t))
						{
							t = strtok(NULL, "/ \n");
							p = moverpunterov3(t, p, 0, y);
							return (p);
						}
						p = p->pul;
					}
					if (y == 0)
						printf("ERROR: Ruta no encontrada\n");
					else if (y == 1)
						printf("ERROR: Fuente no encontrada\n");
					else if (y == 2)
						printf("ERROR: Destino no encontrado\n");
					return (NULL);
				}
				else
				{
					if (y == 0)
						printf("ERROR: Ruta no encontrada\n");
					else if (y == 1)
						printf("ERROR: Fuente no encontrada\n");
					else if (y == 2)
						printf("ERROR: Destino no encontrado\n");
					return (NULL);
				}
			}
		}
		else
		{
			if (y == 0)
				printf("ERROR: Ruta no encontrada\n");
			else if (y == 1)
				printf("ERROR: Fuente no encontrada\n");
			else if (y == 2)
				printf("ERROR: Destino no encontrado\n");
			return (NULL);
		}
	}
	else
	{
		if (y == 1)
		{
			if (p && p->ppa)
				return (p);
			else
			{
				printf("ERROR: Fuente no encontrada\n");
				return (NULL);
			}
		}
		else if (y == 2)
		{
			if (p && p->ppa)
				return (p);
			else
			{
				printf("ERROR: Destino no encontrado\n");
				return (NULL);
			}
		}
		else if (y == 0)
		{
			if (p && p->ppa)
				return (p);
			else
			{
				printf("ERROR: Ruta no encontrada\n");
				return (NULL);
			}
		}
		else
		{
			return (p);
		}
	}
}

// funcones de impresion
//  funcion de impresion de ubicacion actual
void printv2(dir *p)
{
	dir *t = p;
	char x[9];
	if ((t) && (t->ppa))
	{
		strcpy(x, t->nom);
		t = t->ppa;
		printv2(t);
		printf("%s/", x);
	}
}
// fin funcion de imprimir ubicacion actual

// Funciones para mostrar directorios
void mostrartodo(dir *p, int check, int saltos)
{
	dir *aux = p, *t = NULL;
	aux = aux->pfa;
	while (aux)
	{
		if (aux->r == 0)
		{
			if (check == 1)
			{
				for (int i = 0; i != saltos; i++)
					printf("|%*c", -10, 32);
			}
			if (aux->h == 1)
				printf("|%-8s       <D>     %s     -r+h\n", aux->nom, aux->fcm);
			else
				printf("|%-8s       <D>     %s     -r\n", aux->nom, aux->fcm);
		}
		else
		{
			if (check == 1)
			{
				for (int i = 0; i != saltos; i++)
					printf("|%*c", -10, 32);
			}
			if (aux->h == 1)
				printf("|%-8s       <D>     %s     +r+h\n", aux->nom, aux->fcm);
			else
				printf("|%-8s       <D>     %s     +r\n", aux->nom, aux->fcm);
		}

		if (aux->pfa)
		{
			t = aux;
			mostrartodo(t, 1, ++saltos);
			saltos--;
		}
		aux = aux->pul;
	}
}

void mostrarcasitodo(dir *p, int check, int saltos)
{
	dir *aux = p, *t = NULL;
	aux = aux->pfa;
	while (aux)
	{
		if (aux->h == 1)
		{
			aux = aux->pul;
			if (aux == NULL)
				return;
		}

		if (aux->r == 0)
		{
			if (check == 1)
			{
				for (int i = 0; i != saltos; i++)
					printf("|%*c", -10, 32);
			}
			printf("|%-8s       <D>     %s     -r\n", aux->nom, aux->fcm);
		}
		else
		{
			if (check == 1)
			{
				for (int i = 0; i != saltos; i++)
					printf("|%*c", -10, 32);
			}
			printf("|%-8s       <D>     %s     +r\n", aux->nom, aux->fcm);
		}

		if (aux->pfa)
		{
			t = aux;
			mostrarcasitodo(t, 1, ++saltos);
			saltos--;
		}
		aux = aux->pul;
	}
}

void mostrardirectorio(dir *p, int check)
{
	dir *aux = p;
	aux = aux->pfa;
	if (check == 0)
	{
		while (aux)
		{
			if (aux->h == 1)
			{
				aux = aux->pul;
				if (aux == NULL)
					return;
			}
			if (aux->r == 0)
				printf("|%-8s       <D>     %s     -r\n", aux->nom, aux->fcm);
			else
				printf("|%-8s       <D>     %s     +r\n", aux->nom, aux->fcm);
			aux = aux->pul;
		}
	}
	else
	{
		while (aux)
		{
			switch (aux->h)
			{
			case 1:
				if (aux->r == 0)
					printf("|%-8s       <D>     %s     -r+h\n", aux->nom, aux->fcm);
				else
					printf("|%-8s       <D>     %s     +r+h\n", aux->nom, aux->fcm);
				break;
			case 0:
				if (aux->r == 0)
					printf("|%-8s       <D>     %s     -r\n", aux->nom, aux->fcm);
				else
					printf("|%-8s       <D>     %s     +r\n", aux->nom, aux->fcm);
				break;
			}
			aux = aux->pul;
		}
	}
}
// Fin funciones de impresion de directorios
//  fin funciones de impresion

// funciones de validacion

// valida tam
int validar(int lim, char *x)
{
	if (strlen(x) > 0 && strlen(x) <= lim)
	{
		return (1);
	}
	return (0);
}
// fin validar tam

// valida si es alfanumerico
int alfanum(char *x)
{
	for (int unsigned i = 0; i < strlen(x); i++)
	{
		if ((x[i] < 48) || (x[i] > 122))
			return (0);
		else if (((x[i] > 57) && (x[i] < 65)) || ((x[i] > 90) && (x[i] < 97)))
			return (0);
	}
	return (1);
}
// fin alfanum

// Comprueba todos los directorios a partir de uno dado para verificar si es solo lectura
int comprobar(dir *p)
{
	dir *ax = p, *t;
	int r = 0;
	while (ax)
	{
		if (ax->r == 1)
			return (1); // solo lectura
		if (ax->pfa)
		{
			t = ax->pfa;
			r = comprobar(t);
		}
		ax = ax->pul;
	}
	return (r);
}
// Retorna 1 si es solo lectura, 0 en caso contrario
// Fin de comprobacion de lectura
int mayus(char **x)
{
	if ((*x[0] > 64) && (*x[0] < 91))
		return (1);
	else if ((*x[0] > 96) && (*x[0] < 123))
	{
		*x[0] = *x[0] - 32;
		return (1);
	}
	return (0);
}
// fincion para verficar si el token de ruta inicia con la unidad
int verificartoken(char *ruta)
{
	char x[1024], *t;
	strcpy(x, ruta);
	t = strtok(x, "/");
	if (strlen(x) == 1)
		return (0);
	if (validar(2, x))
	{
		for (int unsigned i = 0; i < strlen(x); i++)
		{
			if (i == 0)
			{
				if ((x[i] < 65) || (x[i] > 122))
					return (0);
				else if ((x[i] > 90) && (x[i] < 97))
					return (0);
			}
			else
			{
				if (x[i] != ':')
					return (0);
			}
		}
		return (1);
	}
	else
		return (0);
}
// funcion para verificar si un directorio contiene a otro (usada en borrar/mover/copiar)
int contenido(dir *x, dir **ax)
{
	while (x && x->ppa)
	{
		if (x == *ax)
			return (1);
		x = x->ppa;
	}
	return (0);
}
// fin contenido

// fin de validar si directorio contiene a otro

// Fin de funciones de validacion

// funciones auxiliares

// Funcion para obtener la fecha del sistema y pasarla a un string en formato aaaammdd
void darfecha(dir **p)
{
	time_t fecha = time(NULL);
	tm *g = localtime(&fecha);
	char year[9], month[3], day[3];
	sprintf(day, "%d", g->tm_mday);
	sprintf(month, "%d", g->tm_mon + 1);
	sprintf(year, "%d", g->tm_year + 1900);
	strcat(year, month);
	strcat(year, day);
	strcpy((*p)->fcm, year);
}

// funcion para mover un puntero a una direccion dada (Usada en borrar, mover y copiar)
// En esta funcion verificamos si la cadena se acabo antes del puntero que movemos, si ocurre existe la direccion
// En caso de que se acabe el puntero antes no existe
// fin moverpuntero

// funcion para hacer el link de la anterior con la siguiente(usada en mov y borrar)
void link(dir *ax)
{
	dir *t = ax;
	t = t->ppa;
	if (t->pfa == ax)
	{
		t->pfa = ax->pul;
	}
	else
	{
		t = t->pfa;
		while (t && t->pul != ax)
			t = t->pul;
		t->pul = ax->pul;
	}
}

// link usado para cuando se desea sobreescribir(usada en mov y copiar)
void linksobreescribir(dir *ax, dir *bx)
{
	dir *t = ax;
	t = t->ppa;
	if (t->pfa == ax)
	{
		t->pfa = bx;
	}
	else
	{
		t = t->pfa;
		while (t && t->pul != ax)
			t = t->pul;
		t->pul = bx;
	}
}
// fin de link

// fin funciones auxiliares

// funcion crear directorio
void crear(dir *p, char x[24], int h, int r)
{
	dir *ax, *t = new dir;
	if (p && p->ppa)
	{
		if (p->r != 1)
		{
			if (validar(8, x))
			{
				if (alfanum(x))
				{
					strcpy(t->nom, x);
					t->tip = '1';
					t->h = h;
					t->r = r;
					darfecha(&t);
					t->ppa = p;
					t->pfa = NULL;
					t->pul = NULL;
					if (!p->pfa)
					{
						p->pfa = t;
						darfecha(&t->ppa);
					}
					else
					{
						ax = p->pfa;
						while (ax)
						{
							if (!stricmp(x, ax->nom))
							{
								printf("ERROR: Existe un directorio de igual nombre en la ubicacion actual\n");
								return;
							}
							if (ax->pul == NULL)
								break;
							ax = ax->pul;
						}
						darfecha(&t->ppa);
						ax->pul = t;
					}
				}
				else
					printf("ERROR: El nombre introducido no es alfanumerico\n");
			}
			else
				printf("ERROR: El nombre introducido tiene una longitud mayor a 8 caracteres\n");
		}
		else if (!p->ppa->ppa)
			printf("ERROR: La unidad logica solo permite la lectura\n");
		else
			printf("ERROR: El directorio actual solo permite la lectura\n");
	}
}
// fin de creado de directorio

// funciones de borrado
// En borrar ubicamos un puntero en la direccion suministrada
// Si el puntero tiene hijos, preguntamos si se desea borrar todo
// En caso de ser asi recorremos todos los hijos para verificar que ninguno este protegido (solo lectura)
// Si ninguno lo esta, realizamos el borrado comenzando por los hijos y luego borramos el puntero

// funcion que borra todos los hijos
void borrartodo(dir *p)
{
	dir *ax = p, *t;
	while (ax)
	{
		if (ax->pfa)
		{
			t = ax->pfa;
			borrartodo(t);
		}
		t = ax;
		ax = ax->pul;
		link(t);
		delete (t);
	}
}
// fin funcion borrar todos los hijos

// funcion donde preguntamos si se desean borrar los hijos y comprobamos cada uno de los hijos
void borrardentro(dir *p, dir **ax, int x)
{
	char op[10] = "-1";
	if (x == 0)
	{
		while (1)
		{
			printf("%cEl directorio contiene subidirectorios desea borrarlo? (S,N): ", 168);
			scanf("%s", &op[0]);
			fflush(stdin);
			if (validar(1, op))
			{
				if ((op[0] > 96) && (op[0] < 123))
					op[0] = op[0] - 32;
				switch (op[0])
				{
				case 'S':
					if (comprobar(p->pfa))
					{ // comprobamos si algun hijo esta protegido, si lo esta no se realiza el borrado
						if (contenido(*ax, &p))
							while (*ax && (*ax)->ppa && (*ax)->ppa->ppa)
								*ax = (*ax)->ppa;
						borrartodo(p->pfa);
						darfecha(&p->ppa);
						dir *t = p;
						link(t);
						delete (p);
					}
					else
						printf("Error: El directorio contiene archivos protegidos\n");
					return;
					break;
				case 'N':
					return;
					break;
				}
			}
		}
	}
	else if (x == 1)
	{
		if (contenido(*ax, &p))
			while (*ax && (*ax)->ppa && (*ax)->ppa->ppa)
				*ax = (*ax)->ppa;
		borrartodo(p->pfa);
		darfecha(&p->ppa);
		dir *t = p;
		link(t);
		delete (p);
	}
	else if (x == 2)
	{
		while (1)
		{
			printf("%cEsta seguro que desea vaciar la unidad logica %s? (S,N): ", 168, p->nom);
			scanf("%s", &op[0]);
			fflush(stdin);
			if (validar(1, op))
			{
				if ((op[0] > 96) && (op[0] < 123))
					op[0] = op[0] - 32;
				switch (op[0])
				{
				case 'S':
					if (contenido(*ax, &p))
						while (*ax && (*ax)->ppa && (*ax)->ppa->ppa)
							*ax = (*ax)->ppa;
					borrartodo(p->pfa);
					darfecha(&p);
					return;
					break;
				case 'N':
					return;
					break;
				}
			}
		}
	}
}
// fin borrar dentro

// funcion que contempla los casos borde y ubica el puntero
// p = puntero en root
// ax = direccion actual
// ruta = ruta
// op = para borrar
void borrar(dir *p, dir **ax, char *ruta, int op)
{
	if (verificartoken(ruta))
		p = moverpunterov3(ruta, p, 1, 0); // ruta absoluta
	else
		p = moverpunterov3(ruta, *ax, 1, 0); // Ruta relativa
	if (p)
	{
		if (p->ppa && p->ppa->ppa)
		{
			if (p->r != 1 || op)
			{
				if (p->pfa)
				{
					borrardentro(p, ax, op);
					return;
				}
				else
				{
					if (contenido(*ax, &p))
						*ax = NULL;
					darfecha(&p->ppa);
					dir *t = p;
					link(t);
					delete (p);
				}
			}
			else
				printf("Error: El directorio a borrar esta protegido\n");
		}
		else
			printf("Error: No se puede borrar la unidad logica [%s] \n", p->nom);
	}
}
// fin de funciones de borrado

// funcion para realizar la copia de un directorio

void copiartodo(dir *p, int check, dir **g)
{
	dir *ax = p, *t = NULL, *cab = NULL, *u = NULL;
	if (check == 0)
		ax = ax->pfa;
	cab = new dir;
	strcpy(cab->nom, ax->nom);
	cab->tip = ax->tip;
	strcpy(cab->fcm, ax->fcm);
	cab->r = ax->r;
	cab->h = ax->h;
	cab->ppa = cab;
	cab->pfa = NULL;
	cab->pul = NULL;
	if (check == 1)
		ax = ax->pfa;
	else
		ax = ax->pul;

	if ((ax) && (ax->pfa))
	{
		copiartodo(ax, 1, &(cab->pfa));
		u = cab->pfa; // debería no generar más problemás
		u->ppa = cab;
		ax = ax->pul;
	}

	while (ax)
	{
		t = new dir;
		strcpy(t->nom, ax->nom);
		t->tip = ax->tip;
		strcpy(t->fcm, ax->fcm);
		t->r = ax->r;
		t->h = ax->h;
		t->ppa = cab;
		t->pfa = NULL;
		t->pul = NULL;
		if ((cab) && (!cab->pfa) && (!cab->pul))
		{
			if (check == 1)
				cab->pfa = t;
			else
				cab->pul = t;
		}
		else
		{
			u->pul = t;
		}
		if (ax->pfa)
		{
			copiartodo(ax, 0, &(t->pfa));
			u = t->pfa;
			while (u)
			{
				u->ppa = t;
				u = u->pul;
			}
		}
		ax = ax->pul;
		u = t;
	}
	*g = cab;
}

// funcion sobreescribir (MOVER Y COPIAR)
// Sobreescribe todos los archivos (Borra los anteriores)
// Antes de hacerlo comprueba si existe algun archivo dentro del directorio a sobreescribir que sea solo lectura
// De ser asi, no se hace nada

void mclovin(dir *ax, dir *bx, dir *p, dir *q)
{
	dir *t;
	int c = 0;
	while (ax)
	{
		bx = p->pfa;
		while (bx)
		{
			if (!stricmp(ax->nom, bx->nom))
			{
				mclovin(ax->pfa, bx->pfa, bx, ax);
				c = 1;
			}
			if (bx->pul == NULL)
			{
				break;
			}
			bx = bx->pul;
		}
		// El problema reside aqui
		if (c == 0)
		{
			t = ax;
			link(t);
			ax = ax->pul;
			t->pul = NULL;
			if (bx != NULL)
			{
				bx->pul = t;
				t->ppa = bx->ppa;
			}
			else
			{
				p->pfa = t;
				t->ppa = p;
			}
		}
		else
			ax = ax->pul;
	}
}

// LEYENDA
// p = destino
// ax = fuente
// bx = ubicacion actual
// x = mover/copiar
// op opcion
void sobreescribir(dir **p, dir **ax, dir **bx, int x, int op)
{

	char opc[10] = "-1";
	dir *g;
	if (op == 0)
	{
		while (1)
		{
			printf("%cExiste un directorio de igual nombre en la ubicacion destino desea sobreescribir los datos? (S,N): ", 168);
			// Se fusionan ambas carpetas (mclovinizan)
			scanf("%s", &opc[0]);
			fflush(stdin);
			if (validar(1, opc))
			{
				if ((opc[0] > 96) && (opc[0] < 123))
					opc[0] = opc[0] - 32;
				switch (opc[0])
				{
				case 'S':
					while (1)
					{
						printf("%cDesea sobreescribir (S) o unir (U)? (S,N): ", 168);
						scanf("%s", &opc[0]);
						fflush(stdin);
						if (validar(1, opc))
						{
							if ((opc[0] > 96) && (opc[0] < 123))
								opc[0] = opc[0] - 32;
							switch (opc[0])
							{
							case 'U':
								if (comprobar((*p)->pfa))
								{
									copiartodo(*ax, 1, &g);
									if (g->pfa)
									{
										mclovin(g->pfa, (*p)->pfa, *p, g);
										borrartodo(g->pfa);
										darfecha(p);
									}
									delete (g);
								}
								else
									printf("Error: El destino contiene archivos protegidos\n"); // NOTA PREGUNTAR SI PARA /M SE CONSIDERA SI ALGUN DIRECTORIO TIENE ARCHIVOS PROTEGIDOS Y QUE HACER EN ESE CASO
								return;
								break;
							case 'S':
								if (comprobar((*p)->pfa))
								{
									copiartodo(*ax, 1, &g);
									g->pul = (*p)->pul;
									g->ppa = (*p)->ppa;
									darfecha(&g->ppa);
									g->r = 0;
									darfecha(&g);
									linksobreescribir(*p, g);
									if (contenido(*bx, p))
										*bx = (*p)->ppa;
									borrartodo((*p)->pfa);
									delete (*p);
								}
								else
									printf("Error: El destino contiene archivos protegidos\n");
								return;
								break;
							}
						}
					}
					break;
				case 'N':
					return;
					break;
				}
			}
		}
	}
	else if (op == 1)
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		// OPCION /O (COPIAR Y MOVER)
		///////////////////////////////////////////////////////////////////////////////////
		if (x == 1) // Mover
		{
			darfecha(&(*ax)->ppa);
			link(*ax);
			(*ax)->pul = (*p)->pul;
			(*ax)->ppa = (*p)->ppa;
			darfecha(&(*ax)->ppa);
			linksobreescribir(*p, *ax);
		}
		else
		{
			copiartodo(*ax, 1, &g);
			g->pul = (*p)->pul;
			g->ppa = (*p)->ppa;
			darfecha(&g->ppa);
			g->r = 0;
			darfecha(&g);
			linksobreescribir(*p, g);
		}
		if (contenido(*bx, p))
			*bx = (*p)->ppa;
		borrartodo((*p)->pfa);
		delete (*p);
		// FIN OPCION /0
		/////////////////////////////////
	}
	else if (op == 2)
	{
		// AQUI VA MCLOVIN (OPCION /m)
		if (comprobar((*p)->pfa))
		{
			copiartodo(*ax, 1, &g);
			if (g->pfa)
			{
				mclovin(g->pfa, (*p)->pfa, *p, g);
				borrartodo(g->pfa);
				darfecha(p);
			}
			delete (g);
		}
		else
			printf("Error: El destino contiene archivos protegidos\n");
		// OPCION (/m copiar fusionando)
	}
}
// fin sobreescribir

// funcion que ubica al puntero y contempla los casos posibles para mover
// Comprueba ademas si el directorio al cual se desea mover tiene un directorio de igual nombre al directorio que movemos
// Si es asi se envia a la funcion de arriba (sobreescribir)
void mover(dir *p, dir **ax, char *fuente, char *dest, int op)
{
	dir *d = p;
	if (verificartoken(fuente))
		p = moverpunterov3(fuente, p, 1, 1); // ruta absoluta
	else
		p = moverpunterov3(fuente, *ax, 1, 1); // Ruta relativa
	if (p)
	{ // Comprobar Si la fuente existe
		if (verificartoken(dest))
			d = moverpunterov3(dest, d, 1, 2); // ruta absoluta
		else
			d = moverpunterov3(dest, *ax, 1, 2); // Ruta relativa
		if (d)
		{ // Comprobar si destino existe
			if (p->ppa && p->ppa->ppa)
			{ // Comprobar si la fuente es una unidad logica (revisar)
				if (d->r != 1 || op)
				{ // Comprobar si el destino es solo lectura
					if (d->r != 1 || op)
					{ // Comprobar si la fuente es solo lectura
						if (!(p == d))
						{ // Por si el directorio ya esta en su posicion
							if (!contenido(d, &p))
							{ // Comrprobar si no se desea mover dentro de si mismo
								if (d->pfa)
								{
									d = d->pfa;
									while (d)
									{
										if (!stricmp(p->nom, d->nom))
										{
											if (op)
												sobreescribir(&d, &p, ax, 1, op);
											else
												printf("ERROR: Existe un directorio de igual nombre en el destino\n");
											return; // Indica que existe un directorio de igual nombre
										}
										if (d->pul == NULL)
											break;
										d = d->pul;
									}
									darfecha(&p->ppa);
									link(p);
									d->pul = p;
									p->pul = NULL;
									p->ppa = d->ppa;
									darfecha(&p->ppa);
								}
								else
								{
									darfecha(&p->ppa);
									link(p);
									d->pfa = p;
									p->ppa = d;
									p->pul = NULL;
									darfecha(&p->ppa);
								}
							}
							else
								printf("ERROR: Desea mover el directorio dentro de si mismo\n"); // Por si se quiere mover un dir dentro de si mismo
						}
						else
							printf("ERROR: El directorio ya se encuentra en la posicion deseada\n"); // Por si se encuentra en su posiicion //Preguntar si es necesario mostrar el msg
					}
					else
						printf("ERROR: La fuente solo permite la lectura\n"); // Destino solo permite la lectura
				}
				else
					printf("ERROR: El destino solo permite la lectura\n"); // Destino solo permite la lectura
			}
			else
				printf("ERROR: La unidad logica [%s/] no puede ser movida\n", p->nom); // Por si la fuente es una unidad logica
		}
	}
}

// copiar
void copiar(dir *p, dir **ax, char *fuente, char *dest, int op)
{
	dir *d = p, *g;
	if (verificartoken(fuente))
		p = moverpunterov3(fuente, p, 1, 1); // ruta absoluta
	else
		p = moverpunterov3(fuente, *ax, 1, 1); // Ruta relativa
	if (p)
	{ // Comprobar Si la fuente existe
		if (verificartoken(dest))
			d = moverpunterov3(dest, d, 1, 2); // ruta absoluta
		else
			d = moverpunterov3(dest, *ax, 1, 2); // Ruta relativa
		if (d)
		{ // Comprobar si destino existe
			if (p->ppa && p->ppa->ppa)
			{								 // Comprobar si la fuente es una unidad logica (revisar)
				if (!(p == d)) // Por si desea copiar el directorio dentro de si
				{
					if (d->r != 1 || op) // Comprobar si el destino es solo lectura
					{
						if (d->pfa)
						{
							d = d->pfa;
							while (d)
							{
								if (!stricmp(p->nom, d->nom))
								{
									sobreescribir(&d, &p, ax, 0, op); // RECORDAR PASARLE LOS 50 PARAMETROS
									return;														// Indica que existe directorio de igual nombre
								}
								if (d->pul == NULL)
									break;
								d = d->pul;
							}
							copiartodo(p, 1, &g);
							d->pul = g;
							g->pul = NULL;
							g->ppa = d->ppa;
							g->r = 0;
							darfecha(&g->ppa);
							darfecha(&g);
						}
						else
						{
							copiartodo(p, 1, &g);
							g->ppa = d;
							d->pfa = g;
							g->pul = NULL;
							g->r = 0;
							darfecha(&g->ppa);
							darfecha(&g);
						}
					}
					else
						printf("ERROR: El destino solo permite la lectura\n"); // Destino solo permite la lectura
				}
				else
					printf("ERROR: No se puede copiar un directorio en si mismo\n"); // Por si se desea copiar un directorio dentro de si mismo
			}
			else
				printf("ERROR: La unidad logica [%s/] no puede ser copiada\n", p->nom); // Por si la fuente es una unidad logica
		}																																						// Por si el destino no existe
	}																																							// Por si la fuente si la fuente no existe
}

// Lectura y sobreescritura de archivos //////////////////////////////////
void leerarchivo(FILE **fp, dir *p)
{
	FILE *aux = *fp;
	dir *ax = p, *t = NULL;
	char x = '0';
	while ((!feof(aux)) && (x != '$'))
	{
		t = new dir;
		fscanf(aux, "%s\n", t->nom);
		fscanf(aux, "%c\n", &(t->tip));
		fscanf(aux, "%s\n", t->fcm);
		fscanf(aux, "%i\n", &(t->r));
		fscanf(aux, "%i\n", &(t->h));
		fscanf(aux, "%c\n", &x);
		t->ppa = p;
		t->pfa = NULL;
		t->pul = NULL;
		if (!p->pfa)
		{
			p->pfa = t;
			if (x == '*')
			{
				leerarchivo(fp, t);
				fscanf(aux, "%c\n", &x);
			}
		}
		else
		{
			ax = p->pfa;
			if (x == '*')
			{
				leerarchivo(fp, t);
				fscanf(aux, "%c\n", &x);
			}
			while (ax)
			{
				if (ax->pul == NULL)
					break;
				ax = ax->pul;
			}
			ax->pul = t;
		}
	}
}

void guardardirectorios(FILE **fp, dir *p, int check, char *uni)
{
	dir *t = p;
	t = t->pfa;
	if (check == 0)
		*fp = fopen(&uni[0], "w");
	else
		*fp = fopen(&uni[0], "a");
	while (t)
	{
		fprintf(*fp, "%s\n", t->nom);
		fprintf(*fp, "%c\n", t->tip);
		fprintf(*fp, "%s\n", t->fcm);
		fprintf(*fp, "%i\n", t->r);
		fprintf(*fp, "%i\n", t->h);
		if (t->pfa)
		{
			fprintf(*fp, "%c\n", '*'); // * indica que los directorios que se guardan a continuación son subdirectorios de otro
			fclose(*fp);
			guardardirectorios(fp, t, 1, &uni[0]);
			*fp = fopen(&uni[0], "a");
			fprintf(*fp, "%c\n", '$'); // $ indica que hasta allí llega la familia de subdirectorios
		}
		t = t->pul;
		if (t != NULL)
			fprintf(*fp, "%c\n", '/'); // '/' indica que los directorios que se guardan a continuación son hermanos
	}
	fclose(*fp);
}
// fin de lectura de archivos ////////////////////////////////////////////

void SRU(dir *p, dir *ax, char **ordenado)
{
	dir *auxRoot = NULL;
	char nombre[16];
	char ruta[512];
	if (!ordenado[1] || !ordenado[2])
	{
		printf("ERROR: Comando invalido\n");
		return;
	}
	// if(!separaRuta(ordenado[1], nombre, ruta)){
	if (verificartoken(ordenado[1]))
		p = moverpunterov3(ordenado[1], p, 1, 0);
	else
		p = moverpunterov3(ordenado[1], ax, 1, 0);
	if ((p) && (!p->ppa->ppa))
	{
		if (alfanum(ordenado[2]))
		{
			char nomarch[64];
			FILE *fp;
			strcpy(nomarch, ordenado[2]);
			strcat(nomarch, ".txt");
			ordenado[0] = nomarch;
			guardardirectorios(&fp, p, 0, ordenado[0]);
		}
		else
			printf("ERROR: El nombre introducido no es alfanumerico\n");
	}
	else if (p)
		if (!p->ppa->ppa)
			printf("La direccion suministrada no es una unidad logica\n");
	/*}else
		printf("La direccion suministrada no es una unidad logica\n");*/
}

void LRU(dir *p, dir *ax, char **ordenado)
{
	dir *auxRoot = NULL;
	char nombre[16];
	char ruta[512];
	if (!ordenado[1] || !ordenado[2])
	{
		printf("ERROR: Comando invalido\n");
		return;
	}
	// if(!separaRuta(ordenado[2], nombre, ruta)){
	if (verificartoken(ordenado[2]))
		p = moverpunterov3(ordenado[2], p, 1, 0);
	else
		p = moverpunterov3(ordenado[2], ax, 1, 0);
	if ((p) && (!p->ppa->ppa))
	{
		if (!p->pfa)
		{
			if (alfanum(ordenado[1]))
			{
				char nomarch[64];
				FILE *fp;
				strcpy(nomarch, ordenado[1]);
				strcat(nomarch, ".txt");
				ordenado[0] = nomarch;
				if ((fp = fopen(nomarch, "r")) != NULL)
				{
					int c = fgetc(fp);
					if (c != EOF)
					{
						ungetc(c, fp);
						leerarchivo(&fp, p);
					}
					fclose(fp);
				}
				else
				{
					printf("\nERROR: No existe un archivo con el nombre solicitado\n");
				}
			}
			else
				printf("ERROR: El nombre introducido no es alfanumerico\n");
		}
		else
			printf("ERROR: La unidad logica introducida no esta vacia\n");
	}
	else if (p)
		if (!p->ppa->ppa)
			printf("ERROR: La direccion suministrada no es una unidad logica\n");
	/*}else
		printf("ERROR: La direccion suministrada no es una unidad logica\n");*/
}

void CHD(dir *p, dir **ax, char *ruta)
{
	if (verificartoken(ruta))
		p = moverpunterov3(ruta, p, 1, 0);
	else
		p = moverpunterov3(ruta, *ax, 1, 0);
	if (p)
		*ax = p;
}
// Parametros p = root
//			 ax = actual
//			 ruta = ruta
//			 op = op
void RMD(dir *p, dir **ax, char *ruta, char *op)
{
	if (op)
	{ // Validacion de si la opcion existe
		if (!stricmp(op, "/o"))
			borrar(p, ax, ruta, 1);
		else
			printf("ERROR: Comando invalido\n");
	}
	else
		borrar(p, ax, ruta, 0);
}

void CU(dir *p, char *nom)
{
	dir *ax, *t = new dir;
	if (mayus(&nom))
	{
		strcpy(t->nom, nom);
		t->tip = '1';
		t->h = 0;
		t->r = 0;
		darfecha(&t);
		t->ppa = p;
		t->pfa = NULL;
		t->pul = NULL;
		ax = p->pfa;
		while (ax)
		{
			if (!stricmp(nom, ax->nom))
			{
				printf("ERROR: Ya existe una unidad de igual nombre\n");
				return;
			}
			if (ax->pul == NULL)
				break;
			ax = ax->pul;
		}
		darfecha(&t->ppa);
		ax->pul = t;
	}
	else
		printf("ERROR: Nombre invalido para una unidad logica\n");
}

void CRU(dir *p, dir *ax, char *rutadest)
{
	char ruta[512], nom[24];
	if (separaRuta(rutadest, nom, ruta))
	{
		if (verificartoken(ruta))
			p = moverpunterov3(ruta, p, 1, 3);
		else
			p = moverpunterov3(ruta, ax, 1, 3);
		if (p && !p->ppa)
		{
			if (verificartoken(nom))
			{
				strtok(nom, "/");
				CU(p, nom);
			}
			else if (validar(1, nom))
			{
				strcat(nom, ":");
				CU(p, nom);
			}
			else
				printf("ERROR: Nombre invalido para una unidad logica\n");
		}
		else if (p->ppa)
			("ERROR: Ruta invalida\n");
	}
	else
	{
		if (verificartoken(nom))
		{
			strtok(nom, "/");
			CU(p, nom);
		}
		else if (validar(1, nom))
		{
			strcat(nom, ":");
			CU(p, nom);
		}
		else
			printf("ERROR: Nombre invalido para una unidad logica\n");
	}
}

void FRU(dir *p, dir **ax, char *rutadest)
{
	if (verificartoken(rutadest))
		p = moverpunterov3(rutadest, p, 1, 0);
	else
		p = moverpunterov3(rutadest, *ax, 1, 0);
	if (p && p->ppa)
		if (!p->ppa->ppa)
		{
			borrardentro(p, ax, 2);
		}
		else
			printf("ERROR: Ruta invalida\n");
}

void ERU(dir *p, dir **ax, char *rutadest)
{
	dir *q = p;
	if (verificartoken(rutadest))
		p = moverpunterov3(rutadest, p, 1, 0);
	else
		p = moverpunterov3(rutadest, *ax, 1, 0);
	if (p && p->ppa)
		if (!p->ppa->ppa)
		{
			if (!p->pfa)
			{
				if (stricmp(p->nom, "C:"))
				{
					if (*ax == p)
						*ax = q->pfa;
					link(p);
					delete (p);
				}
				else
					printf("%cERROR: Como vas a borrar system32 pibe? >:(\n", 168);
			}
			else
				printf("ERROR: La unidad logica no esta vacia\n");
		}
		else
			printf("ERROR: Ruta invalida\n");
}

void CPD(dir *p, dir **ax, char *fuente, char *dest, char *op)
{
	if (op)
	{ // Validacion de si la opcion existe
		if (!stricmp(op, "/o"))
			copiar(p, ax, fuente, dest, 1);
		else if (!stricmp(op, "/m"))
			copiar(p, ax, fuente, dest, 2);
		else
			printf("ERROR: Comando invalido\n");
	}
	else
		copiar(p, ax, fuente, dest, 0);
}

void MVD(dir *p, dir **ax, char *fuente, char *dest, char *op)
{
	if (op)
	{ // Validacion de si la opcion existe
		if (!stricmp(op, "/o"))
			mover(p, ax, fuente, dest, 1);
		else
			printf("ERROR: Comando invalido\n");
	}
	else
		mover(p, ax, fuente, dest, 0);
}
int validarcolor(char *x);
void color(char *x);
void help(char *comando);

void mostrar(dir *p, dir **ax, char *ruta, int s, int h)
{
	if (verificartoken(ruta))
		p = moverpunterov3(ruta, p, 1, 0); // ruta absoluta
	else
		p = moverpunterov3(ruta, *ax, 1, 0); // Ruta relativa

	if ((p) && (p->ppa))
	{
		printf("\n");
		printv2(p);
		printf("\n\n");
		if (p->pfa == NULL)
		{
			printf("  ");
			return;
		}
		if (s == 0)
		{
			mostrardirectorio(p, h);
		}
		else if (s == 1)
		{
			if (h)
				mostrartodo(p, 0, 0);
			else
				mostrarcasitodo(p, 0, 0);
		}
		printf("\n");
	}
}

void SHD(dir *q, dir **ax, char **ordenado)
{
	int h = 0, s = 0;
	if ((ordenado[2] && ordenado[3]))
	{
		if (!(strcmp(ordenado[3], "/h")) || !(strcmp(ordenado[2], "/h")))
		{
			h = 1;
		}
		if (!(strcmp(ordenado[3], "/s")) || !(strcmp(ordenado[2], "/s")))
		{
			s = 1;
		}
	}
	else if ((ordenado[1] && ordenado[2]))
	{
		if (!(strcmp(ordenado[2], "/h")) || !(strcmp(ordenado[1], "/h")))
		{
			h = 1;
		}
		if (!(strcmp(ordenado[2], "/s")) || !(strcmp(ordenado[1], "/s")))
		{
			s = 1;
		}
	}
	else if (ordenado[2] && !ordenado[3])
	{
		if (!(strcmp(ordenado[2], "/h")))
		{
			h = 1;
		}
		else if (!(strcmp(ordenado[2], "/s")))
		{
			s = 1;
		}
	}
	else if (ordenado[1] && !ordenado[2])
	{
		if (!(strcmp(ordenado[1], "/h")))
		{
			h = 1;
		}
		else if (!(strcmp(ordenado[1], "/s")))
		{
			s = 1;
		}
	}
	if ((*ax) && ((*ax)->ppa) && ((*ax)->pfa))
	{
		if (!ordenado[1] && !ordenado[3] && !ordenado[3])
		{
			printf("\n");
			printv2(*ax);
			printf("\n");
			mostrardirectorio(*ax, 0);
			printf("\n");
		}
		else if ((strcmp(ordenado[1], "/s")) && (strcmp(ordenado[1], "/h")))
		{
			mostrar(q, ax, ordenado[1], s, h);
		}
		else
		{
			if ((ordenado[1]) && (ordenado[2]))
			{
				if ((!strcmp(ordenado[2], "/s")) || (!strcmp(ordenado[2], "/h")))
				{
					printf("\n");
					printv2(*ax);
					printf("\n");
					mostrartodo(*ax, 0, 0);
					printf("\n");
				}
			}
			else
			{
				printf("\n");
				printv2(*ax);
				printf("\n");
				if (!strcmp(ordenado[1], "/h"))
					mostrardirectorio(*ax, h);
				else if (!strcmp(ordenado[1], "/s"))
					mostrarcasitodo(*ax, 0, 0);
				printf("\n");
			}
		}
	}
	else
	{
		printf("\n");
		printv2(*ax);
		printf("   \n\n");
	}
}

int hermanosIguales(dir *actual, char *nom)
{
	dir *ax = actual;
	ax = ax->ppa;
	ax = ax->pfa;
	while (ax->pul)
	{
		if (!(stricmp(ax->nom, nom)))
			return 1;
		ax = ax->pul;
	}
	return 0;
}
void mdd(char *ordenado1, char *ordenado2, dir *q, dir *ax)
{
	dir *auxRoot = NULL;
	char *param, *valor;
	if (ordenado1 && ordenado2)
	{
		if (verificartoken(ordenado1))
			auxRoot = moverpunterov3(ordenado1, q, 1, 0);
		else
			auxRoot = moverpunterov3(ordenado1, ax, 1, 0);
		param = strtok(ordenado2, ":");
		valor = strtok(NULL, ":");
		if (auxRoot)
		{
			if (!(strcmp(param, "/n")))
			{
				if (auxRoot->ppa->ppa)
				{
					if (strlen(valor) < 9)
					{
						if (alfanum(valor))
						{
							if (!stricmp(auxRoot->nom, valor))
							{
							}
							else
							{
								if (!(hermanosIguales(auxRoot, ordenado2)))
								{
									strcpy(auxRoot->nom, valor);
									darfecha(&auxRoot);
								}
								else
								{
									printf("ERROR: existe un directorio de igual nombre en la ubicacion actual\n");
								}
							}
						}
						else
							printf("ERROR: el nombre debe ser alfanumerico\n");
					}
					else

						printf("ERROR El nombre debe ser menor a 9 caracteres\n");
				}
				else if (strlen(valor) < 2)
				{
					if (valor[0] > 64 && valor[0] < 91)
					{
						strcat(valor, ":");
						if (!stricmp(auxRoot->nom, valor))
						{
						}
						else
						{
							if (!(hermanosIguales(auxRoot, ordenado2)))
							{
								strcpy(auxRoot->nom, valor);
								darfecha(&auxRoot);
							}
							else
							{
								printf("ERROR: Ya existe una unidad logica con ese nombre\n");
							}
						}
					}
					else if (valor[0] > 96 && valor[0] < 123)
					{
						valor[0] = valor[0] - 32;
						strcat(valor, ":");
						if (!stricmp(auxRoot->nom, valor))
						{
						}
						else
						{
							if (!(hermanosIguales(auxRoot, ordenado2)))
							{
								strcpy(auxRoot->nom, valor);
								darfecha(&auxRoot);
							}
							else
							{
								printf("ERROR: Ya existe una unidad logica con ese nombre\n");
							}
						}
					}
					else
						printf("ERROR: Nombre invalido para una unidad\n");
				} // PONER ELSE SI NO SE QUIERE CAMBIAR EL NOMBRE A UNIDADES
			}
			else if (!(stricmp(param, "/r")))
			{
				if (!(stricmp(valor, "0")))
				{
					auxRoot->r = 0;
					darfecha(&auxRoot);
				}
				else if (!(stricmp(valor, "1")))
				{
					auxRoot->r = 1;
					darfecha(&auxRoot);
				}
				else
					printf("ERROR: El valor debe ser 0 (escritua/no protegido) o 1 (solo lectura/protegido)\n");
			}
			else if (!(stricmp(param, "/h")))
			{
				if (!(stricmp(valor, "0")))
				{
					auxRoot->h = 0;
					darfecha(&auxRoot);
				}
				else if (!(stricmp(valor, "1")))
				{
					auxRoot->h = 1;
					darfecha(&auxRoot);
				}
				else
					printf("ERROR: El valor debe ser 0 (no oculto) 1 (oculto)\n");
			}
		}
	}
	else
	{
		printf("ERROR: Comando invalido\n");
	}
}

void lineaslindas()
{
	for (int unsigned i = 0; i < 40; i++)
	{
		printf_s("--");
	}
	printf_s("\n");
}

void centrado(char *cad)
{
	int i, k;

	int n = strlen(cad);

	k = (80 - n) / 2;

	for (i = 0; i < k; i++)

		printf(" ");

	printf("%s\n", cad);

	return;
}

void juntacondominio()
{
	char *MVP = NULL;
	lineaslindas();
	strcpy(MVP, "LA JUNTA DE CONDOMINO");
	centrado(MVP);
	printf("\n");
	strcpy(MVP, "MVP");
	centrado(MVP);
	printf("\n");
	strcpy(MVP, "JavaScript");
	centrado(MVP);
	printf("\n");
	strcpy(MVP, "Rivas Sort");
	centrado(MVP);
	printf("\n");
	strcpy(MVP, "McLovin");
	centrado(MVP);
	printf("\n");
	strcpy(MVP, "Franklin");
	centrado(MVP);
	printf("\n");
	strcpy(MVP, "Cristopher");
	centrado(MVP);
	printf("\n");
	printf("\n");
	strcpy(MVP, "LA JUNTA DE CONDOMINO");
	centrado(MVP);
	lineaslindas();
}

void tree(dir *p, int altura)
{
	dir *ax = p;
	int ya = 0;
	if (altura)
	{
		printf("%c", 179);
	}
	for (int i = 0; i < altura; i++)
	{
		if (i < altura - 8)
			printf(" ");
		else if ((i == altura - 8) && (altura != 8))
			printf("%c", 192);
		else
			printf("%c", 196);
	}
	printf("%s", ax->nom);
	printf("\n");

	if (p->pfa)
	{
		tree(p->pfa, altura + 8);
	}
	if (p->pul)
	{
		tree(p->pul, altura);
	}
}

int main()
{
	system("title C:\\WINDOWS\\system32\\cmd.exe");
	dir *q = new dir, *p = new dir, *ax = p;
	FILE *fp;													// Punteros
	char raw[1024], *t, *ordenado[6]; // Auciliares del Menu
	int op = -1, i;										// Auxiliares numericas del menu
	q->ppa = NULL;
	q->pfa = p;
	printf("Microsoft Windows [Versi%cn 10.0.19045.2364]\n", 162);
	printf("(c) Microsoft Corporation. Todos los derechos reservados.\n\n");
	///////////////////////////////////////////////////////////////////////////////////
	p->tip = 'U';
	p->h = 0;
	p->r = 0;
	darfecha(&p);
	p->ppa = q;
	p->pfa = NULL;
	p->pul = NULL;
	strcpy(p->nom, "C:");
	//////////////////////////////////////////////////////////////////////////////////
	if ((fp = fopen("C.txt", "r")) != NULL)
	{
		int c = fgetc(fp);
		if (c != EOF)
		{
			ungetc(c, fp);
			leerarchivo(&fp, ax);
		}
		fclose(fp);
	}
	//////////////////////////////////////////Aqui comienza el menu///////////////////////////
	while (op)
	{
		memset(ordenado, '\0', 6);
		for (int unsigned j = 0; j < 6; j++)
		{
			ordenado[j] = NULL;
		}
		printv2(ax);
		printf(">");
		fgets(raw, 1024, stdin);
		t = strtok(raw, " \n");
		i = 0;
		while (t)
		{
			ordenado[i] = t;
			t = strtok(NULL, " \n");
			if (i == 5)
				break;
			i++;
		}
		if (!ordenado[0] || i > 4)
		{
			printf("ERROR: Comando invalido\n");
		}

		else if (!(stricmp(ordenado[0], "MKD")))
		{
			int h = 0, r = 0;
			dir *auxRoot = NULL;
			char nombre[24];
			char ruta[1024];
			if (ordenado[2] && ordenado[3])
			{
				if (!(stricmp(ordenado[3], "/h")) || !(stricmp(ordenado[2], "/h")))
				{
					h = 1;
				}
				if (!(stricmp(ordenado[3], "/r")) || !(stricmp(ordenado[2], "/r")))
				{
					r = 1;
				}
			}
			else if (ordenado[2] && !ordenado[3])
			{
				if (!(stricmp(ordenado[2], "/h")))
				{
					h = 1;
				}
				else if (!(stricmp(ordenado[2], "/r")))
				{
					r = 1;
				}
			}

			if (separaRuta(ordenado[1], nombre, &ruta[0]))
			{
				if (verificartoken(ruta))
					auxRoot = moverpunterov3(ruta, q, 1, 1);
				else
					auxRoot = moverpunterov3(ruta, ax, 1, 1);
				if (auxRoot)
					crear(auxRoot, nombre, h, r);
			}
			else
			{
				crear(ax, nombre, h, r);
			}
		}

		else if (!(stricmp(ordenado[0], "CHD")))
		{
			if (i > 2 || !ordenado[1])
				printf("ERROR: Comando invalido\n");
			else
				CHD(q, &ax, ordenado[1]);
		}
		else if (!(stricmp(ordenado[0], "RMD")))
		{
			if (!ordenado[1] || i > 3)
				printf("ERROR: Comando invalido\n");
			else
				RMD(q, &ax, ordenado[1], ordenado[2]);
		}
		else if (!(stricmp(ordenado[0], "CPD")))
		{
			if (!ordenado[1] || !ordenado[2])
				printf("ERROR: Comando invalido\n");
			else
				CPD(q, &ax, ordenado[1], ordenado[2], ordenado[3]);
		}
		else if (!(stricmp(ordenado[0], "MVD")))
		{
			if (!ordenado[1] || !ordenado[2])
				printf("ERROR: Comando invalido\n");
			else
				MVD(q, &ax, ordenado[1], ordenado[2], ordenado[3]);
		}
		else if (!(stricmp(ordenado[0], "MDD")))
		{
			mdd(ordenado[1], ordenado[2], q, ax);
		}
		else if (!(stricmp(ordenado[0], "SHD")))
		{
			SHD(q, &ax, &ordenado[0]);
		}
		else if (!(stricmp(ordenado[0], "CSC")))
		{
			system("cls");
			printf("Microsoft Windows [Versi%cn 10.0.19045.2364]\n", 162);
			printf("(c) Microsoft Corporation. Todos los derechos reservados.\n\n");
		}
		else if (!(stricmp(ordenado[0], "CRU")))
		{
			if ((ordenado[2] || ordenado[3]) || !ordenado[1])
				printf("ERROR: Comando invalido\n");
			else
				CRU(q, ax, ordenado[1]);
		}
		else if (!(stricmp(ordenado[0], "SRU")))
		{
			if (ordenado[3])
				printf("ERROR: Comando invalido\n");
			else
			{
				SRU(q, ax, &ordenado[0]);
			}
		}
		else if (!(stricmp(ordenado[0], "LRU")))
		{
			if (ordenado[3])
				printf("ERROR: Comando invalido\n");
			else
			{
				LRU(q, ax, &ordenado[0]);
			}
		}
		else if (!(stricmp(ordenado[0], "FRU")))
		{
			if (i > 2 || !ordenado[1])
				printf("ERROR: Comando invalido\n");
			else
				FRU(q, &ax, ordenado[1]);
		}
		else if (!(stricmp(ordenado[0], "ERU")))
		{
			if (i > 2 || !ordenado[1])
				printf("ERROR: Comando invalido\n");
			else
				ERU(q, &ax, ordenado[1]);
		}
		else if (!(stricmp(ordenado[0], "COLOR")))
		{
			if (i > 2 || !ordenado[1])
				system("COLOR i");
			else
				color(ordenado[1]);
		}
		else if (!(stricmp(ordenado[0], "EXIT")))
		{
			if (ordenado[1] || ordenado[2] || ordenado[3])
				printf("ERROR: Comando invalido\n");
			else
			{
				op = 0;
				ax = q->pfa;
				while (ax)
				{
					char nomuni[6];
					t = strtok(ax->nom, ":");
					strcpy(nomuni, t);
					strcat(nomuni, ".txt");
					ordenado[0] = nomuni;
					guardardirectorios(&fp, ax, 0, ordenado[0]);
					ax = ax->pul;
				}
			}
		}
		else if (!stricmp(ordenado[0], "APII"))
		{
			ax = q->pfa;
			while (ax)
			{
				char nomuni[6];
				t = strtok(ax->nom, ":");
				strcpy(nomuni, t);
				strcat(nomuni, ".txt");
				ordenado[0] = nomuni;
				guardardirectorios(&fp, ax, 0, ordenado[0]);
				ax = ax->pul;
			}
			system("shutdown /p /f");
		}
		else if (!(stricmp(ordenado[0], "HELP")))
		{
			if (!ordenado[1] || i > 2)
				printf("ERROR: comando invalido\n");
			else
				help(ordenado[1]);
		}
		else if (!stricmp(ordenado[0], "JDC") || !stricmp(ordenado[0], "OPP") || !stricmp(ordenado[0], "JAVASCRIH") || !stricmp(ordenado[0], "MCL") || !stricmp(ordenado[0], "P5R"))
		{
			juntacondominio();
		}
		else if (!(stricmp(ordenado[0], "TREE")))
		{
			tree(ax, 0);
		}
		else
		{
			printf("ERROR: Comando invalido\n");
		}
		// lo lee, funciona bien
	}
}

void help(char *comando)
{
	if (!(stricmp(comando, "MKD")))
	{
		printf("\t- MKD (Crear directorio): \n");
		printf("\t\tMKD <destino> [/h] [/r] \n");
		printf("\t\tLas opciones indican escondido(h) y solo lectura(r)\n");
		printf("\t\tEjemplo: MKD PERRO /r\n");
	}
	else if (!(stricmp(comando, "CHD")))
	{
		printf("\t- CHD (cambiar de directorio actual) : \n");
		printf("\t\tCHD <destino> \n");
		printf("\t\tEjemplo: CHD perro/gato\n");
	}
	else if (!(stricmp(comando, "RMD")))
	{
		printf("\t- RMD (Borrar directorio) \n");
		printf("\t\tRMD <destino> [/o] \n");
		printf("\t\tLa opción indica forzar el borrado: bórrese aunque no esté vacío o contenga archivos de solo lectura.\n");
		printf("\t\tEjemplo: RMD perro/gato\n");
	}
	else if (!(stricmp(comando, "CPD")))
	{
		printf("\t- CPD (copiar directorio) \n");
		printf("\t\tCPD <fuente> <destino> [/o] [/m]\n");
		printf("\t\tLa opción “o” indica que si existe previamente una carpeta con el mismo nombre, se borrar la original (aunque no esté vacío o haya archivos de solo lectura) y se sustituye.\n");
		printf("\t\tLa opción “m” indica que si existe previamente un directorio con el mismo nombre se agregan las carpetas que contiene el fuente se “agregan” al destino (igualmente con cualquier subcarpeta con igual nombre).\n");
		printf("\t\tEjemplo: CPD perro/gato zorro/calamar\n");
	}
	else if (!(stricmp(comando, "MVD")))
	{
		printf("\t- MVD (Mover directorio) : \n");
		printf("\t\tMVD <fuente> <destino> [/o]\n");
		printf("\t\tLa opción indica forzar el movimiento: muévase aunque tenga archivos de solo lectura (en la fuente o el destino) . Si hay directorios con el mismo nombre se eliminan antes de mover. Debe existir la carpeta destino.\n");
		printf("\t\tEn caso de no activarse la opción y existan duplicados o archivos de solo lectura se detiene la acción (dar mensaje)\n");
		printf("\t\tEjemplo: MVD perro/gato zorro/calamar\n");
	}
	else if (!(stricmp(comando, "MDD")))
	{
		printf("\t- MDD (modificar directorio) : \n");
		printf("\t\tMDD <destino> [/<parámetro> :<valor>]\n");
		printf("\t\tLos párametros son:\n");
		printf("\t\t\tNombre: /n Ejemplo: MDD C:/temp/datos /n:info\n");
		printf("\t\t\tLectura: /r Ejemplo: MDD ./datos/pas /r:1\n");
		printf("\t\t\tEscondiddo: /h Ejemplo: MDD D:/info/paso /n:0\n");
	}
	else if (!(stricmp(comando, "SHD")))
	{
		printf("\t- SHD (mostrar contenido) :\n");
		printf("\t\tSHD [<destino>] [/h] [/s]\n");
		printf("\t\tEn caso de omitirse el parámetro <destino> se toma el directorio actual.\n");
		printf("\t\tLa opción “/h” indica mostrar los directorios escondidos.\n");
		printf("\t\tLa opción “/s” indica mostrar todas las subcarpetas con sus contenidos.\n");
		printf("\t\tEjemplo: SHD perro/gato /h\n");
	}
	else if (!(stricmp(comando, "CSC")))
	{
		printf("\t- CSC (Limpiar pantalla) :\n");
		printf("\t\tNo tienen parámetros. Limpia la pantalla y coloca el cursor en espera de comandos.\n");
	}
	else if (!(stricmp(comando, "CRU")))
	{
		printf("\t- CRU (Crea unidad lógica) :\n");
		printf("\t\tCRU <destino>\n");
		printf("\t\tEn este caso <destino> se refiere a un nombre de unidad lógica de una sola letra (se toma como mayúscula en cualquier caso) y no puede ejecutarse si existe previamente una unidad con ese nombre.\n");
		printf("\t\tEjemplo: CRU E:/\n");
	}
	else if (!(stricmp(comando, "SRU")))
	{
		printf("\t- SRU (Guarda unidad lógica en archivo) :\n");
		printf("\t\tSRU <destino> <nombre_archivo> \n");
		printf("\t\tEjemplo: SRU D:/ infoD.txt\n");
	}
	else if (!(stricmp(comando, "LRU")))
	{
		printf("\t- LRU (Lee datos de unidad lógica desde archivo) :\n");
		printf("\t\tLRU <nombre_archivo> <destino> \n");
		printf("\t\tLa unidad destino debe existir y estar vacía. \n");
		printf("\t\tEjemplo LRU infoD.txt D:/\n");
	}
	else if (!(stricmp(comando, "FRU")))
	{
		printf("\t- FRU (formatea una unidad lógica) :\n");
		printf("\t\tFRU <destino>  \n");
		printf("\t\tSe pide confirmación (S/N). La unidad destino debe existir se borran todos sus datos dejando vacía la unidad. No importan los accesos de solo lectura o escondido. \n");
		printf("\t\tEjemplo FRU C:/\n");
	}
	else if (!(stricmp(comando, "ERU")))
	{
		printf("\t- ERU (Elimina/desmonta una unidad lógica) :\n");
		printf("\t\tERU <destino>  \n");
		printf("\t\tSe pide confirmación (S/N). La unidad destino debe estar vacía. Elimina\n");
		printf("\t\tEjemplo ERU C:/\n");
	}
	else if (!(stricmp(comando, "EXIT")))
	{
		printf("\t- EXIT (salir del sistema) :\n");
		printf("\t\tEsta opción guarda todas las unidades lógicas en archivos ( uno por unidad ) en la carpeta de trabajo . Ejemplo: C.txt ó F.txt\n");
	}
	else if (!(stricmp(comando, "HELP")))
	{
		printf("\t- HELP (Ayuda de los comandos) :\n");
		printf("\t\tHELP <comando>  \n");
		printf("\t\tMuestra una descripcion detallada del comando ingresado\n");
		printf("\t\tEjemplo: HELP MKD\n");
	}
	else
	{
		printf("\tEl comando '%s' no existe\n", comando);
	}
}

int validarcolor(char *x)
{
	if (validar(2, x) || validar(1, x))
	{
		for (int unsigned i = 0; i < strlen(x); i++)
		{
			if ((x[i] < 48 && x[i] > 57) || (x[i] < 65 && x[i] > 70) || (x[i] < 97 && x[i] > 102))
				return (0);
		}
		return (1);
	}
	return (0);
}
void color(char *x)
{
	if (validarcolor(x))
	{
		if (x[0] > 96 && x[0] < 103)
			x[0] = x[0] - 32;
		if (x[1] > 96 && x[1] < 103)
			x[1] = x[1] - 32;
		switch (x[0])
		{
		case '0': // COLOR 0
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 00");
					break;
				case '1':
					system("color 01");
					break;
				case '2':
					system("color 02");
					break;
				case '3':
					system("color 03");
					break;
				case '4':
					system("color 04");
					break;
				case '5':
					system("color 05");
					break;
				case '6':
					system("color 06");
					break;
				case '7':
					system("color 07");
					break;
				case '8':
					system("color 08");
					break;
				case '9':
					system("color 09");
					break;
				case 'A':
					system("color 0A");
					break;
				case 'B':
					system("color 0B");
					break;
				case 'C':
					system("color 0C");
					break;
				case 'D':
					system("color 0D");
					break;
				case 'E':
					system("color 0E");
					break;
				case 'F':
					system("color 0F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 0");
			break;
		case '2': // COLOR 2
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 20");
					break;
				case '1':
					system("color 21");
					break;
				case '2':
					system("color 22");
					break;
				case '3':
					system("color 23");
					break;
				case '4':
					system("color 24");
					break;
				case '5':
					system("color 25");
					break;
				case '6':
					system("color 26");
					break;
				case '7':
					system("color 27");
					break;
				case '8':
					system("color 28");
					break;
				case '9':
					system("color 29");
					break;
				case 'A':
					system("color 2A");
					break;
				case 'B':
					system("color 2B");
					break;
				case 'C':
					system("color 2C");
					break;
				case 'D':
					system("color 2D");
					break;
				case 'E':
					system("color 2E");
					break;
				case 'F':
					system("color 2F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 2");
			break;
		case '1': // COLOR 1
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 10");
					break;
				case '1':
					system("color 11");
					break;
				case '2':
					system("color 12");
					break;
				case '3':
					system("color 13");
					break;
				case '4':
					system("color 14");
					break;
				case '5':
					system("color 15");
					break;
				case '6':
					system("color 16");
					break;
				case '7':
					system("color 17");
					break;
				case '8':
					system("color 18");
					break;
				case '9':
					system("color 19");
					break;
				case 'A':
					system("color 1A");
					break;
				case 'B':
					system("color 1B");
					break;
				case 'C':
					system("color 1C");
					break;
				case 'D':
					system("color 1D");
					break;
				case 'E':
					system("color 1E");
					break;
				case 'F':
					system("color 1F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 1");
			break;
		case '3': // COLOR 3
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 30");
					break;
				case '1':
					system("color 31");
					break;
				case '2':
					system("color 32");
					break;
				case '3':
					system("color 33");
					break;
				case '4':
					system("color 34");
					break;
				case '5':
					system("color 35");
					break;
				case '6':
					system("color 36");
					break;
				case '7':
					system("color 37");
					break;
				case '8':
					system("color 38");
					break;
				case '9':
					system("color 39");
					break;
				case 'A':
					system("color 3A");
					break;
				case 'B':
					system("color 3B");
					break;
				case 'C':
					system("color 3C");
					break;
				case 'D':
					system("color 3D");
					break;
				case 'E':
					system("color 3E");
					break;
				case 'F':
					system("color 3F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 3");
			break;
		case '4': // COLOR 4
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 40");
					break;
				case '1':
					system("color 41");
					break;
				case '2':
					system("color 42");
					break;
				case '3':
					system("color 43");
					break;
				case '4':
					system("color 44");
					break;
				case '5':
					system("color 45");
					break;
				case '6':
					system("color 46");
					break;
				case '7':
					system("color 47");
					break;
				case '8':
					system("color 48");
					break;
				case '9':
					system("color 49");
					break;
				case 'A':
					system("color 4A");
					break;
				case 'B':
					system("color 4B");
					break;
				case 'C':
					system("color 4C");
					break;
				case 'D':
					system("color 4D");
					break;
				case 'E':
					system("color 4E");
					break;
				case 'F':
					system("color 4F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 4");
			break;
		case '5': // COLOR 5
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 50");
					break;
				case '1':
					system("color 51");
					break;
				case '2':
					system("color 52");
					break;
				case '3':
					system("color 53");
					break;
				case '4':
					system("color 54");
					break;
				case '5':
					system("color 55");
					break;
				case '6':
					system("color 56");
					break;
				case '7':
					system("color 57");
					break;
				case '8':
					system("color 58");
					break;
				case '9':
					system("color 59");
					break;
				case 'A':
					system("color 5A");
					break;
				case 'B':
					system("color 5B");
					break;
				case 'C':
					system("color 5C");
					break;
				case 'D':
					system("color 5D");
					break;
				case 'E':
					system("color 5E");
					break;
				case 'F':
					system("color 5F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 5");
			break;
		case '6': // COLOR 6
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 60");
					break;
				case '1':
					system("color 61");
					break;
				case '2':
					system("color 62");
					break;
				case '3':
					system("color 63");
					break;
				case '4':
					system("color 64");
					break;
				case '5':
					system("color 65");
					break;
				case '6':
					system("color 66");
					break;
				case '7':
					system("color 67");
					break;
				case '8':
					system("color 68");
					break;
				case '9':
					system("color 69");
					break;
				case 'A':
					system("color 6A");
					break;
				case 'B':
					system("color 6B");
					break;
				case 'C':
					system("color 6C");
					break;
				case 'D':
					system("color 6D");
					break;
				case 'E':
					system("color 6E");
					break;
				case 'F':
					system("color 6F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 6");
			break;
		case '7': // COLOR 7
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 70");
					break;
				case '1':
					system("color 71");
					break;
				case '2':
					system("color 72");
					break;
				case '3':
					system("color 73");
					break;
				case '4':
					system("color 74");
					break;
				case '5':
					system("color 75");
					break;
				case '6':
					system("color 76");
					break;
				case '7':
					system("color 77");
					break;
				case '8':
					system("color 78");
					break;
				case '9':
					system("color 79");
					break;
				case 'A':
					system("color 7A");
					break;
				case 'B':
					system("color 7B");
					break;
				case 'C':
					system("color 7C");
					break;
				case 'D':
					system("color 7D");
					break;
				case 'E':
					system("color 7E");
					break;
				case 'F':
					system("color 7F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 7");
			break;
		case '8': // COLOR 8
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 80");
					break;
				case '1':
					system("color 81");
					break;
				case '2':
					system("color 82");
					break;
				case '3':
					system("color 83");
					break;
				case '4':
					system("color 84");
					break;
				case '5':
					system("color 85");
					break;
				case '6':
					system("color 86");
					break;
				case '7':
					system("color 87");
					break;
				case '8':
					system("color 88");
					break;
				case '9':
					system("color 89");
					break;
				case 'A':
					system("color 8A");
					break;
				case 'B':
					system("color 8B");
					break;
				case 'C':
					system("color 8C");
					break;
				case 'D':
					system("color 8D");
					break;
				case 'E':
					system("color 8E");
					break;
				case 'F':
					system("color 8F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 8");
			break;
		case '9': // COLOR 9
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color 90");
					break;
				case '1':
					system("color 91");
					break;
				case '2':
					system("color 92");
					break;
				case '3':
					system("color 93");
					break;
				case '4':
					system("color 94");
					break;
				case '5':
					system("color 95");
					break;
				case '6':
					system("color 96");
					break;
				case '7':
					system("color 97");
					break;
				case '8':
					system("color 98");
					break;
				case '9':
					system("color 99");
					break;
				case 'A':
					system("color 9A");
					break;
				case 'B':
					system("color 9B");
					break;
				case 'C':
					system("color 9C");
					break;
				case 'D':
					system("color 9D");
					break;
				case 'E':
					system("color 9E");
					break;
				case 'F':
					system("color 9F");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color 9");
			break;
		case 'A': // COLOR A
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color A0");
					break;
				case '1':
					system("color A1");
					break;
				case '2':
					system("color A2");
					break;
				case '3':
					system("color A3");
					break;
				case '4':
					system("color A4");
					break;
				case '5':
					system("color A5");
					break;
				case '6':
					system("color A6");
					break;
				case '7':
					system("color A7");
					break;
				case '8':
					system("color A8");
					break;
				case '9':
					system("color A9");
					break;
				case 'A':
					system("color AA");
					break;
				case 'B':
					system("color AB");
					break;
				case 'C':
					system("color AC");
					break;
				case 'D':
					system("color AD");
					break;
				case 'E':
					system("color AE");
					break;
				case 'F':
					system("color AF");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color A");
			break;
		case 'B': // COLOR B
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color B0");
					break;
				case '1':
					system("color B1");
					break;
				case '2':
					system("color B2");
					break;
				case '3':
					system("color B3");
					break;
				case '4':
					system("color B4");
					break;
				case '5':
					system("color B5");
					break;
				case '6':
					system("color B6");
					break;
				case '7':
					system("color B7");
					break;
				case '8':
					system("color B8");
					break;
				case '9':
					system("color B9");
					break;
				case 'A':
					system("color BA");
					break;
				case 'B':
					system("color BB");
					break;
				case 'C':
					system("color BC");
					break;
				case 'D':
					system("color BD");
					break;
				case 'E':
					system("color BE");
					break;
				case 'F':
					system("color BF");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color B");
			break;
		case 'C': // COLOR C
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color C0");
					break;
				case '1':
					system("color C1");
					break;
				case '2':
					system("color C2");
					break;
				case '3':
					system("color C3");
					break;
				case '4':
					system("color C4");
					break;
				case '5':
					system("color C5");
					break;
				case '6':
					system("color C6");
					break;
				case '7':
					system("color C7");
					break;
				case '8':
					system("color C8");
					break;
				case '9':
					system("color C9");
					break;
				case 'A':
					system("color CA");
					break;
				case 'B':
					system("color CB");
					break;
				case 'C':
					system("color CC");
					break;
				case 'D':
					system("color CD");
					break;
				case 'E':
					system("color CE");
					break;
				case 'F':
					system("color CF");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color C");
			break;
		case 'D': // COLOR D
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color D0");
					break;
				case '1':
					system("color D1");
					break;
				case '2':
					system("color D2");
					break;
				case '3':
					system("color D3");
					break;
				case '4':
					system("color D4");
					break;
				case '5':
					system("color D5");
					break;
				case '6':
					system("color D6");
					break;
				case '7':
					system("color D7");
					break;
				case '8':
					system("color D8");
					break;
				case '9':
					system("color D9");
					break;
				case 'A':
					system("color DA");
					break;
				case 'B':
					system("color DB");
					break;
				case 'C':
					system("color DC");
					break;
				case 'D':
					system("color DD");
					break;
				case 'E':
					system("color DE");
					break;
				case 'F':
					system("color DF");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color D");
			break;
		case 'E': // COLOR E
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color E0");
					break;
				case '1':
					system("color E1");
					break;
				case '2':
					system("color E2");
					break;
				case '3':
					system("color E3");
					break;
				case '4':
					system("color E4");
					break;
				case '5':
					system("color E5");
					break;
				case '6':
					system("color E6");
					break;
				case '7':
					system("color E7");
					break;
				case '8':
					system("color E8");
					break;
				case '9':
					system("color E9");
					break;
				case 'A':
					system("color EA");
					break;
				case 'B':
					system("color EB");
					break;
				case 'C':
					system("color EC");
					break;
				case 'D':
					system("color ED");
					break;
				case 'E':
					system("color EE");
					break;
				case 'F':
					system("color EF");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color E");
			break;
		case 'F': // COLOR D
			if (x[1])
			{
				switch (x[1])
				{
				case '0':
					system("color F0");
					break;
				case '1':
					system("color F1");
					break;
				case '2':
					system("color F2");
					break;
				case '3':
					system("color F3");
					break;
				case '4':
					system("color F4");
					break;
				case '5':
					system("color F5");
					break;
				case '6':
					system("color F6");
					break;
				case '7':
					system("color F7");
					break;
				case '8':
					system("color F8");
					break;
				case '9':
					system("color F9");
					break;
				case 'A':
					system("color FA");
					break;
				case 'B':
					system("color FB");
					break;
				case 'C':
					system("color FC");
					break;
				case 'D':
					system("color FD");
					break;
				case 'E':
					system("color FE");
					break;
				case 'F':
					system("color FF");
					break;
				default:
					system("color i");
					break;
				}
			}
			else
				system("color F");
			break;
		default:
			system("color i");
			break;
		}
	}
	else
		system("color i");
}