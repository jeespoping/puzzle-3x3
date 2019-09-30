//pacman
//programa elaborado por carlos arango
//carlosarango@chetumail.com


#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<dos.h>

#define bool     int
#define false    0
#define true     1
#define limite   0.3   //tiempo que se demoran los fanstasmas para pensar
#define sup_time 3.5   //tiempo en el que pacman es invencible cuando come
#define Ncol     15
#define Nfil     15


int px[14]={0,30,60,90,120,150,180,210,240,270,300,330,360,390};
int py[14]={0,30,60,90,120,150,180,210,240,270,300,330,360,390};


//matriz del camino del escenario

int table[Nfil][Ncol]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,
0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,
0,1,0,0,1,1,1,1,1,1,1,0,0,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,
0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,
0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};


//matriz que indica donde hay galleticas 1=pequeñas  2=grandes
int galleta[Nfil][Ncol]={
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,
0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,
0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,
0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,
0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,
0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,
0,2,1,1,1,1,1,1,1,1,1,1,1,2,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

int Ngalletas=0; //numero total de galletas
enum direccion {arriba,abajo,izquierda,derecha};
int Nfan; //numero de fantasmas
clock_t inicio,fin,primer;
clock_t ini_sup;
float  total; //tiempo total

bool terminar=false; 
bool indefensos=false; //cuando vale true pacman es invencible

int puntaje;


int x,y;
bool cc; 
void dibujar();





 if(!indefensos){setcolor(14);setfillstyle(SOLID_FILL,14);} //esto es para que cambie de color
 else{if(cc){setcolor(1);setfillstyle(SOLID_FILL,1);cc=false;}
      else {setcolor(3);setfillstyle(SOLID_FILL,3);cc=true;}
     }
 switch(dir)
       {case arriba:
	     pieslice(px[x]+15,py[y]+15,0,60,10);
	     pieslice(px[x]+15,py[y]+15,120,360,10);
	     break;
	case abajo:
	     pieslice(px[x]+15,py[y]+15,0,240,10);
	     pieslice(px[x]+15,py[y]+15,300,360,10);
	     break;
	case izquierda:
	     pieslice(px[x]+15,py[y]+15,0,150,10);
	     pieslice(px[x]+15,py[y]+15,210,360,10);
	     break;
	case derecha:
	     pieslice(px[x]+15,py[y]+15,30,330,10);
	     break;
       }
}


class fantasma{
public:
int x,y;
int color;
bool levantado; //si pacman es invencible y lo levanto
fantasma(int xx,int yy,int c){x=xx;y=yy;color=c;levantado=false;}
void direccionar();
void randomizar_();
void dibujar();
};

typedef fantasma* pfan;
pfan fan[7]; //array de punteros a objeto

void fantasma::dibujar()
{
 if(levantado)
     {  setfillstyle(SOLID_FILL,color);
	bar(px[x]+11,py[y]+11,px[x]+14,py[y]+14);
	bar(px[x]+16,py[y]+11,px[x]+19,py[y]+14);
     }
 else{  setfillstyle(SOLID_FILL,color);
	bar(px[x]+10,py[y]+8,px[x]+20,py[y]+17);
	bar(px[x]+6,py[y]+15,px[x]+11,py[y]+20);
	bar(px[x]+13,py[y]+15,px[x]+16,py[y]+20);
	bar(px[x]+18,py[y]+15,px[x]+23,py[y]+20);
	setfillstyle(SOLID_FILL,BLACK);
	bar(px[x]+12,py[y]+11,px[x]+14,py[y]+14);
	bar(px[x]+16,py[y]+11,px[x]+18,py[y]+14);
     }
}

bool es_posible(int xx,int yy) //dice si un fantasma puede moverse al punto
{                               //indicado
 if(table[yy][xx]==0)return false;
 else{
      for(register int i=0;i<Nfan;i++)
	 {if(fan[i]->x==xx&&fan[i]->y==yy)return false;
	 }
     }
 return true;
}

//cuando pacman es invencible el fantasma sigue esta rutina

void fantasma::randomizar_()
{
 if(es_posible(x,y+1))y++;
 else if(es_posible(x,y-1))y--;
 else if(es_posible(x-1,y))x--;
 else if(es_posible(x+1,y))x++;
}


