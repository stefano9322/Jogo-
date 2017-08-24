/*
standard input output para imprimir caracteres de codigo ascii
windows.h para el cursor comandos
*/
#include <stdio.h>
#include <windows.h> // coord set... y handle
#include <conio.h>
#include <stdlib.h>// random
#include <list>
using namespace std;
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
void gotoxy(int x, int y) // coordenadas x e y // viene de la biblioteca conio.h
{
    HANDLE hcn;
    hcn=GetStdHandle(STD_OUTPUT_HANDLE);//Salida de la consola
    COORD dwPos; // estructura, objeto; posicion de la coordenada
    dwPos.X=x;
    dwPos.Y=y;

    SetConsoleCursorPosition(hcn,dwPos);//(handle y estructura de datos)handle, menu puntero del raton

}

void ocultar_cursor()// para ocultar cursor[]
{
    HANDLE hcn;
    hcn = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize=2;// var tipo enetera de 1-100
    cursor.bVisible=false; // tomar 2 tributos V o F si es v se mpostrara caso cantrario
    SetConsoleCursorInfo(hcn,&cursor);// handle id de consola
    // curs por ref para accder a datos sin modificarse
}
void dib_limites()// cerco
{
    for(int i=2; i<78;i++)// recta horizontal
    {
        gotoxy(i,3);printf("%c",205); //arriba
        gotoxy(i,33);printf("%c",205); // abajo
    }
    for (int i=4; i<33;i++) // recta vertical
    {
        //    (y,x)
        gotoxy(2,i);printf("%c",186);// izquierda
        gotoxy(77,i);printf("%c",186); // derecha
    }
    gotoxy(2,3);printf("%c",201); // esquinas F
    gotoxy(77,3);printf("%c",187);// esquinas T
    gotoxy(2,33);printf("%c",200);// esquinas L
    gotoxy(77,33);printf("%c",188);// esquinas j

}
class Nave
{
    int x,y,cora,vidas; // coordenadas
public:
    Nave(int xx, int yy, int cc, int vi):x(xx),y(yy),cora(cc),vidas(vi){}
    int getX(){return x;}// para return var private x
    int getY(){return y;}// ""           ""         y
    int getV(){return vidas;}
    void corazon(){cora--;}// diminuye el num de corazones tras impacto con esteroides lo que te inyectas
    void pintar_nave() // diseñar la nave
    {
        gotoxy(x,y);    printf("  %c",30);
        gotoxy(x,y+1);  printf(" %c%c%c",30,30,30);
        gotoxy(x,y+2);  printf("%c%c %c%c",30,30,30,30);
    }
    void borrar_nave() // borrar posiciones
    {
        gotoxy(x,y)  ;printf("        ");
        gotoxy(x,y+1);printf("        "); // +1 al contrario de -1
        gotoxy(x,y+2);printf("        "); // -1 al contrario de +1
    }
    void mover_nave()
    {
        if (kbhit())// tecla para las coordenadas
        {
            char tecla=getch();// func guardar letra que se presione a var tecla
            borrar_nave();
            //gotoxy(x,y);printf(" "); // para borrar de xxxxxxx -> x    x  x
            if(tecla==IZQUIERDA && x>3)x-=2; // mover 1 pos a izquierda
            if(tecla == DERECHA && x+6<76)x+=2;// mover una pos a derecha +6 por el ancho de la nave
            if(tecla ==ARRIBA && y>4)y--;// para subir
            if(tecla==ABAJO && y+3<33)y++; // +3 por el alto de la nave
            //gotoxy(x,y);printf("X");
            if (tecla=='e')cora--; // opcional para bajar los corazones
            pintar_nave();
            pintar_corazon();
        }

    }
    void pintar_corazon()
    {
        gotoxy(50,2);printf("Vidas %d",vidas); // imprime vidas en posicion (2,50) con num 3
        gotoxy(64,2);printf("Salud"); // pos fil x col (2,64)
        gotoxy(70,2);printf("    "); // pos (2,70) 4 tabs
        for(int i=0;i<cora;i++)
        {
            gotoxy(70+i,2);printf("%c",3); // por cada caracter 4 corazon
        }
    }
    void morir_nave()
    {
        if (cora==0)
        {
            borrar_nave();// borra la nave
            gotoxy(x,y)  ;printf("   **   ");
            gotoxy(x,y+1);printf("  ****  "); // 3 gotoxy para la altura de nave
            gotoxy(x,y+2);printf("   **   ");
            Sleep(200);// tiempo de ejecucion 200 milisegundos
            borrar_nave();// borra  la explosion
            gotoxy(x,y)  ;printf(" * ** * ");
            gotoxy(x,y+1);printf("  ****  "); // 3 gotoxy para la altura de nave
            gotoxy(x,y+2);printf(" * ** * ");
            Sleep(200);
            borrar_nave();// segunda explosion
            vidas--;
            cora=3;
            pintar_corazon();
            pintar_nave();
        }
    }
};
class Asteroide // clase asteroide
{
    int x,y;
public:
    Asteroide(int xx, int yy):x(xx),y(yy){}
    int getX(){return x;}
    int getY(){return y;}
    void pintar_asteroide()
    {
        gotoxy(x,y);printf("%c",184);
    }
    void mover_asteroide()
    {
        gotoxy(x,y);printf(" ");// para mover y no dejar rastro
        y++;// para que vaya cayendo asteroides
        if (y>32)// piso
        {
            x=rand()%71+4; // limite de la izquierda numero entre 0 y 71
            //Sleep(100000000); // tiempo para que aparezca el asteroide // como controlar la veloc?
            y=4; // para subirlo desde el cerco arriba comienzan a caer
        }
        pintar_asteroide();
    }
    void colision (class Nave &N)//& para modificar var // asteroide choque a la nave y los corzaones diminuyan
    {
        // coord de Xde asteroide mayor a coord de nave
        if (x>=N.getX()&& x<N.getX()+5 && y>=N.getY()&& y<=N.getY()+2)// +5 por el tam de la nave 6 // para saber si hay colision
        {
            N.corazon(); //para quitarle corazon por colision de esteroide
            N.pintar_nave();// (evita)cae el asteroide y borrar parte de la nave
            N.pintar_corazon();
            x=rand()%71+4;// para que se elija un valor al azar
            y=4;
        }

    }

};

