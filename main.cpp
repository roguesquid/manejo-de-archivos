// Proyecto Etapa I
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
	char *nom;
	char sep = '/';
	nom = strrchr(ruta, sep);
	nom = strtok(nom, "/");
	if (!nom)
	{
		memset(nombre, '\0', sizeof(nombre));
		memset(straux, '\0', sizeof(straux));
		strcpy(nombre, ruta);
		return(0);
	}
	lenRuta = strlen(ruta) - strlen(nom) - 1;
	memset(nombre, '\0', sizeof(nombre));
	memset(straux, '\0', sizeof(straux));
	strcpy(nombre, nom);
	strncpy(straux, ruta, lenRuta);
	return(1);
}

dir *moverpunterov3(char *t, dir *p, int x)
{
	if (x)
		t = strtok(t, "/ \n");
	if (t)
	{
		if (p)
		{
			if (!strcmp(t, "."))
			{
				t = strtok(NULL, "/ \n");
				p = moverpunterov3(t, p, 0);
				return (p);
			}
			else if (!strcmp(t, ".."))
			{
				t = strtok(NULL, "/ \n");
				p = p->ppa;
				p = moverpunterov3(t, p, 0);
				return (p);
			}
			else
			{
				if (p->pfa)
				{
					p = p->pfa;
					while (p)
					{
						if (!strcmp(p->nom, t))
						{
							t = strtok(NULL, "/ \n");
							p = moverpunterov3(t, p, 0);
							return (p);
						}
						p = p->pul;
					}
					printf("Ruta no encontrada\n");
					return (NULL);
				}
				else
				{
					printf("Ruta no encontrada\n");
					return (NULL);
				}
			}
		}
		else
		{
			printf("Ruta no encontrada\n");
			return (NULL);
		}
	}
	else
	{
		if (p)
			return (p);
		else
		{
			printf("Ruta no encontrada\n");
			return (NULL);
		}
	}
}
// funcion de impresion de ubicacion actual
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

// fin de impresion ubicacion actual

// funciones de impresion de directorios
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
			printf("|%-8s       <%c>     %s     -r\n", aux->nom, aux->tip, aux->fcm);
		}
		else
		{
			if (check == 1)
			{
				for (int i = 0; i != saltos; i++)
					printf("|%*c", -10, 32);
			}
			printf("|%-8s       <%c>     %s     +r\n", aux->nom, aux->tip, aux->fcm);
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
		if (aux->h == 1)
		{
			aux = aux->pul;
			if (aux == NULL)
				return;
		}
		while (aux)
		{
			if (aux->r == 0)
				printf("|%-8s       <%c>     %s     -r\n", aux->nom, aux->tip, aux->fcm);
			else
				printf("|%-8s       <%c>     %s     +r\n", aux->nom, aux->tip, aux->fcm);
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
					printf("|%-8s       <%c>     %s     -r+h\n", aux->nom, aux->tip, aux->fcm);
				else
					printf("|%-8s       <%c>     %s     +r+h\n", aux->nom, aux->tip, aux->fcm);
				break;
			case 0:
				if (aux->r == 0)
					printf("|%-8s       <%c>     %s     -r-h\n", aux->nom, aux->tip, aux->fcm);
				else
					printf("|%-8s       <%c>     %s     +r-h\n", aux->nom, aux->tip, aux->fcm);
				break;
			}
			aux = aux->pul;
		}
	}
}

void mostrar(dir *p, int check)
{
	system("cls");
	printf("\n\n---------------------------------------------------");
	printf("\n  Manejo virtual de Archivos (MVA) ");
	printf("\n  [7. Mostrar directorio (SHD)]");
	printf("\n---------------------------------------------------\n");
	printf("\n");
	printv2(p);
	printf("\n\n");
	if (p->pfa == NULL)
	{
		printf("  ");
		return;
	}
	if (check == 0)
	{
		mostrardirectorio(p, 0);
	}
	else if (check == 1)
	{
		mostrardirectorio(p, 1);
	}
	else
		mostrarcasitodo(p, 0, 0);
	printf("\n\n  ");
}

// Fin menu mov/cpd/borrar

// fin funciones de impresion

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
	for (int i = 0; i < strlen(x); i++)
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