//los fantasmas siguen a pacman
void fantasma::direccionar()
{
 if(x-pac.x>0)
     {
      if(y-pac.y>0)
	  {if(y-pac.y>x-pac.x)
	       {if(es_posible(x,y-1))y--;
		else if(es_posible(x-1,y))x--;
		else if(es_posible(x,y+1))y++;
		else if(es_posible(x+1,y))x++;
	       }
	   else{if(es_posible(x-1,y))x--;
		else if(es_posible(x,y-1))y--;
		else if(es_posible(x,y+1))y++;
		else if(es_posible(x+1,y))x++;
	       }
	  }
      else{if(abs(y-pac.y)>x-pac.x)
	       {if(es_posible(x,y+1))y++;
		else if(es_posible(x-1,y))x--;
		else if(es_posible(x,y-1))y--;
		else if(es_posible(x+1,y))x++;
	       }
	   else{if(es_posible(x-1,y))x--;
		else if(es_posible(x,y+1))y++;
		else if(es_posible(x,y-1))y--;
		else if(es_posible(x+1,y))x++;
	       }
	  }
     }
 else{
      if(y-pac.y>0)
	  {if(y-pac.y>abs(x-pac.x))
	       {if(es_posible(x,y-1))y--;
		else if(es_posible(x+1,y))x++;
		else if(es_posible(x,y+1))y++;
		else if(es_posible(x-1,y))x--;
	       }
	   else{if(es_posible(x+1,y))x++;
		else if(es_posible(x,y-1))y--;
		else if(es_posible(x,y+1))y++;
		else if(es_posible(x-1,y))x--;
	       }
	  }
      else{if(abs(y-pac.y)>abs(x-pac.x))
	       {if(es_posible(x,y+1))y++;
		else if(es_posible(x+1,y))x++;
		else if(es_posible(x,y-1))y--;
		else if(es_posible(x-1,y))x--;
	       }
	   else{if(es_posible(x+1,y))x++;
		else if(es_posible(x,y+1))y++;
		else if(es_posible(x,y-1))y--;
		else if(es_posible(x-1,y))x--;
	       }
	  }
      }
}

void iniciar_modografico()
{
 int gdriver = DETECT, gmode, errorcode;
 initgraph(&gdriver, &gmode, "");
 errorcode = graphresult();
 if (errorcode != grOk)
  {
   printf("Graphics error: %s\n", grapherrormsg(errorcode));
   printf("Press any key to halt:");
   getch();
   exit(1);
  }
}


//se grafica el tablero segun la matriz de caminos
void tablerito()
{setcolor(3);
 int i,j;
 for(j=0;j<Nfil;j++)
    {for(i=0;i<Ncol;i++)
	{if(table[j][i]==1)
	   {
	    if(table[j][i-1]==0)line(i*30,j*30,i*30,(j+1)*30);
	    if(table[j][i+1]==0)line((i+1)*30,j*30,(i+1)*30,(j+1)*30);
	    if(table[j-1][i]==0)line(i*30,j*30,(i+1)*30,j*30);
	    if(table[j+1][i]==0)line(i*30,(j+1)*30,(i+1)*30,(j+1)*30);
	   }
	}
    }
 setcolor(15);
 outtextxy(500,100,"PUNTOS");
 outtextxy(500,150,"TIEMPO");
}

//convierte los putajes y el tiempo a cadenas y las imprime

void actualizar_puntaje()
{
 char tim[5];
 char pun[5];
 itoa((int)total,tim,10);
 itoa(puntaje,pun,10);
 setfillstyle(1,0);
 bar(500,120,550,130);
 bar(500,170,550,180);
 setcolor(15);
 outtextxy(500,120,pun);
 outtextxy(500,170,tim);
}

//dibuja galleta pequeña
void galletica(int xx,int yy)
{setcolor(BROWN);
 circle(px[xx]+15,py[yy]+15,2);
 setfillstyle(SOLID_FILL,BROWN);
 floodfill(px[xx]+15,py[yy]+15,BROWN);
}

//dibuja galleta grande
void galletica_p(int xx,int yy)
{
 setcolor(BROWN);
 circle(px[xx]+15,py[yy]+15,5);
 setfillstyle(SOLID_FILL,BROWN);
 floodfill(px[xx]+15,py[yy]+15,BROWN);
}

//borra un cuadro del juego
void borra(int xx,int yy)
{setfillstyle(SOLID_FILL,0);
 bar(px[xx]+2,py[yy]+2,px[xx]+28,py[yy]+28);
}


