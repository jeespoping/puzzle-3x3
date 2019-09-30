#include <iostream>
#include<stdlib.h>
#include<time.h>
#include <conio.h>
#include <allegro.h>

using namespace std;
typedef struct arcos *siga; 
typedef struct nodos{
        
        int matriz[3][3];
        
        
		nodos *sig;
        arcos *siga;
};
nodos *ptr=NULL;
typedef struct arcos {
		int costo;
		static bool estado;
		
        arcos *sigb;
        nodos *sign;
};
/* Tener en cuenta:
	- 0 es el espacio donde pueden mover la ficha
*/
int Solucion[3][3] ={
{1,2,3},
{4,5,6},
{7,8,0}
};
int pasoos=0;
int pasos=0;
// Genera juego inicial
void BuscarJuegoInicial();
bool Ingreselo(int num);
int Inversiones(int v[3][3]);

// Movimientos del juego
bool MoverMatriz(int m[3][3], string direccion); // une las 4 funciones de abajo
bool MovDerecha(int m[3][3]);
bool MovIzquierda(int m[3][3]);
bool MovArriba(int m[3][3]);
bool MovAbajo(int m[3][3]);

// Ingreso de un nuevo nodo
void InsertarNodo(int origen[3][3], string direccion);
void CopiarMatriz(int origen[3][3], int destino[3][3]);
void ImprimirMatriz(int m[3][3]);
nodos * BuscarVertice(int m[3][3]);
bool SonIguales(int m[3][3], int n[3][3]);

// Generar solucion 
bool GenerarSolucion();
// Heurisiticas y costo
int costo(int m[3][3]);
int distancia(int m[3][3], int ficha);
int FichasFueraDeLugar(int a[3][3]);
int RegresosDirectos(int a[3][3]);
string MenorOrdenLexicografico(int a[9],int b[9]);
void EliminarTodo();
int nHijos(nodos *p);

arcos* MejorRuta(nodos* h);
bool EstaIngresadoEnGrafo(nodos *h, int m[3][3]);
nodos * NodoAnterior(nodos *r);




// Allegro:
BITMAP *buffer;
BITMAP *fondo;
BITMAP *cero;
BITMAP *uno;
BITMAP *dos;
BITMAP *tres;
BITMAP *cuatro;
BITMAP *cinco;
BITMAP *seis;
BITMAP *siete;
BITMAP *ocho;
BITMAP *nueve;
BITMAP *cargando;
BITMAP *fondoSolucion;
BITMAP *cursor;
BITMAP *ganaste;