// fincion para verficar si el token de ruta inicia con la unidad
int verificartoken(char *ruta)
{
	char x[512], *t;
	int resp = 0;
	strcpy(x, ruta);
	t = strtok(x, "/");
	/*while(t){
		resp++;
		t=strtok(NULL,"/");
	}
	if(resp==1) {*/
	if (validar(2, x))
	{
		for (int i = 0; i < strlen(x); i++)
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
	//}
	// else return(0);
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

int descomponerfecha(char *a, int check)
{
	if (check == 2)
	{
		int cont = 0;
		for (int i = 0; i != 4; i++)
		{
			if ((a[i] != '0') && (a[i] != '1') && (a[i] != '2') && (a[i] != '3') && (a[i] != '4') && (a[i] != '5') && (a[i] != '6') && (a[i] != '7') && (a[i] != '8') && (a[i] != '9'))
			{
				return (0);
			}
		}
		return (1);
	}
	else if (check == 1)
	{
		if (a[0] == '1')
		{
			if ((a[1] == '0') || (a[1] == '1') || (a[1] == '2'))
			{
				return (1);
			}
			else
				return (0);
		}
		else if (a[0] == '0')
		{
			if ((a[1] == '1') || (a[1] == '2') || (a[1] == '3') || (a[1] == '4') || (a[1] == '5') || (a[1] == '6') || (a[1] == '7') || (a[1] == '8') || (a[1] == '9'))
			{
				return (1);
			}
			else
				return (0);
		}
		else
			return (0);
	}
	else
	{
		if (a[0] == '1')
		{
			if ((a[1] == '0') || (a[1] == '1') || (a[1] == '2') || (a[1] == '3') || (a[1] == '4') || (a[1] == '5') || (a[1] == '6') || (a[1] == '7') || (a[1] == '8') || (a[1] == '9'))
			{
				return (1);
			}
			else
				return (0);
		}
		else if (a[0] == '0')
		{
			if ((a[1] == '1') || (a[1] == '2') || (a[1] == '3') || (a[1] == '4') || (a[1] == '5') || (a[1] == '6') || (a[1] == '7') || (a[1] == '8') || (a[1] == '9'))
			{
				return (1);
			}
			else
				return (0);
		}
		else if (a[0] == '2')
		{
			if ((a[1] == '0') || (a[1] == '1') || (a[1] == '2') || (a[1] == '3') || (a[1] == '4') || (a[1] == '5') || (a[1] == '6') || (a[1] == '7') || (a[1] == '8') || (a[1] == '9'))
			{
				return (1);
			}
			else
				return (0);
		}
		else if (a[0] == '3')
		{
			if ((a[1] == '0') || (a[1] == '1'))
			{
				return (1);
			}
			else
				return (0);
		}
		else
			return (0);
	}
}

void modificarfecha(dir **p)
{
	char x[16], z[9];
	printf("\n\tIntroduzca el nuevo a%co de creacion del directorio (aaaa - 4 digitos):  ", 164);
	scanf("%s", x);
	if (strlen(x) != 4)
	{
		printf(" \n\t\t\t  ERROR\n\n");
		printf("\tel dato introducido no tiene 4 digitos\n\n  ");
	}
	else if (descomponerfecha(&x[0], 2))
	{
		strcpy(z, x);
		printf("\n\tIntroduzca el nuevo mes de creacion del directorio (mm - 2 digitos):  ");
		scanf("%s", x);
		if (strlen(x) != 2)
		{
			printf(" \n\t\t\t  ERROR\n\n");
			printf("\tel dato introducido no tiene 2 digitos\n\n  ");
		}
		else if (descomponerfecha(&x[0], 1))
		{
			strcat(z, x);
			printf("\n\tIntroduzca el nuevo dia de creacion del directorio (dd - 2 digitos): ");
			scanf("%s", x);
			if (strlen(x) != 2)
			{
				printf(" \n\t\t\t  ERROR\n\n");
				printf("\tel dato introducido no tiene 2 digitos\n\n  ");
			}
			else if (descomponerfecha(&x[0], 0))
			{
				strcat(z, x);
				strcpy((*p)->fcm, z);
				printf("\n  Se actualizo la fecha exitosamente\n\n  ");
			}
			else
			{
				printf(" \n\t\t\t  ERROR\n\n");
				printf("\tel dato introducido: %s no es un dia valido\n\n  ", x);
			}
		}
		else
		{
			printf(" \n\t\t\t  ERROR\n\n");
			printf("\tel dato introducido: %s no es un mes valido\n\n   ", x);
		}
	}
	else
	{
		printf(" \n\t\t\t  ERROR\n\n");
		printf("\t el dato introducido: %s no es un dato valido\n\n  ", x);
	}
}
// fin de funcion para obtener la fecha

// funcion para mover un puntero a una direccion dada (Usada en borrar, mover y copiar)
// En esta funcion verificamos si la cadena se acabo antes del puntero que movemos, si ocurre existe la direccion
// En caso de que se acabe el puntero antes no existe
dir *moverpunterov2(char *t, dir *p, int x)
{
	if (x == 1)
	{
		while (p)
		{
			if (!strcmp(p->nom, t))
			{ // Comprueba todas las direcciones hermanas
				t = strtok(NULL, "/");
				if (t != NULL)
				{ // Preguntamos si sigue quedando string
					if (p->pfa)
					{ // Preguntamos si el puntero actual contiene hijos
						p = p->pfa;
						p = moverpunterov2(t, p, x);
						return (p);
					}
					else
						return (NULL); // Se acaba puntero (No existe)
				}
				else
					return (p); // se acabo string (Existe)
			}
			p = p->pul;
		}
		return (NULL); // De no existir en las direcciones hermanas, la direccion no existe
	}
	else
	{
		t = strtok(t, "/");
		if (t != NULL)
		{ // Si no existe nada, el usuario puso una direccion vacia
			while (p)
			{
				if (!strcmp(p->nom, t))
				{ // Comprueba si la primera direccion es igual al nombre de la unidad logica
					t = strtok(NULL, "/");
					if (t != NULL)
					{ // Verificamos si se acabo el string
						if (p->pfa)
						{ // Preguntamos si tiene hijos
							p = p->pfa;
							p = moverpunterov2(t, p, 1);
							return (p);
						}
						else
							return (NULL); // Si no tiene, al acabarse el puntero antes que el string no existe la direccion
					}
					else
						return (p); // Si se acabo el string, la direccion es la unidad logica
				}
				else
					return (NULL); // Si no lo es, la direccion no existe
				p = p->pul;
			}
			return (NULL);
		}
		else
			return (NULL); // En este caso, al tratarse de la primera direccion si se acabo el string, no existe
	}
}
// fin de mover puntero

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
	if (p->ppa)
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
							if (!strcmp(x, ax->nom))
							{
								printf("ERROR:  Existe un directorio de igual nombre en la ubicacion actual\n\n  ");
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
					printf("ERROR:  El nombre introducido no es alfanumerico\n");
			}
			else
				printf("ERROR:  El nombre introducido tiene una longitud mayor a 8 caracteres\n");
		}
		else if (strcmp(p->ppa->nom, "root"))
			printf("ERROR:  El directorio actual solo permite la lectura\n");
		else
			printf("ERROR:  La unidad logica solo permite la lectura\n");
	}
	else
		printf("ERROR:  Acceso denegado\n");
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
		while (op[0] != 'S' && op[0] != 'N')
		{
			printf("%cEl directorio contiene subidirectorios desea borrarlo? (S,N) ", 168);
			scanf("%s", &op[0]);
			if (validar(1, op))
			{
				switch (op[0])
				{
				case 'S':
					if (!comprobar(p->pfa))
					{ // comprobamos si algun hijo esta protegido, si lo esta no se realiza el borrado
						if (contenido(*ax, &p))
							*ax = NULL;
						borrartodo(p->pfa);
						darfecha(&p->ppa);
						dir *t = p;
						link(t);
						delete (p);
						fflush(stdin);
					}
					else
					{
						printf("Error El directorio contiene archivos protegidos\n");
					}
					break;
				case 'N':
					fflush(stdin);
					break;
				}
			}
		}
	}
	else
	{
		if (contenido(*ax, &p))
			*ax = NULL;
		borrartodo(p->pfa);
		darfecha(&p->ppa);
		dir *t = p;
		link(t);
		delete (p);
	}
}
// fin borrar dentro

