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
	char *nom;
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
	memset(straux, '\0', sizeof(straux));
	strcpy(nombre, nom);
	strncpy(straux, ruta, lenRuta);
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
			if (!strcmp(t, "."))
			{
				t = strtok(NULL, "/ \n");
				p = moverpunterov3(t, p, 0,y);
				return (p);
			}
			else if (!strcmp(t, ".."))
			{
				t = strtok(NULL, "/ \n");
				p = p->ppa;
				p = moverpunterov3(t, p, 0,y);
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
							p = moverpunterov3(t, p, 0,y);
							return (p);
						}
						p = p->pul;
					}
					printf("ERROR: Ruta no encontrada\n");
					return (NULL);
				}
				else
				{
					printf("ERROR: Ruta no encontrada\n");
					return (NULL);
				}
			}
		}
		else
		{
			printf("ERROR: Ruta no encontrada\n");
			return (NULL);
		}
	}
	else
	{	
		if(y==1){
			if (p&&p->ppa)
				return (p);
			else
			{
				printf("ERROR: Fuente no encontrada\n");
				return (NULL);
			}	
		} else if(y==2){
			if (p&&p->ppa)
				return (p);
			else
			{
				printf("ERROR: Destino no encontrado\n");
				return (NULL);
			}	
		} else if (y==0) {
			if (p&&p->ppa)
				return (p);
			else
			{
				printf("ERROR: Ruta no encontrada\n");
				return (NULL);
			}
		} else {
			return(p);
		}
	}
}



//funcones de impresion
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
//fin funcion de imprimir ubicacion actual

//Funciones para mostrar directorios
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
			if (aux->h == 1) printf("|%-8s       <%c>     %s     -r+h\n", aux->nom, aux->tip, aux->fcm);
			else printf("|%-8s       <%c>     %s     -r\n", aux->nom, aux->tip, aux->fcm);
		}
		else
		{
			if (check == 1)
			{
				for (int i = 0; i != saltos; i++)
					printf("|%*c", -10, 32);
			}
			if (aux->h == 1) printf("|%-8s       <%c>     %s     +r+h\n", aux->nom, aux->tip, aux->fcm);
			else printf("|%-8s       <%c>     %s     +r\n", aux->nom, aux->tip, aux->fcm);
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
					printf("|%-8s       <%c>     %s     -r\n", aux->nom, aux->tip, aux->fcm);
				else
					printf("|%-8s       <%c>     %s     +r\n", aux->nom, aux->tip, aux->fcm);
				break;
			}
			aux = aux->pul;
		}
	}
}
//Fin funciones de impresion de directorios
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
int mayus(char **x){
		if((*x[0] > 64) && (*x[0] < 91)) return (1);
		else if ((*x[0] > 96) && (*x[0] < 123)) {
			*x[0] = *x[0]-32;
			return (1);
		}
		return(0);
}
// fincion para verficar si el token de ruta inicia con la unidad
int verificartoken(char *ruta)
{
	char x[1024], *t;
	strcpy(x, ruta);
	t = strtok(x, "/");
	if(strlen(x)==1) return (0);
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
//fin moverpuntero

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
	if (p&&p->ppa)
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
								printf("ERROR: Existe un directorio de igual nombre en la ubicacion actual\n\n  ");
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
		else if (strcmp(p->ppa->nom, "root"))
			printf("ERROR: El directorio actual solo permite la lectura\n");
		else
			printf("ERROR: La unidad logica solo permite la lectura\n");
	}
	else
		printf("ERROR: Acceso denegado\n");
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
				switch (op[0])
				{
					case 'S':
						if (!comprobar(p->pfa))
						{ // comprobamos si algun hijo esta protegido, si lo esta no se realiza el borrado
							if (contenido(*ax, &p)) while(*ax&&(*ax)->ppa&&(*ax)->ppa->ppa) *ax=(*ax)->ppa;
							borrartodo(p->pfa);
							darfecha(&p->ppa);
							dir *t = p;
							link(t);
							delete (p);
							return;
						}
						else printf("Error: El directorio contiene archivos protegidos\n");
					break;
					case 'N':
						return;
					break;
					case 's':
						if (!comprobar(p->pfa))
						{ // comprobamos si algun hijo esta protegido, si lo esta no se realiza el borrado
							if (contenido(*ax, &p)) while(*ax&&(*ax)->ppa&&(*ax)->ppa->ppa) *ax=(*ax)->ppa;
							borrartodo(p->pfa);
							darfecha(&p->ppa);
							dir *t = p;
							link(t);
							delete (p);
						}
						else printf("Error: El directorio contiene archivos protegidos\n");
						return;
					break;
					case 'n':
						return;
					break;			
				}
			}
		}
	}
	else if(x==1)
	{
		if (contenido(*ax, &p)) while(*ax&&(*ax)->ppa&&(*ax)->ppa->ppa) *ax=(*ax)->ppa;
		borrartodo(p->pfa);
		darfecha(&p->ppa);
		dir *t = p;
		link(t);
		delete (p);
		
	} else if(x==2){
		while (1)
		{	
			printf("%cEsta seguro que desea vaciar la unidad logica %s? (S,N): ", 168,p->nom);
			scanf("%s", &op[0]);
			fflush(stdin);
			if (validar(1, op))
			{	
			switch (op[0])
				{
				case 'S':
					if (contenido(*ax, &p)) while(*ax&&(*ax)->ppa&&(*ax)->ppa->ppa) *ax=(*ax)->ppa;
					borrartodo(p->pfa);
					darfecha(&p);
					return;
				break;
				case 'N':
					return;
				break;
				case 's':
					if (contenido(*ax, &p)) while(*ax&&(*ax)->ppa&&(*ax)->ppa->ppa) *ax=(*ax)->ppa;
					borrartodo(p->pfa);
					darfecha(&p);
					return;
				break;
				case 'n':
					return;
				break;
				}	
				
			}
		}
		
	}
}
// fin borrar dentro

