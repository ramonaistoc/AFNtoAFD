//Tranformarea unui automot finit nedeterministe
//intr-un automat finit deterministe

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ifstream f("date.in");

void creare_matrice_afn( int &nr_stari_afn,int &nr_tranz_afn,int &nr_litere,int &nr_stari_fin_afn,int stari_fin_afn[20], int matrice_afn[20][20][20] )
{
    int i, j;
    f>>nr_litere;

    f>>nr_stari_afn;
    f>>nr_stari_fin_afn;

    for(i=0; i<nr_stari_fin_afn; i++)
        f>>stari_fin_afn[i];

    f>>nr_tranz_afn;

    int stare1, stare2;
    char litera;

    for(i=0; i<nr_stari_afn; i++)
        for (j=0; j<nr_litere; j++)
            matrice_afn[i][j][0]=-1;

    for(i=0; i<nr_tranz_afn; i++)
    {
        f>>stare1>>litera>>stare2;

        j=0;
        while(matrice_afn[stare1][litera-'a'][j]!=-1)
            j++;
        matrice_afn[stare1][litera-'a'][j]=stare2;
        matrice_afn[stare1][litera-'a'][j+1]=-1;
    }
}

void afisare_afn( int nr_stari_afn, int nr_litere, int matrice_afn[20][20][20])
{
    int i, j, k;
    for(i=0; i<nr_stari_afn; i++)
    {
        cout<<"Starea "<<i<<":";
        for(j=0; j<nr_litere; j++)
        {
            for(k=0; matrice_afn[i][j][k]!=-1; k++)
                cout<<matrice_afn[i][j][k];
            for (int l=0; l<=nr_stari_afn-k+1; l++)
                cout<<" ";
        }
        cout<<endl;
    }
}

int verif_stare(int matrice_afd[20][20][20], int stare[20], int nr_stari_adaugate, int nr_litere)
{
    int i, k;

    for(i=0;i<nr_stari_adaugate;i++)        //verif daca starea pe care o adaug mai exista
    { k=0;                                  //verif lungimea si caracterul
      while(matrice_afd[i][nr_litere][k]!=-1 && stare[k]!=-1 && matrice_afd[i][nr_litere][k]==stare[k])
        k++;

      if(stare[k]==-1 && matrice_afd[i][nr_litere][k]==-1)      //daca am ajuns la final cu ambele starea exista
         return 1;
    }
    return 0;
}

void sortare_stergere(int v[20], int n)
{
    int i, j, aux;

    for(i=0;i<n-1;i++)
      {
        for(j=i+1;j<n;j++)
         if(v[i]>v[j])
         {
           aux=v[i];
           v[i]=v[j];
           v[j]=aux;
         }
      }

    for(i=0;i<n-1;i++)
      if(v[i]==v[i+1])
    {
       for(j=i+1;j<n;j++)
            v[j]=v[j+1];

      n--;
     }
}
void adaug_stare(int matrice_afn[20][20][20], int matrice_afd[20][20][20], int nr_stari_afn, int stare_adaugata[10], int &nr_stari_adaugate, int nr_litere)
{
    int i, j, k, m;
 if(verif_stare(matrice_afd,stare_adaugata,nr_stari_adaugate,nr_litere)==0)
 {
    for(i=0;stare_adaugata[i]!=-1;i++)                                  //adaug coloana de stari la sfarsit pentru ca pe coloana 0 e a, 1 b
         matrice_afd[nr_stari_adaugate][nr_litere][i]=stare_adaugata[i];

    matrice_afd[nr_stari_adaugate][nr_litere][i]=-1;

    for(i=0;i<nr_litere;i++)
    {
        m=0;
      for(j=0;stare_adaugata[j]!=-1;j++)                                    //copiez in matricea afd elementele din afn; pt mai multe stari sunt parcurse amandoua
      {
        for(k=0;matrice_afn[stare_adaugata[j]][i][k]!=-1;k++)
          matrice_afd[nr_stari_adaugate][i][m++]=matrice_afn[stare_adaugata[j]][i][k];
      }
        matrice_afd[nr_stari_adaugate][i][m]=-1;
        sortare_stergere(matrice_afd[nr_stari_adaugate][i],m);
    }
    nr_stari_adaugate++;
  }
}