// funcion que contempla los casos borde y ubica el puntero
void borrar(dir *p, dir **ax, char *ruta, int op)
{
	if (verificartoken(ruta))
		p = moverpunterov3(ruta, p, 1); // ruta absoluta
	else
		p = moverpunterov3(ruta, (*ax)->pfa, 1); // Ruta relativa
	if (p)
	{
		if (p->ppa)
		{
			if (p->ppa && strcmp(p->ppa->nom, "root"))
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
					printf("Error el directorio a borrar solo permite la lectura\n");
			}
			else
				printf("Error No se puede borrar la unidad [%s] logica\n", p->nom);
		}
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
void sobreescribir(dir **p, dir **ax, int x)
{
	char op[10] = "-1", op2[10] = "-1";
	dir *g;
	while (op[0] != '1' && op[0] != '2')
	{
		printf("\n\n  Existe un directorio de igual nombre en la ubicacion destino\n");
		printf("  %cDesea sobreescribir los datos?\n\n", 168);
		printf("  Se perderan los anteriores\n\n");
		printf("  1. Si\n");
		printf("  2. No\n");
		printf("  Su opcion [1-2]: ");
		scanf("%s", &op);
		if (validar(1, op))
		{
			switch (op[0])
			{
			case '1':
				if ((*p)->pfa)
				{
					if (!comprobar((*p)->pfa))
					{
						if (x == 1)
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
						if (contenido(*ax, p))
							*ax = NULL;
						borrartodo((*p)->pfa);
						delete (*p);
						printf("\n  El directorio se ha sobreescrito exitosamente\n\n  ");
					}
					else
					{
						printf("\n\t\t\t  ERROR\n\n");
						if (x == 1)
							printf("               El directorio no se ha movido\n");
						else
							("               El directorio no se ha copiado\n");
						printf("  El directorio contiene archivos protegidos (solo lectura)\n\n  ");
					}
				}
				else
				{
					if (x == 1)
					{
						link(*ax);
						(*ax)->pul = (*p)->pul;
						(*ax)->ppa = (*p)->ppa;
						linksobreescribir(*p, *ax);
						darfecha(&(*ax)->ppa);
						darfecha(ax);
					}
					else
					{
						copiartodo(*ax, 1, &g);
						g->pul = (*p)->pul;
						g->ppa = (*p)->ppa;
						g->r = 0;
						darfecha(&g->ppa);
						darfecha(&g);
						linksobreescribir(*p, g);
					}
					delete (*p);
					printf("\n  El directorio se ha sobreescrito exitosamente\n\n  ");
				}
				break;
			case '2':
				if (x == 1)
					printf("\n  El directorio no se ha movido\n\n  ");
				else
					printf("\n  El directorio no se ha copiado\n\n  ");
				break;
			default:
				printf("\n\t\t\t  ERROR\n\n");
				printf("  Recuerde que debe elegir una opcion en el rango [1-2]\n\n  ");
				break;
			}
		}
		else
		{
			printf("\n\t\t\t  ERROR\n\n");
			printf("  Recuerde que debe elegir una opcion en el rango [1-2]\n\n  ");
			system("pause");
		}
	}
}
// fin sobreescribir

// funcion que ubica al puntero y contempla los casos posibles para mover
// Comprueba ademas si el directorio al cual se desea mover tiene un directorio de igual nombre al directorio que movemos
// Si es asi se envia a la funcion de arriba (sobreescribir)
void mover(dir *p, dir **ax)
{
	if ((p->pfa) && (p != *ax))
	{
		if ((*ax)->r != 1)
		{
			int x;
			char string[400];
			printf("\n  NOTA: El formato de la direccion debe ser Unidad_logica/dir1/dir2/...dirN/");
			printf("\n  	En caso de no separar con diagonales la direccion no se encontrara");
			printf("\n\n  Introduzca la direccion a la cual desea mover el directorio actual: \n\n  ");
			scanf("%s", string);
			p = moverpunterov2(string, p, 0);
			if (p)
			{
				if (p->r != 1)
				{
					if ((*ax)->ppa != p)
					{
						if (!contenido(p, ax))
						{
							if (p->pfa)
							{
								p = p->pfa;
								while (p)
								{
									if (!strcmp((*ax)->nom, p->nom))
									{
										sobreescribir(&p, ax, 1);
										return; // Indica que existe un directorio de igual nombre
									}
									if (p->pul == NULL)
										break;
									p = p->pul;
								}
								darfecha(&(*ax)->ppa);
								link(*ax);
								p->pul = *ax;
								(*ax)->pul = NULL;
								(*ax)->ppa = p->ppa;
								darfecha(&(*ax)->ppa);
								printf("\n  El directorio se ha movido exitosamente\n\n  ");
							}
							else
							{
								darfecha(&(*ax)->ppa);
								link(*ax);
								p->pfa = *ax;
								(*ax)->ppa = p;
								(*ax)->pul = NULL;
								darfecha(&(*ax)->ppa);
								printf("\n  El directorio se ha movido exitosamente\n\n  ");
							}
						}
						else
						{
							printf("\n\t\t\t     ERROR\n\n");
							printf("  Desea mover el directorio actual a un subdirectorio de el mismo\n\n  ");
						}
					}
					else
					{
						printf("\n\t\t\t  ERROR\n\n");
						printf("  El directorio ya se encuentra en la ubicacion deseada\n\n  ");
					}
				}
				else
				{
					printf("\n\t\t     ERROR\n\n");
					printf("  El directorio destino solo permite la lectura\n\n  ");
				}
			}
			else
			{
				printf("\n\t\t ERROR\n\n");
				printf("  La direccion suministrada no existe\n\n  ");
			}
		}
		else
		{
			printf("\n\t\t      ERROR\n\n");
			printf("  El directorio a mover solo permite la lectura\n\n  ");
		}
	}
	else
	{
		printf("\n\t\t    ERROR\n\n");
		printf("  No se puede mover la unidad logica [%s/]\n\n  ", p->nom);
	}
}

// copiar
void copiar(dir *p, dir **ax)
{
	if (((p->pfa) && (p != *ax)))
	{
		int x;
		char string[300];
		dir *g;
		printf("\n  NOTA: El formato de la direccion debe ser Unidad_logica/dir1/dir2/...dirN/");
		printf("\n  	En caso de no separar con diagonales la direccion no se encontrara");
		printf("\n\n  Introduzca la direccion a la cual desea copiar el directorio actual: \n\n  ");
		scanf("%s", string);
		p = moverpunterov2(string, p, 0);
		if (p)
		{
			if (p->r != 1)
			{
				if (!contenido(p, ax))
				{
					if (p->pfa)
					{
						p = p->pfa;
						while (p)
						{
							if (!strcmp((*ax)->nom, p->nom))
							{
								sobreescribir(&p, ax, 0);
								return; // Indica que existe directorio de igual nombre
							}
							if (p->pul == NULL)
								break;
							p = p->pul;
						}
						copiartodo(*ax, 1, &g);
						p->pul = g;
						g->pul = NULL;
						g->ppa = p->ppa;
						g->r = 0;
						darfecha(&g->ppa);
						darfecha(&g);
						printf("\n  El directorio se ha copiado exitosamente\n\n  ");
					}
					else
					{
						copiartodo(*ax, 1, &g);
						g->ppa = p;
						p->pfa = g;
						g->pul = NULL;
						g->r = 0;
						darfecha(&g->ppa);
						darfecha(&g);
						printf("\n  El directorio se ha copiado exitosamente\n\n  ");
					}
				}
				else
				{
					printf("\n\t\t\t  ERROR\n\n");
					printf(" No puede copiar un directorio padre dentro de una de sus carpetas\n\n ");
				}
			}
			else
			{
				printf("\n\t\t     ERROR\n\n");
				printf("  El directorio destino solo permite la lectura\n\n  ");
			}
		}
		else
		{
			printf("\n\t\t\t  ERROR\n\n");
			printf("  La direccion suministrada no existe\n\n  ");
		}
	}
	else
	{
		printf("\n\t\t\t  ERROR\n\n");
		printf("  La unidad logica [%s/] no puede ser copiada\n\n  ", p->nom);
	}
}

// modificar
void modificar(dir *ax, dir *p, char x)
{
	int r;
	char y[20];
	switch (x)
	{
	case '0':
		printf("\n\n  ");
		break;
	case '1':
		if (ax != p)
		{
			printf("\n  NOTA: Recuerde que el nuevo nombre debe ser menor o igual a 8 caracteres ");
			printf("\n        Los nombres deben ser alfanumericos");
			printf("\n  Introduzca el nuevo nombre que desea darle al directorio: ");
			scanf("%s", y);
			if (validar(8, y))
			{
				if (alfanum(y))
				{
					dir *t = ax;
					t = t->pul;
					while (t)
					{
						if (!strcmp(t->nom, y))
						{
							printf("\n\t\t\t      ERROR\n\n");
							printf("  Ya existe un directorio con el mismo nombre en la ubicacion actual\n\n  ");
							return;
						}
						t = t->pul;
					}
					darfecha(&ax);
					darfecha(&ax->ppa);
					strcpy(ax->nom, y);
					printf("\n  Se realizo el cambio exitosamente\n\n  ");
				}
				else
				{
					printf("\n\t\t    ERROR\n\n");
					printf("  Recuerde que el nombre debe ser alfanumerico\n\n  ");
				}
			}
			else
			{
				printf("\n\t\t\t     ERROR\n\n");
				printf("  Recuerde que el nombre debe ser menor o igual a 8 caracteres\n\n  ");
			}
		}
		else
		{
			printf("\n\t\t  ERROR\n\n");
			printf("  No se puede renombrar la unidad logica\n\n  ");
		}
		break;
	case '2':
		modificarfecha(&ax);
		break;
	case '3':
		printf("\n  Indique si desea que el directorio sea solo lectura\n");
		printf("  0. No\n");
		printf("  1. Si\n");
		printf("  Su opcion [0-1]: ");
		scanf("%s", &y);
		if (validar(1, y))
		{
			if (y[0] == '1' || y[0] == '0')
			{
				r = atoi(y);
				ax->r = r;
				darfecha(&ax);
				printf("\n  Se realizo el cambio exitosamente\n\n  ");
			}
			else
			{
				printf("\n\t\t\t  ERROR\n\n");
				printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
			}
		}
		else
		{
			printf("\n\t\t\t  ERROR\n\n");
			printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
		}
		break;
	case '4':
		printf("\n  Indique si desea que el directorio se encuentre oculto\n");
		printf("  0. No\n");
		printf("  1. Si\n");
		printf("  Su opcion [0-1]: ");
		scanf("%s", &y);
		if (validar(1, y))
		{
			if (y[0] == '1' || y[0] == '0')
			{
				r = atoi(y);
				ax->h = r;
				darfecha(&ax);
				printf("\n  Se realizo el cambio exitosamente\n\n  ");
			}
			else
			{
				printf("\n\t\t\t  ERROR\n\n");
				printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
			}
		}
		else
		{
			printf("\n\t\t\t  ERROR\n\n");
			printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
		}
		break;
	}
}
// fin modificar

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

void guardardirectorios(FILE **fp, dir *p, int check)
{
	dir *t = p;
	t = t->pfa;
	if (check == 0)
		*fp = fopen("directorios.txt", "w");
	else
		*fp = fopen("directorios.txt", "a");
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
			guardardirectorios(fp, t, 1);
			*fp = fopen("directorios.txt", "a");
			fprintf(*fp, "%c\n", '$'); // $ indica que hasta allí llega la familia de subdirectorios
		}
		t = t->pul;
		if (t != NULL)
			fprintf(*fp, "%c\n", '/'); // '/' indica que los directorios que se guardan a continuación son hermanos
	}
	fclose(*fp);
}
// fin de lectura de archivos ////////////////////////////////////////////