// funcion que contempla los casos borde y ubica el puntero
//p = puntero en root
//ax = direccion actual
//ruta = ruta
//op = para borrar 
void borrar(dir *p, dir **ax, char *ruta, int op)
{
		if (verificartoken(ruta))
			p = moverpunterov3(ruta, p, 1,0); // ruta absoluta
		else
			p = moverpunterov3(ruta, *ax, 1,0); // Ruta relativa
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


void mclovin(dir *ax, dir *bx, dir *p, dir *q){
	dir *t;int c = 0;
	while(ax) {
	     bx = p->pfa;
	     while(bx) {
	        if (!strcmp(ax->nom,bx->nom)){
		        mclovin(ax->pfa,bx->pfa,bx,ax);
				c=1;
			}
			if(bx->pul==NULL){
				break;
			}	
			bx=bx->pul;	
	     }
	     	//El problema reside aqui 
	        if(c==0){
	        	t=ax;
	        	link(t);
	        	ax = ax->pul;
	         	t->pul=NULL;
	         	if(bx!=NULL) {
	        		bx->pul=t;
	         		t->ppa=bx->ppa;
				} else {
					p->pfa=t;
					t->ppa=p;	
				}
	       	} else ax = ax->pul;	    
	}	
}
			
			
//LEYENDA
//p = destino
//ax = fuente
//bx = ubicacion actual
//x = mover/copiar
//op opcion			
void sobreescribir(dir **p, dir **ax, dir **bx, int x, int op) 
{

	char opc[10] = "-1";
	dir *g;
	if(op==0){
		while (1)
		{
			printf("%cExiste un directorio de igual nombre en la ubicacion destino desea sobreescribir los datos? (S,N): ",168);
			//Se fusionan ambas carpetas (mclovinizan) 
			scanf("%s", &opc[0]);
			fflush(stdin);
			if (validar(1, opc)){
				switch (opc[0]) {
					case 'S':
							while (1){
								printf("%cDesea sobreescribir (S) o unir (U)? (S,N): ",168);
								scanf("%s", &opc[0]);
								fflush(stdin);
								if(validar(1,opc)){
									switch (opc[0]) {
										case 'U':	
											if(!comprobar((*p)->pfa)){
												copiartodo(*ax, 1, &g);
												if(g->pfa){
													mclovin(g->pfa,(*p)->pfa,*p,g);
													borrartodo(g->pfa);
													darfecha(p);
												}
												delete(g);	
											} else printf("Error: El destino contiene archivos protegidos\n"); //NOTA PREGUNTAR SI PARA /M SE CONSIDERA SI ALGUN DIRECTORIO TIENE ARCHIVOS PROTEGIDOS Y QUE HACER EN ESE CASO
											return;
										break;
										case 'u':	
											if(!comprobar((*p)->pfa)){
												copiartodo(*ax, 1, &g);
												if(g->pfa){
													mclovin(g->pfa,(*p)->pfa,*p,g);
													borrartodo(g->pfa);
													darfecha(p);
												}
												delete(g);	
											} else printf("Error: El destino contiene archivos protegidos\n"); //NOTA PREGUNTAR SI PARA /M SE CONSIDERA SI ALGUN DIRECTORIO TIENE ARCHIVOS PROTEGIDOS Y QUE HACER EN ESE CASO
											return;
										break;
										case 'S':
											if(!comprobar((*p)->pfa)){
												copiartodo(*ax, 1, &g);
												g->pul = (*p)->pul;
												g->ppa = (*p)->ppa;
												darfecha(&g->ppa);
												g->r = 0;
												darfecha(&g);
												linksobreescribir(*p, g);
												if (contenido(*bx, p)) *bx=(*p)->ppa;
												borrartodo((*p)->pfa);
												delete (*p);	
											} else printf("Error: El destino contiene archivos protegidos\n");
											return;
										break;
										case 's':
											if(!comprobar((*p)->pfa)){
												copiartodo(*ax, 1, &g);
												g->pul = (*p)->pul;
												g->ppa = (*p)->ppa;
												darfecha(&g->ppa);
												g->r = 0;
												darfecha(&g);
												linksobreescribir(*p, g);
												if (contenido(*bx, p)) *bx=(*p)->ppa;
												borrartodo((*p)->pfa);
												delete (*p);	
											} else printf("Error: El destino contiene archivos protegidos\n");
											return;
										break;
									}
								}
							}	
					break;
					case 'N':
					default:
					break;
				}
			}
		}
	} else if(op==1) {
		//////////////////////////////////////////////////////////////////////////////////////////
		// OPCION /O (COPIAR Y MOVER)
		///////////////////////////////////////////////////////////////////////////////////
		if (x == 1) //Mover
		{
			darfecha(&(*ax)->ppa);
			link(*ax);
			(*ax)->pul = (*p)->pul;
			(*ax)->ppa = (*p)->ppa;
			darfecha(&(*ax)->ppa);
			linksobreescribir(*p, *ax);
		} else {
			copiartodo(*ax, 1, &g);
			g->pul = (*p)->pul;
			g->ppa = (*p)->ppa;
			darfecha(&g->ppa);
			g->r = 0;
			darfecha(&g);
			linksobreescribir(*p, g);
		}
		if (contenido(*bx, p)) *bx=(*p)->ppa;
		borrartodo((*p)->pfa);
		delete (*p);
		//FIN OPCION /0
		/////////////////////////////////
		
	} else if(op==2){
		//AQUI VA MCLOVIN (OPCION /m)
		if(!comprobar((*p)->pfa)){
			copiartodo(*ax, 1, &g);
			if(g->pfa){
				mclovin(g->pfa,(*p)->pfa,*p,g);
				borrartodo(g->pfa);
				darfecha(p);
			}
			delete(g);
		} else printf("Error: El destino contiene archivos protegidos\n");
		//OPCION (/m copiar fusionando)
	}
}
// fin sobreescribir

// funcion que ubica al puntero y contempla los casos posibles para mover
// Comprueba ademas si el directorio al cual se desea mover tiene un directorio de igual nombre al directorio que movemos
// Si es asi se envia a la funcion de arriba (sobreescribir)
void mover(dir *p, dir **ax, char *fuente, char *dest, int op)
{
	dir *d=p,*g;
	if (verificartoken(fuente))
		p = moverpunterov3(fuente, p, 1,1); // ruta absoluta
	else
		p = moverpunterov3(fuente, *ax, 1,1); // Ruta relativa
	
	if (verificartoken(dest))
		d = moverpunterov3(dest, d, 1,2); // ruta absoluta
	else
		d = moverpunterov3(dest, *ax, 1,2); // Ruta relativa
	if(p){ //Comprobar Si la fuente existe
		if(d){ //Comprobar si destino existe
			if (p->ppa && p->ppa->ppa){ //Comprobar si la fuente es una unidad logica (revisar)
				if (d->r != 1 || op){ //Comprobar si el destino es solo lectura
					if (d->r != 1 || op) {//Comprobar si la fuente es solo lectura
						if(!(p==d)){ //Por si el directorio ya esta en su posicion
							if(!contenido(d,&p)) { //Comrprobar si no se desea mover dentro de si mismo
								if (d->pfa)
								{
									d = d->pfa;
									while (d)
									{
										if (!strcmp(p->nom, d->nom))
										{
											if(op) sobreescribir(&d, &p, ax,1,op);
											else printf("ERROR: Existe un directorio de igual nombre en el destino\n");
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
							} else printf("ERROR: Desea mover el directorio dentro de si mismo\n"); //Por si se quiere mover un dir dentro de si mismo
						} else printf("ERROR: El directorio ya se encuentra en la posicion deseada\n"); //Por si se encuentra en su posiicion //Preguntar si es necesario mostrar el msg
					} else printf("ERROR: La fuente solo permite la lectura\n"); //Destino solo permite la lectura
				} else printf("ERROR: El destino solo permite la lectura\n"); //Destino solo permite la lectura
			} else printf("ERROR: La unidad logica [%s/] no puede ser movida\n", p->nom); //Por si la fuente es una unidad logica
		} 
	} 	

}

// copiar
void copiar(dir *p, dir **ax, char *fuente, char *dest, int op)
{
	dir *d=p,*g;
	if (verificartoken(fuente))
		p = moverpunterov3(fuente, p, 1,1); // ruta absoluta
	else
		p = moverpunterov3(fuente, *ax, 1,1); // Ruta relativa
	
	if (verificartoken(dest))
		d = moverpunterov3(dest, d, 1,2); // ruta absoluta
	else
		d = moverpunterov3(dest, *ax, 1,2); // Ruta relativa
	if(p){ //Comprobar Si la fuente existe
		if(d){ //Comprobar si destino existe
				if (p->ppa && p->ppa->ppa){ //Comprobar si la fuente es una unidad logica (revisar)
					if(!(p==d)) //Por si desea copiar el directorio dentro de si
					{
						if (d->r != 1 || op) //Comprobar si el destino es solo lectura
						{
							if (d->pfa)
							{
								d = d->pfa;
								while (d)
								{
									if (!strcmp(p->nom, d->nom))
									{
										sobreescribir(&d, &p, ax, 0,op); //RECORDAR PASARLE LOS 50 PARAMETROS
										return; // Indica que existe directorio de igual nombre
									}
									if (d->pul == NULL) break;
									d = d->pul;
								}
								copiartodo(p, 1, &g);
								p->pul = g;
								g->pul = NULL;
								g->ppa = d->ppa;
								g->r = 0;
								darfecha(&g->ppa);
								darfecha(&g);
							} else {
								copiartodo(p, 1, &g);
								g->ppa = d;
								d->pfa = g;
								g->pul = NULL;
								g->r = 0;
								darfecha(&g->ppa);
								darfecha(&g);
							}
						} else printf("ERROR: El destino solo permite la lectura\n"); //Destino solo permite la lectura
					} else printf("ERROR: No se puede copiar un directorio en si mismo\n"); //Por si se desea copiar un directorio dentro de si mismo
				} else printf("ERROR: La unidad logica [%s/] no puede ser copiada\n", p->nom); //Por si la fuente es una unidad logica
		} //Por si el destino no existe
	} //Por si la fuente si la fuente no existe	
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
		p = moverpunterov3(ruta, p, 1,0);
	else
		p = moverpunterov3(ruta, *ax, 1,0);
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
			printf("ERROR: Comando invalido\n");
	}
	else
		borrar(p, ax, ruta, 0);
}

void CU(dir *p, char *nom)
{
	dir *ax, *t = new dir;
	if(mayus(&nom)){
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
	} else printf("ERROR: Nombre invalido para una unidad logica\n");
}


void CRU(dir *p, dir *ax, char *rutadest)
{
	char ruta[512], nom[24];
	if (separaRuta(rutadest, nom, ruta))
	{
		if (verificartoken(ruta))
			p = moverpunterov3(ruta, p, 1,3);
		else
			p = moverpunterov3(ruta, ax, 1,3);
		if (p && !p->ppa) {
			if(verificartoken(nom)){
					CU(p,nom);	
			} else if(validar(1,nom)){
					strcat(nom,":");
					CU(p,nom);
			} else printf("ERROR: Nombre invalido para una unidad logica\n");		
		} else if (p->ppa)("ERROR: Ruta invalida");
	}
	else
	{
		if(verificartoken(nom)){
				CU(p,nom);	
			} else if(validar(1,nom)){
				strcat(nom,":");
				CU(p,nom);
		} else printf("ERROR: Nombre invalido para una unidad logica\n");
	}
}

void FRU(dir *p, dir **ax, char *rutadest)
{
	if (verificartoken(rutadest))
		p = moverpunterov3(rutadest, p, 1,0);
	else
		p = moverpunterov3(rutadest, *ax, 1,0);
	if(p&&p->ppa) 
		if(!p->ppa->ppa) {
			borrardentro(p, ax,2);
		}  else printf("ERROR: Ruta invalida\n");	
}

void CPD(dir *p, dir **ax, char *fuente, char *dest, char *op){
	if (op)
	{ // Validacion de si la opcion existe
		if (!strcmp(op, "/o"))
			copiar(p, ax, fuente, dest, 1);
		else if (!strcmp(op, "/m"))
			copiar(p, ax, fuente, dest, 2);
		else printf("ERROR: Comando invalido\n");	
	} else copiar(p, ax, fuente,dest, 0);	
}

void MVD(dir *p, dir **ax, char *fuente, char *dest, char *op){
	if (op)
	{ // Validacion de si la opcion existe
		if (!strcmp(op, "/o")) 
			mover(p, ax, fuente, dest, 1);
		else printf("ERROR: Comando invalido\n");	
	} else mover(p, ax, fuente,dest, 0);	
}


void SHD(dir *p, dir **ax, char *ruta, int s, int h){
	if (verificartoken(ruta))
		p = moverpunterov3(ruta, p, 1,0); // ruta absoluta
	else
		p = moverpunterov3(ruta, *ax, 1,0); // Ruta relativa

	if(p){
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
			if(h) mostrartodo(p, 0, 0);
			else mostrarcasitodo(p, 0, 0);
		}
		printf("\n\n");
	}

}

int main()
{
	dir *q = new dir, *p = new dir, *ax = p; FILE *fp; //Punteros
	char raw[1024], *t, *ordenado[6]; //Auciliares del Menu
	int op = -1, i; //Auxiliares numericas del menu
	q->ppa = NULL;
	q->pfa = p;
	///////////////////////////////////////////////////////////////////////////////////
	p->tip = 'U';p->h = 0;p->r = 0;darfecha(&p);p->ppa = q;p->pfa = NULL;p->pul = NULL;strcpy(p->nom,"C:");
	//////////////////////////////////////////////////////////////////////////////////
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
	//////////////////////////////////////////Aqui comienza el menu///////////////////////////
	while (op)
	{
		for (int j = 0; j < 6; j++)
			ordenado[j] = NULL;
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

			if (separaRuta(ordenado[1], nombre, ruta))
			{
				if (verificartoken(ruta))
					auxRoot = moverpunterov3(ruta, q, 1,0);
				else
					auxRoot = moverpunterov3(ruta, ax, 1,0);
				if (auxRoot)
					crear(auxRoot, nombre, h, r);
			}
			else
			{
				crear(ax, nombre, h, r);
			}
		}

		else if (!(strcmp(ordenado[0], "CHD")))
		{
			if (i>2 || !ordenado[1])
				printf("ERROR: Comando invalido\n");
			else
				CHD(q, &ax, ordenado[1]);
		}
		else if (!(strcmp(ordenado[0], "RMD")))
		{
			if (!ordenado[1] || i>3)
				printf("ERROR: Comando invalido\n");
			else
				RMD(q, &ax, ordenado[1], ordenado[2]);
		}
		else if (!(strcmp(ordenado[0], "CPD")))
		{
			if(!ordenado[1]||!ordenado[2])  printf("ERROR: Comando invalido\n");
			else CPD(q,&ax,ordenado[1],ordenado[2],ordenado[3]);	
		}
		else if (!(strcmp(ordenado[0], "MVD"))){
			if(!ordenado[1]||!ordenado[2]) printf("ERROR: Comando invalido\n");
			else MVD(q,&ax,ordenado[1],ordenado[2],ordenado[3]);
		
		}
		else if (!(strcmp(ordenado[0], "MDD")))
		{
			dir *auxRoot = NULL;
			char *param, *valor;
			if (ordenado[1] && ordenado[2])
			{
				if (verificartoken(ordenado[1]))
					auxRoot = moverpunterov3(ordenado[1], q, 1,0);
				else
					auxRoot = moverpunterov3(ordenado[1], ax, 1,0);
				param = strtok(ordenado[2], ":");
				valor = strtok(NULL, ":");
				if (!(strcmp(param, "/n")))
				{
					if (strlen(valor) < 8)
						strcpy(auxRoot->nom, valor);
					else
						printf("El nombre debe ser menor a 8 caracteres");
				}
				else if (!(strcmp(param, "/r")))
				{
					if (!(strcmp(valor, "0")))
						auxRoot->r = 0;
					else if (!(strcmp(valor, "1")))
						auxRoot->r = 1;
					else
						printf("El valor debe ser 0 o 1");
				}
				else if (!(strcmp(param, "/h")))
				{
					if (!(strcmp(valor, "0")))
						auxRoot->h = 0;
					else if (!(strcmp(valor, "1")))
						auxRoot->h = 1;
					else
						printf("El valor debe ser 0 o 1");
				}
			}
			else
			{
				printf("ERROR: Comando invalido");
			}
		}
		else if (!(strcmp(ordenado[0], "SHD")))
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
				if (!ordenado[1] && !ordenado[3] && !ordenado[3]){
					  printv2(p);
					  printf("\n");
					  mostrardirectorio(ax, 0);
					  printf("\n\n");
				}else if ((strcmp(ordenado[1], "/s")) && (strcmp(ordenado[1], "/h"))){
					SHD(q, &ax, ordenado[1], s, h);
				}else{
					printv2(p);
					printf("\n");
					if((ordenado[1]) && (ordenado[2])){
						mostrartodo(ax, 0, 0);
					}else{
						if(!strcmp(ordenado[1], "/h")) mostrardirectorio(ax, h);
						else if(!strcmp(ordenado[1], "/s")) mostrarcasitodo(ax, 0, 0);					  
					}
					printf("\n\n");

				}
		}
		else if (!(strcmp(ordenado[0], "CSC")))
		{
			system("cls");
		}
		else if (!(strcmp(ordenado[0], "CRU")))
		{
			if ((ordenado[2] || ordenado[3]) || !ordenado[1])
				printf("ERROR: Comando invalido\n");
			else
				CRU(q, ax, ordenado[1]);
		}
		else if (!(strcmp(ordenado[0], "SRU")))
		{
		}
		else if (!(strcmp(ordenado[0], "LRU")))
		{
		}
		else if (!(strcmp(ordenado[0], "FRU")))
		{
			if (i>2 || !ordenado[1])
				printf("ERROR: Comando invalido\n");
			else
				FRU(q, &ax, ordenado[1]);
		}
		else if (!(strcmp(ordenado[0], "ERU")))
		{
		}
		else if (!(strcmp(ordenado[0], "EXIT")))
		{
			if (ordenado[1] || ordenado[2] || ordenado[3])
				printf("ERROR: Comando invalido\n");
			else
				op = 0;
		}
		else
		{
			printf("ERROR: Comando invalido\n");
		}
		// lo lee, funciona bien
	}
}