#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
int pdesk[2];
pipe(pdesk);
int pdesk2;
time_t czas;
time_t *ti;
czas=time(ti);
printf("Czas biezacy: %ld \n", czas);  

switch(fork())
{
case -1:
        exit(1);
case 0:
        if (mkfifo("kolejka", 0777) == -1)
        {
                printf("blad tworzenia kolejki FIFO\n");
                exit(1);
        }
        switch(fork())
        {
        case -1:
                exit(1);
        case 0: {
                time_t wczytaj;
                printf("Wprowadz liczbe sekund\n");
                scanf("%ld", &wczytaj);
                 printf("Wartosc wczytana to %ld\n", wczytaj);
                close(1);
                pdesk2=open("kolejka",O_WRONLY);
                if (pdesk2 != 1)
                        {
                         printf("blad deskryptora do zapisu\n");
                         exit(1);
                         };
                write(pdesk2,&wczytaj, sizeof(wczytaj));
                exit(1);
}
        default:   
                close(0);
                pdesk2=open("kolejka",O_RDONLY);
                if (pdesk2 != 0)
                 {printf("blad deskr odczytu\n");
                exit(1);
                 };
                time_t wczytaj_z_fifo;
                read(pdesk2, &wczytaj_z_fifo, sizeof(wczytaj_z_fifo));
                printf("Wartosc wczytana z fifo %ld\n", wczytaj_z_fifo);
                struct tm *wczas=localtime(&wczytaj_z_fifo);
                char bufor[100];
                strftime(bufor, 100, "Rok: %Y miesiac: %m dzien: %d dzien tygodnia:  %a", wczas);
                printf("Czas ze struktury:\n%s\n", bufor);
		write(pdesk[1], &wczas->tm_year, sizeof(wczas->tm_year));
                exit(1);   
        }
        exit(1);
default:{
        int odebrany_rok;
	read(pdesk[0], &odebrany_rok, sizeof(odebrany_rok));
	odebrany_rok += 1900;
	printf("Odebrany rok: %d\n", odebrany_rok);
        exit(1);
}
}
}