//int m[3][3] = {{1,2,3},{4,5,6},{7,8,0}};
void dibujarMatriz(int m[3][3]);
void pantalla();
void MostrarSolucion(nodos *r);
int main() 
{	
    srand(time(NULL));

	allegro_init();
	install_keyboard();
	
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 700, 0, 0);
	
	buffer = create_bitmap(880, 700);
	cero = load_bitmap("imagenes/casillas/cero.bmp",NULL);
	uno = load_bitmap("imagenes/casillas/uno.bmp",NULL);
    dos = load_bitmap("imagenes/casillas/dos.bmp",NULL);
    tres = load_bitmap("imagenes/casillas/tres.bmp",NULL);
    cuatro = load_bitmap("imagenes/casillas/cuatro.bmp",NULL);
    cinco = load_bitmap("imagenes/casillas/cinco.bmp",NULL);
    seis = load_bitmap("imagenes/casillas/seis.bmp",NULL);
    siete = load_bitmap("imagenes/casillas/siete.bmp",NULL);
    ocho = load_bitmap("imagenes/casillas/ocho.bmp",NULL);
    fondo = load_bitmap("imagenes/fondo.bmp",NULL);
    cargando = load_bitmap("imagenes/cargando.bmp",NULL);
    fondoSolucion = load_bitmap("imagenes/solucion.bmp",NULL);
    ganaste = load_bitmap("imagenes/ganaste.bmp",NULL);
  
    bool TieneSolucion = false;
	while(!TieneSolucion){
        draw_sprite(buffer, cargando, 0, 0);
        pantalla();
		ptr=NULL;
		BuscarJuegoInicial();
		TieneSolucion=GenerarSolucion();
	}
	
	int MatrizParaJugar[3][3];
	CopiarMatriz(ptr->matriz,MatrizParaJugar);
	nodos* aux = NULL;
	bool verSolucion=false;
	int MovimientosJug=0;
	while(!key[KEY_ENTER]){
      
       
       if(key[KEY_RIGHT]){
         if( MoverMatriz(MatrizParaJugar,"derecha") ) MovimientosJug++;
          
       }
       if(key[KEY_LEFT]){
          if( MoverMatriz(MatrizParaJugar,"izquierda") ) MovimientosJug++;
           
       }
       if(key[KEY_UP]){
           if( MoverMatriz(MatrizParaJugar,"arriba") ) MovimientosJug++;
       }
       if(key[KEY_DOWN]){
           if( MoverMatriz(MatrizParaJugar,"abajo") ) MovimientosJug++;
       }
       
     
       draw_sprite(buffer, fondo, 0, 0);
       dibujarMatriz(MatrizParaJugar);
       pantalla();
        text_mode(-1); // Coloca la fuente trasparente.
                       
         textprintf(screen, font,220,103, makecol(500,0,0),"Numero de Movimientos: %d ",MovimientosJug);
     
      if(SonIguales(MatrizParaJugar,Solucion)){
         draw_sprite(buffer, ganaste, 0, 0);
           pantalla();                                    
      }
      
       clear_keybuf();
       readkey();
       
       if(key[KEY_ENTER]){
         verSolucion = true;
         break;
         }
      
    }
    if(verSolucion){
       MostrarSolucion(BuscarVertice(Solucion));
    }
    readkey();
    readkey();
}
END_OF_MAIN();
void pantalla(){
               
     blit(buffer, screen, 0,0,0,0,880,700); // Fondo
}
void dibujarMatriz(int m[3][3]){
     int distX = 20;
     int distY = 170;
     for(int x=0; x<3; x++){
             for(int y=0; y<3; y++){
                     if( m[y][x] == 0){
                         draw_sprite(buffer, cero, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 1){
                         draw_sprite(buffer, uno, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 2){
                         draw_sprite(buffer, dos, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 3){
                         draw_sprite(buffer, tres, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 4){
                         draw_sprite(buffer, cuatro, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 5){
                         draw_sprite(buffer, cinco, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 6){
                         draw_sprite(buffer, seis, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 7){
                         draw_sprite(buffer, siete, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 8){
                         draw_sprite(buffer, ocho, x*100+distX, y*100+distY);
                     }
                     if( m[y][x] == 9){
                         draw_sprite(buffer, nueve, x*100+distX, y*100+distY);
                     }
                    
                     }
     }
}



nodos *InsertarNodo(string direccion){
    nodos *ultimo = NULL;
    nodos *r= (struct nodos *) malloc (sizeof (nodos));
	nodos* q = ptr;
	while(q->sig != NULL){
		q = q->sig;
	}
   
    CopiarMatriz( q->matriz, r->matriz );
    
  	if(MoverMatriz(r->matriz,direccion)){	  
	  	// Enlazar en el grafo <InsertarPorCola>	
		r->siga = NULL;
	    r->sig = NULL;
	
	    q->sig = r;
	    arcos *p = (struct arcos *) malloc (sizeof (arcos));
	    p->sigb = NULL;
	    p->sign = r;
	    q->siga = p;
	    ultimo = r;
	}
	return ultimo;
}
    
int Inversiones(int v[3][3]){
	int m[9];
		int k=0;
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				m[k] = v[i][j];
				k++;
			}
		}
	
	int cantidad = 0;
	for(int i=0; i<9; i++){
		for(int j=i+1; j<9; j++){
			if(m[i]!=0 && m[j]!=0){
					if( m[i]!=0 && m[j] && m[i] > m[j] ){
					cantidad++;	
				}
				
			}
		
		//else if( m[i] == 0 && i%2 == 1) cantidad++;		
		}
	}
	return cantidad;
}
void EliminarTodo(){
	nodos *p = ptr;
	nodos *antV = NULL;
	arcos *antA = NULL;
	while(p!=NULL){
		free(antV);
		arcos *x = p->siga;
		while(x!=NULL){
			free(antA);
			antA=x;
			x=x->sigb;
		}
		antV=p;
		p=p->sig;
	}
}
void BuscarJuegoInicial(){
	ptr = (struct nodos *) malloc (sizeof (nodos));
	ptr->sig=NULL;
	ptr->siga=NULL;
/*	int inicial[3][3] = {
		{0,5,3},
		{7,4,6},
		{1,2,8}
	};
	CopiarMatriz(inicial,ptr->matriz);
	*/
	// Genera el juego inicial
	bool JuegoConSolucion = false;
	int inversiones=0;
	while(JuegoConSolucion == false){
		// inicializar en -1
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				ptr->matriz[i][j] = -1;
			}
		}
		
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				srand(time(NULL));
				int num= rand() %9;
				while( !Ingreselo(num) ){
					 num=rand()%9;
				}
				ptr->matriz[i][j] = num;
			}
		} 
		
		
		inversiones = Inversiones(ptr->matriz);
		//cout<<"La cantidad de inversiones es "<<inversiones<<endl;

		if( inversiones%2 == 0){
				if(costo(ptr->matriz) < 20){
					JuegoConSolucion = true;
					break;
				}
		}
		
	} 
//	cout<<"La cantidad de inversiones es "<<inversiones<<endl;	
}

bool Ingreselo(int num){
	// Valida si un numero ya existe dentro de la matriz inicial
	bool r = true;
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			if( ptr->matriz[i][j] == num){
				r = false;
				break;
			}
		}
	}
	return r;
}
void InsertarNodo(int origen[3][3], string direccion){
    
    nodos *y= (struct nodos *) malloc (sizeof (nodos));
    CopiarMatriz( origen, y->matriz );
    
    if( MoverMatriz(y->matriz,direccion) ){	   // Si el movimiento se puede hacer
	  	// Enlazar en el grafo <InsertarPorCola>
	  	nodos* x = BuscarVertice(origen);
		if(!EstaIngresadoEnGrafo(x,y->matriz) ){
			nodos *h=ptr;
			while(h->sig!=NULL){ // Buscamos el ultimo nodo
				h=h->sig;
			}
		    h->sig = y; // Insertar por cola
		    y->siga = NULL;
		    y->sig = NULL;
		    
		    arcos *z= (struct arcos *) malloc (sizeof (arcos));
		    z->costo = costo(y->matriz);
		    // z->costo = Peso(); ...
	        if (x->siga == NULL){
	    	    x->siga = z;
				z->sigb = NULL;
				z->sign = y;
			}
			else{ 
				z->sigb = x->siga;
				x->siga = z;
				z->sign = y;
			}
		}
	}
}
	