void leer_teclado()
{char tecla;
 tecla=getch();
 if(tecla==27)terminar=true;
 else if(!tecla)
	{
	 tecla=getch();
	 if(tecla==80&&table[pac.y+1][pac.x]==1){pac.y++;pac.dir=abajo;}
	 else if(tecla==72&&table[pac.y-1][pac.x]==1){pac.y--;pac.dir=arriba;}
	 else if(tecla==75&&table[pac.y][pac.x-1]==1){pac.x--;pac.dir=izquierda;}
	 else if(tecla==77&&table[pac.y][pac.x+1]==1){pac.x++;pac.dir=derecha;}
	}
}

void perdio()
{
    setcolor(15);
    settextstyle(0,0,5);
    outtextxy(100,100,"PERDISTE");
}

void gano()
{int col=0;
 for(int i=0;i<60;i++)//Esto es para que cambie de color
     {col++;
      if(col==16)col=0;
      delay(50);
      setcolor(col);
      settextstyle(0,0,5);
      outtextxy(100,100,"GANASTE!");
     }
}
//busca si un fantasma es atrapado por pacman invencible
void fan_muertos()
{for(int i=0;i<Nfan;i++)
    {if(fan[i]->x==pac.x&&fan[i]->y==pac.y)
       {fan[i]->levantado=true;
	fan[i]->x=5+i;
	fan[i]->y=6;
	puntaje+=20;
       }
    }
}
//busca si pacman esta muerto
void buscar_ataque()
{for(int i=0;i<Nfan;i++)
    {if(fan[i]->x==pac.x&&fan[i]->y==pac.y)
       {perdio();
	terminar=true;
	break;
       }
    }
}


//mueve todos los fantasmas
void actualizar_fantasmas()
{
 for(int i=0;i<Nfan;i++)
    {borra(fan[i]->x,fan[i]->y);
     if(galleta[fan[i]->y][fan[i]->x]==1)galletica(fan[i]->x,fan[i]->y);
     else if(galleta[fan[i]->y][fan[i]->x]==2)galletica_p(fan[i]->x,fan[i]->y);
     if(!indefensos)fan[i]->direccionar();
     else fan[i]->randomizar_();
     fan[i]->dibujar();
    }
}



void main()
{
//objetos
 int i,j;
 Nfan=5;
 fan[0]=new fantasma(13,1,BLUE); //Se pide memoria con cada puntero
 fan[1]=new fantasma(1,13,YELLOW);
 fan[2]=new fantasma(13,13,GREEN);
 fan[3]=new fantasma(1,1,RED);
 fan[4]=new fantasma(7,13,WHITE);
 iniciar_modografico();

 settextstyle(0,0,1);
 setcolor(15);
 outtextxy(200,200,"PACMAN..");
 getch();
 cleardevice();

 tablerito();
 for(j=0;j<Nfil;j++)
    {for(i=0;i<Ncol;i++)//dibujamos y contamos las galletas
	{if(galleta[j][i]==1){galletica(i,j);Ngalletas++;}
	 else if(galleta[j][i]==2){galletica_p(i,j);Ngalletas++;}
	}
    }
 pac.dibujar();

 for(i=0;i<Nfan;i++)fan[i]->dibujar();
 total=0;
 puntaje=0;
 primer=clock();

 do{  //ciclo principal
	inicio=clock();
	do{
	   if(kbhit())//busca si hay algo en el buffer
	       {borra(pac.x,pac.y);
		leer_teclado();
		pac.dibujar();
		if(!indefensos)buscar_ataque();
		else fan_muertos();
		if(terminar)break;
		if(galleta[pac.y][pac.x]==1)
		  {
		   galleta[pac.y][pac.x]=0;
		   Ngalletas--;
		   puntaje+=5;
		  }
		if(galleta[pac.y][pac.x]==2)
		  {
		   galleta[pac.y][pac.x]=0;
		   Ngalletas--;
		   puntaje+=10;
		   indefensos=true;
		   ini_sup=clock();
		  }
	       }
	   fin=clock();
	   if(indefensos)
		{if(((fin-ini_sup)/CLK_TCK)>sup_time)
		   {indefensos=false;
		    for(i=0;i<Nfan;i++)fan[i]->levantado=false;
		   }
		}
	   total=(fin-primer)/CLK_TCK;
	}while((fin-inicio)/CLK_TCK<limite);
	if(terminar)break;
	actualizar_puntaje();
	actualizar_fantasmas();
	if(!indefensos)buscar_ataque();
	else fan_muertos();
	if(terminar)break;
 }while(Ngalletas!=0);
 if(Ngalletas==0&&!terminar)gano();
 getch();
 delay(3000);
 for(i=0;i<Nfan;i++)delete fan[i]; //Se libera la memoria de los fantasmas
 closegraph();
}




