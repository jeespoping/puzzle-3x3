#include <stdio.h>

main()
{
float peso,dolar,uf,total,opc;
do
{
Printf ("conversion\n");
printf ("----------\n\n\n");
printf ("1.- Peso a Dolar.\n ");
printf ("2.- Dolar a peso.\n ");
printf ("3.- Peso a euro.\n ");
printf ("4.- Euro a peso.\n ");
printf ("5.- Peso a uf.\n ");
printf ("6.- Uf a peso.\n ");
printf ("7.- salir.\n ");
scanf ("%f",&opc);
}
while (opc>=1 && opc<=7);
return(0);
getch();
}