void CHD(dir *p, dir **ax, char *ruta)
{
	if (verificartoken(ruta))
		p = moverpunterov3(ruta, p, 1);
	else
		p = moverpunterov3(ruta, *ax, 1);
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
		if (!strcmp(op, "/o"))
			borrar(p, ax, ruta, 1);
		else
			printf("El comando es incorrecto\n");
	}
	else
		borrar(p, ax, ruta, 0);
	if (*ax == NULL)
		*ax = p;
}

void CU(dir *p, char *nom)
{
	dir *ax, *t = new dir;
	if (verificartoken(nom))
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
			if (!strcmp(nom, ax->nom))
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

/*void CRU(dir *p,dir *ax, char *nom){
	char *t=NULL,*ruta=NULL;int c=0;
	if(nom[0]!='.'){
		t=strtok(nom,"/ \n");
		while(t){
			c++;
			t=strtok(NULL,"/ \n");
		}
		if(c!=1){
			printf("ERROR comando incorrecto\n");
		} else {
			if(verificartoken(nom)) crear(p, nom);
			else printf("ERROR: Nombre de Unidad invalido\n");
		}
	} else {
		system("pause");
		separaRuta(nom,t,ruta);
		system("pause");
		//ruta=strtok(ruta,"/ \n");
		printf("%s\n",t);
		printf("%s\n",ruta);
		system("pause");
		p=relative(ruta,ax);
		system("Pause");
		if(p) {
			if(!p->ppa) {
				if(verificartoken(t)) crear(p, t);
				else printf("ERROR: Nombre de Unidad invalido\n");
			} else printf("ERROR: comando invalido");
		} else printf("ERROR: Ruta invalida");
	}
}*/

void CRU(dir *p, dir *ax, char *rutadest)
{
	char ruta[512], nom[24];
	if (separaRuta(rutadest, nom, ruta))
	{	
		if (verificartoken(ruta))
			p = moverpunterov3(ruta, p, 1);
		else
			p = moverpunterov3(ruta, ax, 1);
		if (p && !p->ppa)
			CU(p, nom);
		else
			printf("ERROR: Ruta invalida\n");
	}
	else
	{ 
		CU(p, nom);	
	}
}

int main()
{
	dir *q = new dir, *p = new dir, *ax = p;
	FILE *fp;
	char raw[1024], aux[1024], *t, *ordenado[4];
	int op = -1, i;
	q->tip = 'U';
	q->h = 0;
	q->r = 0;
	darfecha(&p);
	q->ppa = NULL;
	q->pfa = NULL;
	q->pul = NULL;
	strcpy(q->nom, "root");
	////////////////////////
	p->tip = 'U';
	p->h = 0;
	p->r = 0;
	darfecha(&p);
	p->ppa = q;
	p->pfa = NULL;
	p->pul = NULL;
	strcpy(p->nom, "C:");
	//////
	q->pfa = p;
	////
	if ((fp = fopen("directorios.txt", "r")) != NULL)
	{
		int c = fgetc(fp);
		if (c != EOF)
		{
			ungetc(c, fp);
			leerarchivo(&fp, ax);
		}
		fclose(fp);
	}
	while (op)
	{
		for (int j = 0; j < 4; j++)
			ordenado[j] = NULL;
		printv2(ax);
		printf(">");
		fgets(raw, 1024, stdin);
		strcpy(aux, raw);
		t = strtok(raw, " \n");
		i = 0;
		while (t)
		{
			ordenado[i] = t;
			i++;
			t = strtok(NULL, " \n");
		}
		if (!ordenado[0])
		{
			printf("El comando es incorrecto\n");
		}

		else if (!(strcmp(ordenado[0], "MKD")))
		{
			int h = 0, r = 0;
			dir *auxRoot = NULL;
			char nombre[24];
			char ruta[512];

			if (ordenado[2] && ordenado[3])
			{
				if (!(strcmp(ordenado[3], "/h")) || !(strcmp(ordenado[2], "/h")))
				{
					h = 1;
				}
				if (!(strcmp(ordenado[3], "/r")) || !(strcmp(ordenado[2], "/r")))
				{
					r = 1;
				}
			}
			else if (ordenado[2] && !ordenado[3])
			{
				if (!(strcmp(ordenado[2], "/h")))
				{
					h = 1;
				}
				else if (!(strcmp(ordenado[2], "/r")))
				{
					r = 1;
				}
			}

			if(separaRuta(ordenado[1], nombre, ruta)){
				if (verificartoken(ruta))
					auxRoot = moverpunterov3(ruta, q, 1);
				else
					auxRoot = moverpunterov3(ruta, ax, 1);
				if(auxRoot) crear(auxRoot, nombre, h, r);
			} else {
				system("Pause");
				crear(ax,nombre,h,r);
			}	
				
		}

		else if (!(strcmp(ordenado[0], "CHD")))
		{
			if ((ordenado[2] || ordenado[3]) || !ordenado[1])
				printf("El comando es incorrecto\n");
			else
				CHD(q, &ax, ordenado[1]);
		}
		else if (!(strcmp(ordenado[0], "RMD")))
		{
			if (!ordenado[1] || ordenado[3])
				printf("El comando es incorrecto\n");
			else
				RMD(q, &ax, ordenado[1], ordenado[2]);
		}
		else if (!(strcmp(ordenado[0], "CPD")))
		{
		}
		else if (!(strcmp(ordenado[0], "MVD")))
		{
		}
		else if (!(strcmp(ordenado[0], "MDD")))
		{
		}
		else if (!(strcmp(ordenado[0], "SHD")))
		{
			mostrar(ax, 2);
		}
		else if (!(strcmp(ordenado[0], "CSC")))
		{
			system("cls");
		}
		else if (!(strcmp(ordenado[0], "CRU")))
		{
			CRU(q,ax,ordenado[1]);
		}
		else if (!(strcmp(ordenado[0], "SRU")))
		{
		}
		else if (!(strcmp(ordenado[0], "LRU")))
		{
		}
		else if (!(strcmp(ordenado[0], "FRU")))
		{
		}
		else if (!(strcmp(ordenado[0], "ERU")))
		{
		}
		else if (!(strcmp(ordenado[0], "EXIT")))
		{
			if (ordenado[1] || ordenado[2] || ordenado[3])
				printf("El comando es incorrecto\n");
			else
				op = 0;
		}
		else
		{
			printf("El comando es incorrecto\n");
		}
		// lo lee, funciona bien
	}
}
/*while (op[0]!='0'){
	system("cls");
	encabezado();
	printf("\n\n\t---------------------------------------------------");
	printf( "\n\t\tManejo virtual de directorios (MVA) ");
	printf("\n\t---------------------------------------------------\n");
	printf("\t\t");printv2(aux,1);printf("]\n\n");
	printf("\t\tManejo de directorios\n\n");
	printf( "\t\t1. Crear un directorio (MKD)\n");
	printf( "\t\t2. Cambiar directorio (CHD)\n");
	printf( "\t\t3. Borrar directorio (RMD)\n");
	printf( "\t\t4. Mover directorio (MVD)\n");
	printf( "\t\t5. Copiar directorio (CPD)\n");
	printf( "\t\t6. Modificar directrorio (MDD)\n");
	printf( "\t\t7. Mostrar contenido (SDH)\n");
	printf( "\t\t0. SALIR y guardar datos\n\n ");
	printf("\t\tSu opcion [0-7]: ");
	scanf("%s", &op);
	if(validar(1,op)){
		switch (op[0]){
			case '0':
				printf("\n\n\t  Los datos se han guardado existosamente\n\n  ");
				printf("\t      %c%c Gracias por usar el programa %c%c\n\n   ",1,1,1,1);
			break;
			case '1':
				while(op2[0]!='0'){
					system("cls");
					encabezado();
					printf("\n\n---------------------------------------------------");
					printf( "\n  Manejo virtual de directorios (MVA) ");
					printf( "\n  [1. Crear un directorio (MKD)]");
					printf("\n---------------------------------------------------\n");
					printf("\n  ");printv2(aux,1);printf("]\n\n");
					printf("  1. Nombre del directorio a crear\n");
					printf("  0. Salir\n");
					printf("  Su opcion [0-1]: ");
					scanf("%s", &op2);
					if(validar(1,op2)){
						switch (op2[0]){
							case '0':
								printf("\n\n  ");
							break;
							case '1':
								crear(aux);
							break;
							default:
								printf("\n\t\t\t  ERROR\n\n");
								printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
							break;
						}
						system("pause");
					} else {
						printf("\n\t\t\t  ERROR\n\n");
						printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
						system("pause");
					}
				}
			break;
			case '2':
				while(op2[0]!='0'){
					system("cls");
					encabezado();
					printf("\n\n---------------------------------------------------");
					printf( "\n  Manejo virtual de directorios (MVA) ");
					printf( "\n  [2. Cambiar de directorio (CHD)]");
					printf("\n---------------------------------------------------\n");
					printf("\n  ");printv2(aux,1);printf("]\n\n");
					printf("  1. Subir a directorio padre\n");
					printf("  2. Bajar a un subdirectorio (indmediato)\n");
					printf("  0. Volver a pantalla principal\n");
					printf("  Su opcion [0-2]: ");
					scanf("%s",&op2);
					if(validar(1,op2)){
						switch (op2[0]){
							case '0':
								printf("\n\n  ");
							break;
							case '1':
								subir(&aux);
							break;
							case '2':
								bajar(&aux);
							break;
							default:
								printf("\n\t\t\t  ERROR\n\n");
								printf("  Recuerde que debe elegir una opcion en el rango [0-2]\n\n  ");
							break;
						}
						system("pause");
					} else {
						printf("\n\t\t\t  ERROR\n\n");
						printf("  Recuerde que debe elegir una opcion en el rango [0-2]\n\n  ");
						system("pause");
					}
				}
			break;
		case '3':
				while(op2[0]!='0'){
					system("cls");
					encabezado();
					printf("\n\n---------------------------------------------------");
					printf( "\n  Manejo virtual de directorios (MVA) ");
					printf( "\n  [3. Borrar directorio (RMD)]");
					printf("\n---------------------------------------------------\n");
					printf("\n  ");printv2(aux,1);printf("]\n\n");
					printf("  1. Nombre del directorio a borrar\n");
					printf("  0. Volver a pantalla principal\n");
					printf("  Su opcion [0-1]: ");
					scanf("%s",&op2);
					if(validar(1,op2)){
						switch (op2[0]){
							case '0':
								printf("\n\n  ");
							break;
							case '1':
								borrar(p,&aux);
								if(aux==NULL) aux = p;
							break;
							default:
									printf("\n\t\t\t  ERROR\n\n");
									printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");

							break;
						}
						system("pause");
					} else {
						printf("\n\t\t\t  ERROR\n\n");
						printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
						system("pause");
					}
				}
			break;
			case '4':
				while(op2[0]!='0'){
					system("cls");
					encabezado();
					printf("\n\n---------------------------------------------------");
					printf( "\n  Manejo virtual de directorios (MVA) ");
					printf( "\n  [4. Mover directorio (MVD)]");
					printf("\n---------------------------------------------------\n");
					printf("\n  ");printv2(aux,1);printf("]\n\n");
					printf("  1. Nombre del directorio destino\n");
					printf("  0. Volver a pantalla principal\n");
					printf("  Su opcion [0-1]: ");
					scanf("%s",&op2);
					if(validar(1,op2)){
						switch (op2[0]){
							case '0':
								printf("\n\n  ");
							break;
							case '1':
								mover(p,&aux);
								if(aux==NULL) aux = p;
							break;
							default:
									printf("\n\t\t\t  ERROR\n\n");
									printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
							break;
						}
						system("pause");
					} else {
						printf("\n\t\t\t  ERROR\n\n");
						printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
						system("pause");
					}
				}
			break;
			case '5':
				while(op2[0]!='0'){
					system("cls");
					encabezado();
					printf("\n\n---------------------------------------------------");
					printf( "\n  Manejo virtual de directorios (MVA) ");
					printf( "\n  [5. Copiar directorio (CPD)]");
					printf("\n---------------------------------------------------\n");
					printf("\n  ");printv2(aux, 1);printf("]\n\n");
					printf("  1. Nombre del directorio destino\n");
					printf("  0. Volver a pantalla principal\n");
					printf("  Su opcion [0-1]: ");
					scanf("%s",&op2);
					if(validar(1,op2)){
						switch (op2[0]){
							case '0':
								printf("\n\n  ");
							break;
							case '1':
								copiar(p,&aux);
								if(aux==NULL) aux = p;
							break;
							default:
									printf("\n\t\t\t  ERROR\n\n");
									printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
							break;
						}
						system("pause");
					} else {
						printf("\n\t\t\t  ERROR\n\n");
						printf("  Recuerde que debe elegir una opcion en el rango [0-1]\n\n  ");
						system("pause");
					}
				}
			break;
			case '6':
				while(op2[0]!='0'){
					system("cls");
					encabezado();
					printf("\n\n---------------------------------------------------");
					printf( "\n  Manejo virtual de directorios (MVA) ");
					printf( "\n  [6. Modificar directorio (MDD)]");
					printf("\n---------------------------------------------------\n");
					printf("\n  ");printv2(aux,1);printf("]\n\n");
					printf("  1. Nombre [%s]\n",aux->nom);
					printf("  2. Fecha [%s]\n",aux->fcm);
					printf("  3. Lectura [%i]\n",aux->r);
					printf("  4. Escondido [%i]\n",aux->h);
					printf("  0. Volver a pantalla principal\n");
					printf("  Su opcion [0-4]: ");
					scanf("%s",&op2);
					if(validar(1,op2)){
						if(op2[0]=='0'||op2[0]=='1'||op2[0]=='2'||op2[0]=='3'||op2[0]=='4') modificar(aux,p,op2[0]);
						else {
								printf("\n\t\t\t  ERROR\n\n");
								printf("  Recuerde que debe elegir una opcion en el rango [0-4]\n\n  ");
							}
						system("pause");
					} else {
						printf("\n\t\t\t  ERROR\n\n");
						printf("  Recuerde que debe elegir una opcion en el rango [0-4]\n\n  ");
						system("pause");
					}
				}
			break;
			case '7':
				while(op2[0]!='0'){
					system("cls");
					encabezado();
					printf("\n\n---------------------------------------------------");
					printf( "\n  Manejo virtual de Archivos (MVA) ");
					printf( "\n  [7. Mostrar directorio (SHD)]");
					printf("\n---------------------------------------------------\n");
					printf("\n  ");printv2(aux,1);printf("]\n\n");
					printf("  1. Contenidos del directorio\n");
					printf("  2. Contenidos del directorio con subcarpetas\n");
					printf("  3. Archivos escondidos [%i]\n", aux->h);
					printf("  0. Volver a la pantalla principal\n");
					printf("  Su opcion [0-1]: ");
					scanf("%s",&op2);
					if(validar(1,op2)){
						switch (op2[0]){
							case '0':
								printf("\n\n  ");
							break;
							case '1':
								mostrar(aux, 0);
							break;

							case '2':
								mostrar(aux,2);
							break;
							case '3':
									mostrar(aux, 1);
							break;
							default:
									printf("\n\t\t\t  ERROR\n\n");
									printf("  Recuerde que debe elegir una opcion en el rango [0-3]\n\n  ");

							break;
						}
						system("pause");
					} else {
						printf("\n\t\t\t  ERROR\n\n");
						printf("  Recuerde que debe elegir una opcion en el rango [0-3]\n\n  ");
						system("pause");
					}
				}
			break;
			default:
					printf("\n\n\t\t\t  ERROR\n\n");
					printf("  Recuerde que debe elegir una opcion en el rango [0-7]\n\n  ");
					system("pause");
			break;
		}
		strcpy(op2,"-1");
	} else {
		printf("\n\n\t\t\t  ERROR\n\n");
		printf("  Recuerde que debe elegir una opcion en el rango [0-7]\n\n  ");
		system("pause");
	}
}
guardardirectorios(&fp, p, 0);
};*/