bool CrearDerecha(int m[3][3]){
	bool sw = false;
	for(int i=0; i<3 && !sw; i++){
		for(int j=0; j<3 && !sw; j++){
			if( m[i][j] == 0){
				if(j>0){
					m[i][j] = m[i][j-1];
					m[i][j-1] = 0;
					sw=true;
				}	
			}
		}	
	}
	return sw;
}
bool MovIzquierda(int m[3][3]){
	bool sw = false;
	for(int i=0; i<3 && !sw; i++){
		for(int j=0; j<3 && !sw; j++){
			if( m[i][j] == 0){
				if(j<2){
					m[i][j] = m[i][j+1];
					m[i][j+1] = 0;
					sw=true;
				}	
			}
		}	
	}
	return sw;
}
bool MovArriba(int m[3][3]){
	bool sw = false;
	for(int i=0; i<3 && !sw; i++){
		for(int j=0; j<3 && !sw; j++){
			if( m[i][j] == 0){
				if(i<2){
					m[i][j] = m[i+1][j];
					m[i+1][j] = 0;
					sw=true;
				}	
			}
		}	
	}
	return sw;
}

bool MovAbajo(int m[3][3]){
	bool sw = false;
	for(int i=0; i<3 && !sw; i++){
		for(int j=0; j<3 && !sw; j++){
			if( m[i][j] == 0){
				if(i>0){
					m[i][j] = m[i-1][j];
					m[i-1][j] = 0;
					sw=true;
				}	
			}
		}	
	}
	return sw;
}

