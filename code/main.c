#include <main.h>
#include <stdio.h>
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, ERRORS)

//communication avec port COM
int flag_threshold = 0; //flag on si nouveau seuil
char *buffer[4]; // 4 car cara dï¿½but trame + seuil (max = 999) ==> ex: @954 ou @001 ou @051
int buf_compteur = 0;
char *trame_start = "@";

#INT_RDA
void  RDA_isr(void)
{
   buffer[buf_compteur] = getc();   //lis le caractï¿½re ï¿½ la position courante du flux de donnï¿½es
   trame_start = buffer[0];
   if (strcmp(buffer[0], trame_start) == 0 && flag_threshold == 0){   //si trame commence par @ et qu'on ne traite pas dï¿½jï¿½ un nouveau seuil
      buf_compteur ++;
      if (buf_compteur >= 4){    //Traitement quand buffer plein
         buf_compteur = 0;
         flag_threshold = 1;
      }
   }
}

void main()
{
   //init variable
   int16 threshold;
   long duration;
   int16 distance;
   int poids_fort;
   int poids_faible;

   setup_timer_0(RTCC_INTERNAL);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   setup_low_volt_detect(FALSE);

   while(1)
   {
      //Initialization + être sure que trigger est off
      output_low(PIN_C0);
      delay_ms(300);
      
      //Prise d'une mesure
      output_high(PIN_C0);
      delay_us(300);
      output_low(PIN_C0);
      
      //Attend un output
      while(!input(PIN_C1)){}
      set_timer0(0);

      
      //Attend que l'output se termine
      while(input(PIN_C1)){}
      duration = (long)get_timer0();
      distance = (int16)(duration / 145);
      
          //Affichage
      if (distance >= 1000){ //Overflow ou distance négative = erreur
         output_b(0);
         output_high(PIN_D7); //affiche juste un point 

      }
      else if (distance >= 100){ // affichage passe en metre
         
         poids_fort = (int)(distance / 100);
         poids_faible = (int)((distance % 100) / 10);
         
         output_high(PIN_D7);
         output_b((poids_faible << 4) + poids_fort); //Decalage de 4 vers la doite à cause du montage physique et logique
 
      }
      else { //affichage en centimetre
      
         poids_fort = (int)distance / 10;
         poids_faible = (int)distance % 10;
         
         output_low(PIN_D7);
         output_b((poids_faible << 4) + poids_fort);
      }
      
      // Nouveau seuil
      if(flag_threshold){  //Calcul nouveau seuil (char - 48 => convert char to int)
         int16 a = buffer[1] - 48;
         int16 b = buffer[2] - 48;
         int16 c = buffer[3] - 48;
         threshold = (int16)((a*100)+(b*10)+c);
         flag_threshold = 0;

      } 
      
      if(threshold == NULL){  //valeur par defaut 
         threshold = 50;

      }
      
      //Alerte
      if(threshold < distance){
         output_high(PIN_D1); //alume led verte
         output_low(PIN_D0);

      }
      if(threshold > distance){
         output_low(PIN_D1); //Coupe led verte
         output_toggle(PIN_D0); //Clignote led red
       
      }
     
   }
}