void creare_matrice_afd(int &s, int &nr_stari_adaugate, int nr_stari_afn, int stare_start, int nr_litere, int stari_fin_afd[20][20], int matrice_afd[20][20][20], int matrice_afn[20][20][20], int stari_fin_afn[20], int nr_stari_fin_afn)
{
    int v[2];
    v[0]=stare_start;       //prima data adaug starea de start
    v[1]=-1;
    nr_stari_adaugate=0;
    adaug_stare(matrice_afn, matrice_afd, nr_stari_afn,v, nr_stari_adaugate, nr_litere);

    int i=0;

    while(i<nr_stari_adaugate)  //adaug starile in ultima coloana
    {
        for(int j=0;j<nr_litere;j++)
         if(matrice_afd[i][j][0]!=-1)
             adaug_stare(matrice_afn,matrice_afd, nr_stari_afn,matrice_afd[i][j],nr_stari_adaugate, nr_litere);

         i++;
    }

    int  k, l, q;
     s=0;

    for(i=0;i<nr_stari_adaugate;i++)
       {
         for(k=0; matrice_afd[i][nr_litere][k]!=-1;k++)
        {
         for(l=0; l<nr_stari_fin_afn;l++)
            if (stari_fin_afn[l]==matrice_afd[i][nr_litere][k])
                {
                for(q=0; matrice_afd[i][nr_litere][q]!=-1; q++)
                    stari_fin_afd[s][q]=matrice_afd[i][nr_litere][q];
         stari_fin_afd[s][q]=-1;
         s++;}

         }}
}
void afisare_afd(int s,  int nr_stari_adaugate, int nr_litere, int matrice_afd[20][20][20], int stari_fin_afd[20][20])
{
    int i, j, k;

    for(i=0; i<nr_stari_adaugate; i++)
    {
        for(k=0;matrice_afd[i][nr_litere][k]!=-1;k++)
            cout<<matrice_afd[i][nr_litere][k];
        for (int l=0; l<=nr_stari_adaugate-k+1; l++)
                cout<<" ";
        for(j=0; j<nr_litere; j++)
        {
            for(k=0; matrice_afd[i][j][k]!=-1; k++)
                cout<<matrice_afd[i][j][k];
            for (int l=0; l<=nr_stari_adaugate-k+1; l++)
                cout<<" ";
        }
        cout<<endl;

    }

    for(i=0;i<s;i++)
    {for(int j=0;stari_fin_afd[i][j]!=-1;j++)
        cout<<stari_fin_afd[i][j]<<" ";
        cout<<endl;}
}

int main()
{
    int nr_stari_afn, nr_tranz_afn, nr_litere, nr_stari_fin_afn, stari_fin_afn[20], stare[20], nr_stari_adaugate;
    int matrice_afn[20][20][20];
    int stare_start;
    creare_matrice_afn(nr_stari_afn, nr_tranz_afn, nr_litere, nr_stari_fin_afn, stari_fin_afn, matrice_afn);
    afisare_afn(nr_stari_afn, nr_litere, matrice_afn);

    int stari_fin_afd[20][20];
    int matrice_afd[20][20][20];

    cout<<"Stare start";
    cin>>stare_start;
    int s;
    creare_matrice_afd(s, nr_stari_adaugate, nr_stari_afn, stare_start, nr_litere, stari_fin_afd, matrice_afd, matrice_afn, stari_fin_afn, nr_stari_fin_afn);
    afisare_afd(s,nr_stari_adaugate, nr_litere, matrice_afd, stari_fin_afd);

    return 0;
}