void CopiarMatriz(int origen[3][3], int destino[3][3]){
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			destino[i][j] = origen[i][j];
		}
	}
}

bool MoverMatriz(int m[3][3], string direccion){
	bool MovRealizado = false;
	if ( direccion == "derecha"){
		MovRealizado = CrearDerecha(m);
	} 
    else if ( direccion == "izquierda"){            
    	MovRealizado = MovIzquierda(m);
    }
  	else if ( direccion == "arriba"){           
    	MovRealizado = MovArriba(m);
    }
    else if ( direccion == "abajo"){        
    	MovRealizado = MovAbajo(m);
    }
    return MovRealizado;
  }

bool GenerarSolucion(){
	ptr->sig=NULL; // Vaciar el grafo
	nodos* padre = ptr;
	bool TieneSolucion = true;
	while( TieneSolucion && SonIguales(padre->matriz,Solucion) == false ){
		
		//imprimir(padre->matriz); 
		
		InsertarNodo(padre->matriz,"derecha");
		InsertarNodo(padre->matriz,"izquierda");
		InsertarNodo(padre->matriz,"arriba");
		InsertarNodo(padre->matriz,"abajo");
		if(nHijos(padre) == 0 || pasoos>60){
			pasoos=0;
			//getch();
			TieneSolucion = false;
			ptr=NULL;
		
			break;
		}
		int costo = MejorRuta(padre)->costo;
		padre = MejorRuta(padre)->sign;
		
		pasoos++;
	}
	//MostrarSolucion();
	return TieneSolucion;
}

nodos * BuscarVertice(int m[3][3]){
	nodos * encontrado = NULL;
	nodos *r=ptr;
	bool sw = false;
	while(r!=NULL){
		if( SonIguales(m,r->matriz) ){
			encontrado = r;
		}
		r=r->sig;
	}
	return encontrado;
}
bool SonIguales(int m[3][3], int n[3][3]){
	bool iguales = true;
	for(int x=0; x<3; x++){
		for(int y=0; y<3; y++){
			if(m[x][y] != n[x][y]){
				iguales=false;
				break;
			}
		}
	}
	return iguales;
}
int nHijos (nodos *p){
	int n=0;
	arcos* h = p->siga;
	while( h!=NULL ){
		n++;
		h=h->sigb;
	}
	return n;
}
int costo(int m[3][3]){
	int costo = 0;
	/*
	    g = Numero de imagenes/casillas fuera de lugar
		h = Suma de movimientos para llegar una ficha a su posicion correcta	
	*/

	int g=0, h=0, inversiones = 0;
	//g = imagenes/casillasFueraDeLugar(m);
	for(int i=1; i<=8; i++ ){ // La ficha en blanco (0) no se tiene en cuenta
		h+= distancia(m,i);
	}
	
	// transformamos la matriz a un vector
	int k=0; int vector[9];
	for(int i=0; i<3; i++ ){
		for(int j=0; j<3; j++ ){
			vector[k] = m[i][j];
			k++;
		}
	}
	
	//inversiones = Inversiones(vector);
	costo = g + h + inversiones;

	return costo;	
}
int distancia(int m[3][3],int ficha){
	int x1, y1, x2, y2;
	int pasos = 0;
	// buscamos la posicion de la ficha en la matriz solucion
	for(int x=0; x<3; x++){
		for(int y=0; y<3; y++){
			if(Solucion[x][y] == ficha){
				x1 = x + 1;
				y1 = y + 1;	
			}
		}
	}
	// buscamos la posicion de la ficha en la matriz sin solucion actual
	for(int x=0; x<3; x++){
		for(int y=0; y<3; y++){
			if(m[x][y] == ficha){
				x2 = x + 1;
				y2 = y + 1;	
			}
		}
	}
	// Las distancias siempre son positivas.
	pasos = abs(x1 - x2) + abs(y1 - y2); // Abs es valor absoluto 
		
	return pasos;
}