class Bala
{
    int x, y;
public:
    Bala(int xx, int yy):x(xx),y(yy){}
    int getX(){return x;}
    int getY(){return y;}
    void mover()
    {
        gotoxy(x,y);printf(" ");
        if (y>4)y--; //para evitar que choque con el cerco arriba
        gotoxy(x,y);printf("%c",206);
    }
    bool fuera()
    {
        if(y==4)return true;
        return false;
    }
};

int main()
{



    ocultar_cursor();
    dib_limites();
    Nave n(37,30,3,3);

    n.pintar_nave();
    n.pintar_corazon();
    list<Asteroide*>A;
    list<Asteroide*>::iterator itA;

    for (int i=0; i<5; i++)
    {
        A.push_back(new Asteroide(rand()%75+3,rand()%5+4));
    }
    //Asteroide a(10,4);
    //Asteroide a1(7,4);
    //Asteroide a2(15,10);
    // invocar mas constrcutores y llamar a metodos para paraecer mas 1 asteroide
    list<Bala*>B; // lista para las balas
    list<Bala*>::iterator it;// iterador para recorrer la lista
    bool fin_juego=false;
    int puntos =0;
    while (!fin_juego)// mientras V
    {
        gotoxy(4,2);printf("Puntos %d ",puntos);
        if(kbhit())
        {
            char tecla=getch();
            if (tecla=='d') // por el medio pos 2
            B.push_back(new Bala(n.getX()+2, n.getY()-2));
            // balas de arriba que han sido creadas
            // crear un iterador
            for (it=B.begin(); it!= B.end(); it++)
            {
                (*it)->mover();
                if ((*it)->fuera())// para saber si es verdad si llego al limite
                {
                    gotoxy((*it)->getX(),(*it)->getY());printf(" ");
                    delete(*it); // para eliminar una bala
                    it=B.erase(it);// quitamos unos igual al que sigue primero 1 luego 2 ...

                    puntos+=5; // colision entre bala y esteroides

                }
            }
        }

        for(itA=A.begin(); itA!=A.end();itA++)
        {
            (*itA)->mover_asteroide();
            (*itA)->colision(n);
        }
        //a.mover_asteroide();a.colision(n);
        //a1.mover_asteroide();a1.colision(n);
        //a2.mover_asteroide();a2.colision(n);
        // parametro objeto N
        for(itA=A.begin();itA!=A.end();itA++)// recorrer asteroides la lista
        {
            for(it=B.begin();it!=B.end();it++)// las balas
            {
                if(((*itA)->getX()==(*it)->getX()) && ((*itA)->getY()+1 == (*it)->getY() || (*itA)->getY()== (*it)->getY()))// colision de las balas
                {
                    gotoxy((*it)->getX(),(*it)->getY()); printf(" ");
                    delete (*it);
                    it=B.erase(it);
                    A.push_back(new Asteroide(rand()%75+3,4));
                    gotoxy((*itA)->getX(),(*itA)->getY()); printf(" ");
                    delete(*itA);
                    itA=A.erase(itA);
                }

            }
        }
        if(n.getV()==0)fin_juego=true; // vidas = 0  el programa se detiene
        n.morir_nave();
        n.mover_nave();
        Sleep(30);// 30 milisegundos para no saturar el procesador
    }
    printf("fin del juego");

}
