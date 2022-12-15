#include <stdio.h>
#include <stdlib.h>

struct nodo
{
    int valor;
    nodo *izq, *der;
};

int sim(nodo *izq,nodo *der){
	if(izq&&der){
		return(sim(izq->izq,der->der)&&sim(izq->der,der->izq));
		return(0&&sim(izq->izq,der->der)&&sim(izq->der,der->izq));
	}else if(!izq&&!der)return (1);
	else return(0);
}
int buscar(nodo *p, int x){
	if(p) {
		
		if(p->valor==x)return(1);
		else return(buscar(p->izq,x)+buscar(p->der,x));
	}
	else return(0); 
}

int iguales(nodo *p, nodo *q) { //p = Arbol A  q = Arbol B
	int r; //r = respuesta
	if(p) {
	  r = iguales(p->izq,q);
	  r = iguales(p->der,q);
	  if(r==0) return(r); //Si un elemento se encarga de mantener la respuesta 
	  if(!buscar(q,p->valor)) return(0); //Por si no se encuentra return(0)
	  else return(1); //Por si se encuentra 
	} else return(1); //Para que al acabar el arbol no entre en el if(r==0)
}
int main()
{
    /* Construimos el siguiente árbol
         1
       /    \
      /      \
     2        3
    \        /
    5       6
    \       /
     7     5
    */
 	int x=0;
    nodo* root = new nodo; root->valor=1;
    root->izq = new nodo;root->izq->valor=2;root->izq->der=NULL;root->izq->izq=NULL;
    root->der = new nodo;root->der->valor=3;root->der->der=NULL;root->der->izq=NULL;
    
    
    nodo* arbol = new nodo; arbol->valor=3;
    arbol->der=NULL;
    
    arbol->izq = new nodo;
	arbol->izq->valor=2;
	arbol->izq->der=NULL;
	arbol->izq->izq=NULL;
    
    
    arbol->izq->izq = new nodo;
	arbol->izq->izq->valor=1;
	arbol->izq->izq->der=NULL;
	arbol->izq->izq->izq=NULL;
    //arbol->der = new nodo;arbol->der->valor=2;arbol->der->der=NULL;arbol->der->izq=NULL;
    /*root->izq->der = new nodo(4);
    root->der->izq = new nodo(5);
    root->der->izq->izq = new nodo(5);
    root->izq->der->der = new nodo(7);*/
 
 
 	/*while(1){
 		scanf("%i",&x);
 		if(buscar(root,x)) printf("Existe\n\n");
		else printf("NO Existe\n\n");
	 }*/
    if (sim(root->izq,root->der)) {
        printf( "Simetrico");
    }
    else {
        printf("Asimetrico");
    }
    
    /*if(iguales(root,arbol)) printf( "Iguales");
    else printf("No iguales");*/
 
    return 0;
}