arcos* MejorRuta(nodos* h){
	int costo=999;
	arcos* mejor = NULL;
	arcos* z = h->siga;
	while(z!=NULL){
		if( z->costo < costo){
			mejor = z;
			costo = z->costo;
		}
		else if(z->costo == costo){
		//	cout<<"Encontre un Costo igual"<<endl;
			int d1 = RegresosDirectos(z->sign->matriz);
			int d2 = RegresosDirectos(mejor->sign->matriz);
	
			if( d1 < d2){
			//	cout<<"Aplique la teoria de Regresos directos"<<endl;
				mejor = z;
				costo = z->costo;
			}else if( d2 == d1){
			//	cout<<"Intentare con menor orden lexonografico"<<endl;
				int a[9], b[9];
				// Convertimos las matrices en vector
				int k = 0;
				for(int i=0; i<3; i++){
					for(int j=0; j<3; j++){
						a[k] = z->sign->matriz[i][j];
						b[k] = mejor->sign->matriz[i][j];
						k++;
					}
				}
				string menor = MenorOrdenLexicografico(a,b);
				if(menor == "a"){
			//		cout<<"Si encontre el menor orden."<<endl;
					mejor = z;
					costo = z->costo;
					
					
				}
			}
		}
		z=z->sigb;
	}
	return mejor;
}

string MenorOrdenLexicografico(int a[9],int b[9]){
	//Debe decir que vector es menor segun el OrdenLexicografico
	string menor = "";
	for(int i=0; i<9; i++){
		if(a[i] < b[i]){
			menor="a";
			break;
		}
		if(b[i] < a[i]){
			menor="b";
			break;
		}
	}
	
	return menor;
}



bool EstaIngresadoEnGrafo(nodos *h, int m[3][3]){
	
	bool respuesta = false;
	nodos* r = ptr;
	nodos *ant =NULL;
	while(r!=NULL){
		if( SonIguales(r->matriz,m) ){
			respuesta=true;
			break;
		}
		r=r->sig;
	}
	
	return respuesta;
}
 int MovMostrados = 0;
void MostrarSolucion(nodos *r){
    //text_mode(-1); // Coloca la fuente trasparente.
            
	if(r==ptr){
	 	draw_sprite(buffer, fondoSolucion, 0, 0);
           dibujarMatriz(r->matriz);
           pantalla();
                       
           textprintf(screen, font,338,140, makecol(500,500,500),"%d / %d ",MovMostrados,pasoos);
           MovMostrados++;
           readkey();
	 	
		return;
	}
	 MostrarSolucion(NodoAnterior(r));
           draw_sprite(buffer, fondoSolucion, 0, 0);
           dibujarMatriz(r->matriz);
           pantalla();
           textprintf(screen, font ,339, 140, makecol(500,500,500),"%d / %d ",MovMostrados,pasoos);
           MovMostrados++;
           readkey();
}

nodos * NodoAnterior(nodos *r){
	nodos *ant = NULL;
	nodos *p = ptr;
	while(p!=NULL){
		arcos *z = p->siga;
		while(z!=NULL){
			if(z->sign == r){
				ant = p;
				break;
			}
			z=z->sigb;
		}
		p=p->sig;
	}
	return ant;
}
int RegresosDirectos(int a[3][3]){
	//intercambiar derecha
	int suma;
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			// intercambiamos con el de la derecha
			if(j<2){
				if(a[i][j]  == Solucion[i][j+1] && a[i][j+1] == Solucion[i][j]){
					suma++;	
				}
			}
			// Intercambio con el de abajo
			if(i<2){
				if(a[i][j] == Solucion[i+1][j] && a[i+1][j] == Solucion[i][j]){
					suma++;	
				}
			}
		}
	}
	return suma*2;	
}
