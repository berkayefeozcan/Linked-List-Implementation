#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
/*
        Program ile ilgili notlar :
        * sehirler.txt den veri cekerken 57,sinop,KA orneginde oldugu gibi
        hem kosuluk bilgisi olmayip hemde satir sonunda virgil bulunmazsa
        sehir ismini listeye ekleyemiyoruz.

        * eger sehirler.txt nin sonunda fazladan bir bosluk varsa program
        patliyor.
        *Sehir eklemede sehir ismi bosluk birakilip girilirse yeni bir sehirmis gibi algilaniyor.
*/
struct sehirBilgileriNode
{

    char sehirIsmi[50];
    char bolge[50];
    int plakaKodu;
    int komsuSayisi;
    struct sehirBilgileriNode *sehirNext;
    struct sehirBilgileriNode *sehirPrev;
    struct komsuBilgileriNode *komsuRoot;

};

struct komsuBilgileriNode
{

    int plakaKodu;
    struct komsuBilgileriNode *next;

};

typedef struct sehirBilgileriNode sehirNode; // kisaltma kullandik.
typedef struct komsuBilgileriNode komsuNode;


sehirNode *sehirNodeRoot=NULL;// sehir bilgilerinin kokunu isaret edecek.
sehirNode *iter=NULL;// sehirler uzerinde gezinmek icin tanimladik.
sehirNode *sehirNodeEnd=NULL;

int modelleme(char *sehirDosyaYolu,char *outputYol);
void listeyiBastir();
void sehirEkle(char *sehirIsmi, char *bolge,int plakaKodu);
sehirNode* sehirBul(char *sehirIsmi);
void komsulukEkle(char *ciktiYolu,char *sehirIsmi,char *komsuIsmi,int bayrak);
void araligaGoreListele(char *output,int ustSinir, int altSinir);
void ekranTemizle();
void bellektekileriDosyayaYaz(char *output);
void sehirSil(char *outputPath, char *sehirIsmi);
void komsuSil(char * outputPath,char * sehirIsmi, char * komsuIsmi);
void bolgeyeGoreListele(char *output,char *bolge);
int main()
{
    char secim[50];
    char sehirIsmi[50];
    char komsuIsmi[50];
    char bolge[50];
    int altSinir;
    int ustSinir;
    int result;
    int plakaKodu;
    FILE *cikti;
    ekranTemizle();

    if (modelleme("sehirler.txt","cikti.txt") == 1 )
    {
        printf("sehirler.txt den basariyla sehirler ve komsulari okundu.\n");
        printf("Devam etmek icin bir tusa basiniz");
        getch();
    }


    while(1)
    {
        ekranTemizle();
        printf("\n  PROLAB 3 BAGLI LISTE UYGULAMASI\n");
        printf("***************************************************************\n");
        printf("*   1- Yeni Sehir Ekle                                        *\n");
        printf("*   2- Komsuluk Ekle                                          *\n");
        printf("*   3- Sehir Silme                                            *\n");
        printf("*   4- Komsuluk Silme                                         *\n");
        printf("*   5- Sehir arama                                            *\n");
        printf("*   6- Bir bolgede bulunan sehirlerin bilgilerini listele     *\n");
        printf("*   7- a-b arasinda komsuluk sayisina sahip sehirleri listele *\n");
        printf("*   8- Sehirlerin bilgilerini listele(Hem dosyaya hem ekrana) *\n");
        printf("*   Cikmak icin q ya basiniz.                                 *\n");
        printf("***************************************************************\n");
        printf("\n   Bir Secim  Yapiniz:");
        scanf("%s",secim);
        if(strcmp(secim,"q")==0)
        {
            exit(1);
        }
        result = atoi(secim);

        while(result == 0)// yani scanf hataliysa
        {

            printf("   Hatali bir secim  yapiniz lutfen tekrar secim yapiniz :");
            scanf("%s",secim);
            if(strcmp(secim,"q")==0)
            {
                exit(1);
            }
            result = atoi(secim);
        }

        switch(result)
        {
        case 1:
        {
            printf("   Sehir ismi: ");
            scanf("%s",sehirIsmi);
            printf("   Bolge: ");
            scanf("%s",bolge);
            printf("   Plaka kodu: ");
            scanf("%d",&plakaKodu);
            sehirEkle(sehirIsmi,bolge,plakaKodu); // sehirler eklendi.

            cikti = fopen("cikti.txt","a");
            fprintf(cikti,"---------EKLEME ISLEMİ--------\n");
            fprintf(cikti,"%s sehri eklendi.\n",sehirIsmi);
            printf("   Devam Etmek icin bir tusa basin ....");
            fclose(cikti);
            getch();// beklemek icin
            break;
        }
        case 2:
        {
            printf("   Komsu eklenecek sehir ismi: ");
            scanf("%s",sehirIsmi);
            printf("   Komsu sehrin ismi:");
            scanf("%s",komsuIsmi);
            komsulukEkle("cikti.txt",sehirIsmi,komsuIsmi,1);
            komsulukEkle("cikti.txt",komsuIsmi,sehirIsmi,1);
            printf("   Devam Etmek icin bir tusa basin ....");
            getch();// beklemek icin
            break;

        }
        case 3:
        {
            printf("   Silinecek sehir ismi : ");
            scanf("%s",sehirIsmi);
            sehirSil("cikti.txt",sehirIsmi);
            printf("   Devam Etmek icin bir tusa basin ....");
            getch();// beklemek icin
            break;
        }
        case 4:
        {

            printf("   Hangi sehrin komsusunu silmek istiyorsunuz ? : ");
            scanf("%s",sehirIsmi);
            printf("   Silinecek komsu ismini giriniz : ");
            scanf("%s",komsuIsmi);
            komsuSil("cikti.txt",sehirIsmi, komsuIsmi );
            komsuSil("cikti.txt",komsuIsmi, sehirIsmi );
            printf("   Devam Etmek icin bir tusa basin ....");
            getch();// beklemek icin
            break;
        }
        case 5:
        {
            printf("   Aramak istediginiz sehrin adi :");
            scanf("%s",sehirIsmi);
            cikti = fopen("cikti.txt","a");
            sehirNode *sonuc = sehirBul(sehirIsmi);
            fprintf(cikti,"\n----------- Sehir Arama -------\n");
            if(sonuc==NULL)
            {
                fprintf(cikti,"Kullanicin aradigi %s sehri bulunamadi\n",sehirIsmi);
                printf("   Eklemek istediginiz sehir bulunamadi\n");
                printf("   Sehri eklemek ister misiniz ? (E/H)");
                char temp[2];
                scanf("%s",temp);
                if(!strcmp("E",temp) || !strcmp("e",temp))
                {
                    printf("   Bolge: ");
                    scanf("%s",bolge);
                    printf("   Plaka kodu: ");
                    scanf("   %d",&plakaKodu);
                    sehirEkle(sehirIsmi,bolge,plakaKodu); // sehirler eklendi.
                    printf("   Sehir eklendi\n");
                    fprintf(cikti,"Kullanici sehri eklemeyi tercih etti ve sehir eklendi.\n");
                }
                else
                {
                    printf("Sehir eklenmedi.\n");
                    fprintf(cikti,"Kullanici sehri eklemeyi tercih etmedi.\n");
                }
            }
            else
            {
                fprintf(cikti,"%s sehir listesinde mevcut.\n",sehirIsmi);
                fprintf(cikti,"Sehrin adi: %s\n",sonuc->sehirIsmi);
                fprintf(cikti,"Plaka kodu : %d\n",sonuc->plakaKodu);
                fprintf(cikti,"Bolge: %s\n",sonuc->bolge);
                fprintf(cikti,"Komsuluk sayisi = %d\n",sonuc->komsuSayisi);
                fprintf(cikti,"Komsular:\n");
                printf("   Sehrin adi: %s\n",sonuc->sehirIsmi);
                printf("   Plaka kodu : %d\n",sonuc->plakaKodu);
                printf("   Bolge: %s\n",sonuc->bolge);
                printf("   Komsuluk sayisi = %d\n",sonuc->komsuSayisi);
                printf("   Komsular:");

                komsuNode *iter = sonuc->komsuRoot; // komsunun rootuna esitledik.

                if(iter == NULL)
                {
                    printf("   Hic bir komsusu yok.\n");
                    fprintf(cikti,"Hic bir komsusu yok.\n");
                }
                while(iter != NULL)
                {
                    printf(" %d ",iter->plakaKodu);
                    fprintf(cikti,"%d ",iter->plakaKodu);
                    iter= iter->next;
                }
                printf("\n");
                fprintf(cikti,"\n");
            }
            printf("   Devam Etmek icin bir tusa basin ....");
            getch();// beklemek icin
            fclose(cikti);
            break;
        }
        case 6 :
        {
            printf("   Bolgenin ismini giriniz:");
            scanf("%s",bolge);
            bolgeyeGoreListele("cikti.txt",bolge);
            printf("   Devam Etmek icin bir tusa basin ....");
            getch();// beklemek icin
            break;
        }
        case 7:
        {
            printf("   a= ");
            scanf("%d",&altSinir);
            printf("   b= ");
            scanf("   %d",&ustSinir);
            araligaGoreListele("cikti.txt",ustSinir,altSinir);
            printf("   Devam Etmek icin bir tusa basin ....");
            getch();// beklemek icin
            break;
        }
        case 8:
        {
            listeyiBastir();
            bellektekileriDosyayaYaz("cikti.txt");
            printf("   Devam Etmek icin bir tusa basin ....");
            getch();// beklemek icin
            break;
        }
        }

    }

    return 0;
}

void ekranTemizle() //isletim sistemine gore consol ekraninin calistirilmasi
{
// bu fonksiyon stackoverflowdan alinmistir.
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
    system("color  c");
#endif
}
void sehirEkle( char *sehirIsmi, char *bolge,int plakaKodu)
{
    if(sehirNodeRoot==NULL)
    {
        sehirNodeRoot = (sehirNode *)malloc(sizeof(sehirNode));
        strcpy(sehirNodeRoot->sehirIsmi,sehirIsmi);
        strcpy(sehirNodeRoot->bolge,bolge);
        sehirNodeRoot->komsuSayisi=0;
        sehirNodeRoot->plakaKodu=plakaKodu;
        sehirNodeRoot->sehirNext=NULL;
        sehirNodeRoot->sehirPrev=NULL;
        sehirNodeRoot->komsuRoot=NULL;
        sehirNodeEnd=sehirNodeRoot;

    }
    else
    {

        iter=sehirNodeRoot;
        sehirNode *newNode = (sehirNode *) malloc(sizeof(sehirNode));
        strcpy(newNode->sehirIsmi,sehirIsmi);
        strcpy(newNode->bolge,bolge);
        newNode->plakaKodu=plakaKodu;
        newNode->komsuRoot=NULL;
        newNode->komsuSayisi=0;

        while(iter!=NULL)
        {
            if(iter->plakaKodu==plakaKodu || stricmp(iter->sehirIsmi,sehirIsmi)==0)  // sehir ismi veya plaka kodu ayni ise
            {
                printf("   Varolan bir sehir ekleyemezsiniz....\n");
                return;
            }
            else if(iter->plakaKodu<plakaKodu && iter->sehirNext==NULL )// ekelenecek olan plaka iterin gezindigi plakadan buyukse.
            {
                iter->sehirNext=newNode;
                newNode->sehirNext=NULL;
                newNode->sehirPrev=iter;
                break;
            }
            else if(iter->plakaKodu>plakaKodu && iter==sehirNodeRoot )  // ekelenecek olan plaka iterin gezindigi plakadan kucukse.
            {
                sehirNode *temp=iter;
                sehirNodeRoot=newNode;
                newNode->sehirNext=temp;
                newNode->sehirPrev=NULL;
                iter->sehirPrev=newNode;
                break;

            }
            else if (iter->sehirNext->plakaKodu>plakaKodu && iter->sehirNext!=NULL)
            {

                sehirNode *temp=iter->sehirNext;
                iter->sehirNext=newNode;
                newNode->sehirNext=temp;
                temp->sehirPrev=newNode;
                break;
            }
            iter=iter->sehirNext;
        }

    }
}

void listeyiBastir()
{
    if(sehirNodeRoot==NULL)
    {
        printf("Liste bos");
    }
    else
    {
        iter=sehirNodeRoot;
        while(iter!=NULL)
        {
            printf("   Sehir ismi : %s\n",iter->sehirIsmi);
            printf("   plaka kodu : %d\n",iter->plakaKodu);
            printf("   bolge : %s\n",iter->bolge);
            printf("   sehrin komsuluk sayisi = %d\n",iter->komsuSayisi);
            printf("   Komsular :");
            komsuNode *komsuIter = iter->komsuRoot;
            while(komsuIter!=NULL)
            {
                printf("%d ",komsuIter->plakaKodu);
                komsuIter=komsuIter->next;
            }
            printf("\n\n");
            iter=iter->sehirNext;
        }
    }

}

int modelleme(char *sehirDosyaYolu,char *outputYol)
{


    FILE *sehirlerTxt = fopen(sehirDosyaYolu,"r");
    FILE *ciktiTxt = fopen(outputYol,"w");
    if(sehirlerTxt==NULL)
    {

        printf("\n\tSehirler txt dosyasi acilamadi. sehirler.txt dosyasi mevcut olmayabilir.");
        fprintf(ciktiTxt,"\n\tSehirler txt dosyasi acilamadi. sehirler.txt dosyasi mevcut olmayabilir.\n");
        getch();
        return -1 ;
    }
    if(outputYol==NULL)
    {

        printf("output dosyasi acilamadi.");
        getch();
        return -1 ;
    }
    char temp[5000]; // en fazla her satirda 5000 tane karakter olabilecegi varsayiliyor.
    int donulenDeger=-1;
    const char ayirac[2]=",";
    char *bolunenKelime;
    int plakaKodu;
    char sehirIsmi[100];
    char bolge[3];

    donulenDeger = (int) fgets(temp,5000,sehirlerTxt);

    while(donulenDeger!=0 )
    {

        bolunenKelime = strtok(temp,ayirac);
        plakaKodu =atoi(bolunenKelime);

        bolunenKelime = strtok(0,ayirac);
        strcpy(sehirIsmi,bolunenKelime);

        bolunenKelime = strtok(0,ayirac);
        strcpy(bolge,bolunenKelime);

        sehirEkle(sehirIsmi,bolge,plakaKodu); //  sehir ekledikten sonra komsuluk eklenecek.
        donulenDeger = (int)fgets(temp,5000,sehirlerTxt);

    }
    // sehirler eklendikten sonra komsuluklari ekleme islemi yapiliyor.
    fseek(sehirlerTxt,0,SEEK_SET);
    donulenDeger = (int) fgets(temp,5000,sehirlerTxt);
    while(donulenDeger!=0 )
    {

        bolunenKelime = strtok(temp,ayirac);
        plakaKodu =atoi(bolunenKelime);
        bolunenKelime = strtok(0,ayirac);
        strcpy(sehirIsmi,bolunenKelime);
        bolunenKelime = strtok(0,ayirac);
        strcpy(bolge,bolunenKelime);

        bolunenKelime = strtok(0,ayirac);

        if(bolunenKelime == 0)  // eger sehrin komsulugu yoksa bir sonraki sehire geciliyor.
        {
            donulenDeger = (int)fgets(temp,5000,sehirlerTxt);
            continue;
        }
        sehirNode *eklenecekSehir = sehirBul(sehirIsmi);
        sehirNode *eklenecekKomsu =NULL;
        while(bolunenKelime!=0)
        {

            eklenecekKomsu = sehirBul(bolunenKelime);
            if(eklenecekSehir!=NULL && eklenecekKomsu!=NULL)
            {
                komsulukEkle("cikti.txt",sehirIsmi,bolunenKelime,0); // komsuluk eklendi.
            }
            bolunenKelime = strtok(0,ayirac);
        }

        donulenDeger = (int)fgets(temp,5000,sehirlerTxt);

    }
    sehirNode *sehirIter = sehirNodeRoot;
    komsuNode *komsuIter = NULL;
    int i;// dongu icin kullanilacak.
    fprintf(ciktiTxt,"<---sehirler.txt dosyadan okunan sehir bilgileri--->\n");
    while(sehirIter!=NULL)
    {
        fprintf(ciktiTxt,"   Plaka Kodu : %d \n",sehirIter->plakaKodu);
        fprintf(ciktiTxt,"   Sehir Ismi : %s ",sehirIter->sehirIsmi);
        komsuIter=sehirIter->komsuRoot;
        while(komsuIter!=NULL)
        {
            fprintf(ciktiTxt,"-----> %d ",komsuIter->plakaKodu);
            komsuIter=komsuIter->next;
        }
        fprintf(ciktiTxt,"\n");
        fprintf(ciktiTxt,"   Bolge Ismi : %s\n",sehirIter->bolge);
        fprintf(ciktiTxt,"   Komsu Sayisi : %d\n\t^\n",sehirIter->komsuSayisi);
        for(i=0; i<3; i++)
        {
            fprintf(ciktiTxt,"\t| |\n");
        }
        fprintf(ciktiTxt,"\tv\n");
        sehirIter=sehirIter->sehirNext;
    }
    fclose(sehirlerTxt);
    fclose(ciktiTxt);
    return 1;
}

void komsulukEkle(char *ciktiYolu,char *sehirIsmi,char *komsuIsmi,int bayrak)
{

    FILE *cikti = fopen(ciktiYolu,"a");
    if(cikti == NULL)
    {
        printf("Dosya acarken hata (komsuluk ekle)");
        return;
    }
    fprintf(cikti,"---------Komsuluk EKLEME ISLEMİ--------\n");
    sehirNode * komsuEklenecekSehir = sehirBul(sehirIsmi);
    sehirNode *komsu=sehirBul(komsuIsmi);


    if(komsuEklenecekSehir== NULL )
    {
        printf("   Komsu eklemek istediginiz sehir bellekte yok !\n");
        fprintf(cikti,"Komsu eklemek istediginiz sehir bellekte yok !\n");
        fclose(cikti);
        return;
    }
    if(komsu == NULL)
    {
        printf("   Komsu eklenecek sehir bulunamadi!\n");
        fprintf(cikti,"Komsu eklenecek sehir bulunamadi!\n");
        fclose(cikti);
        return;
    }
    komsuNode *yeniKomsu =(komsuNode*) malloc(sizeof(komsuNode)); // yeni bir komsu olusturduk.
    yeniKomsu->plakaKodu=komsu->plakaKodu;


    if(komsuEklenecekSehir->komsuRoot==NULL)  // eger hic komsu yoksa
    {
        komsuEklenecekSehir->komsuRoot=yeniKomsu;
        yeniKomsu->next=NULL;
        komsuEklenecekSehir->komsuSayisi = komsuEklenecekSehir->komsuSayisi +1 ;

    }
    else // birden fazla komsu varsa
    {
        komsuNode *iter=komsuEklenecekSehir->komsuRoot;


        while(iter!=NULL)  // komsu listesinin sonuna gelmemissek don.
        {
            if (komsu->plakaKodu==iter->plakaKodu )
            {
                printf("   ayni komsuluk bir daha eklenemez!!\n");
                fprintf(cikti,"ayni komsuluk bir daha eklenemez!!\n");
                fclose(cikti);
                return;

            }
            else if (komsu->plakaKodu==komsuEklenecekSehir->plakaKodu)
            {
                printf("   sehir kendisiyle komsu olamaz!!!\n");
                fprintf(cikti,"sehir kendisiyle komsu olamaz!!!\n");
                fclose(cikti);
                return;
            }
            else if(komsu->plakaKodu < iter->plakaKodu && iter==komsuEklenecekSehir->komsuRoot)
            {
                komsuNode *temp=komsuEklenecekSehir->komsuRoot;
                komsuEklenecekSehir->komsuRoot=yeniKomsu;
                yeniKomsu->next=temp;
                komsuEklenecekSehir->komsuSayisi = komsuEklenecekSehir->komsuSayisi +1 ;
                break;
            }
            else if (komsu->plakaKodu>iter->plakaKodu && iter->next==NULL)
            {

                iter->next=yeniKomsu;
                yeniKomsu->next=NULL;
                komsuEklenecekSehir->komsuSayisi = komsuEklenecekSehir->komsuSayisi +1 ;
                break;
            }
            else if (iter->next->plakaKodu>komsu->plakaKodu && iter->next!=NULL)
            {

                komsuNode *temp=iter->next;
                iter->next=yeniKomsu;
                yeniKomsu->next=temp;
                komsuEklenecekSehir->komsuSayisi = komsuEklenecekSehir->komsuSayisi +1 ;
                break;
            }
            iter=iter->next;
        }

    }
    if(bayrak)
    {
        printf("%s nin komsusu %s olarak eklendi.\n",komsuEklenecekSehir->sehirIsmi,komsu->sehirIsmi);
        fprintf(cikti,"%s nin komsusu %s olarak eklendi.\n",komsuEklenecekSehir->sehirIsmi,komsu->sehirIsmi);
    }
    fclose(cikti);
}
void sehirSil(char *outputPath, char *sehirIsmi)
{

    FILE *cikti = fopen(outputPath, "a");
    if(cikti==NULL)
    {
        printf("   Silme isleminde output dosyasi acilamadi!\n");
        exit(1);
    }

    fprintf(cikti,"\n---Silme islemleri---\n");
    if(sehirNodeRoot==NULL)
    {
        printf("   Liste bos silinecek sehir yok!\n");
        fprintf(cikti,"Liste bos silinecek sehir yok!\n");
        fclose(cikti);
        return ;
    }

    sehirNode * silinecekSehir = sehirBul(sehirIsmi);
    if(silinecekSehir==NULL)
    {
        printf("Silmek istediginiz %s sehri listede yoktur!\n",sehirIsmi);
        fprintf(cikti,"Silmek istediginiz %s sehri listede yoktur!\n",sehirIsmi);
        fclose(cikti);
        return;
    }
    else
    {
        sehirNode *temp ;
        iter=sehirNodeRoot;
        if(stricmp(iter->sehirIsmi, silinecekSehir->sehirIsmi)==0 && iter==sehirNodeRoot)
        {

            temp = iter;
            sehirNodeRoot = sehirNodeRoot->sehirNext;
            sehirNodeRoot->sehirPrev=NULL;
            free(temp);
            printf("   %s sehri silindi(1).\n",sehirIsmi);
            fprintf(cikti,"Silmek istediginiz %s sehri silindi!\n",sehirIsmi);
            fclose(cikti);
            return;
        }
        while(stricmp(iter->sehirNext->sehirIsmi,sehirIsmi)!=0 && iter->sehirNext!=NULL)
        {
            iter = iter->sehirNext;
        }

        if(iter->sehirNext==NULL)
        {
            printf("   Silmek istediginiz sehir sehir bulunamadi!\n");
            fprintf(cikti,"Silmek istediğiniz %s sehri bulunamadi!\n",sehirIsmi);
            fclose(cikti);
            return;
        }
        temp = iter->sehirNext;
        iter ->sehirNext = temp->sehirNext;
        if(iter->sehirNext!=NULL)
            iter->sehirNext->sehirPrev = temp->sehirPrev;

        free(temp);
        printf("   %s sehri silindi(2).\n",sehirIsmi);
        fprintf(cikti,"Silmek istediginiz %s sehri silindi.\n",sehirIsmi);

    }
    fclose(cikti);

}

void komsuSil(char * outputPath,char * sehirIsmi, char * komsuIsmi)
{
    FILE *cikti  =fopen(outputPath, "a");
    fprintf(cikti,"\n---Silme islemleri---\n");
    if(cikti==NULL)
    {
        printf("Output dosyasi komsu silmek icin acilamadi!\n");
        return;
    }
    sehirNode * komsusuSilinecekSehirDugum = sehirBul(sehirIsmi);
    sehirNode * silinecekKomsuDugumu = sehirBul(komsuIsmi);
    if(komsusuSilinecekSehirDugum==NULL || silinecekKomsuDugumu==NULL)
    {
        printf("   Komsunu silmek istediginiz %s sehrine veya %s isimli komsusuna ait sehir ismi bilgisi yanlistir.\n Sehirler Bellekten silinmis de olabilir.!\n",sehirIsmi,komsuIsmi);
        fprintf(cikti,"Komsunu silmek istediginiz %s sehrine veya %s isimli komsusuna ait sehir ismi bilgisi yanlistir!n Sehirler Bellekten silinmis de olabilir.\n",sehirIsmi,komsuIsmi);
        fclose(cikti);
        return;
    }
    if(komsusuSilinecekSehirDugum->komsuSayisi==0)
    {
        printf("   Komsunu silmek istediginiz %s sehrinin hicbir komsusu yoktur!\n",komsusuSilinecekSehirDugum->sehirIsmi);
        fprintf(cikti,"Komsunu silmek istediginiz %s sehrinin hicbir komsusu yoktur!\n",komsusuSilinecekSehirDugum->sehirIsmi);
        fclose(cikti);
        return;
    }
    else
    {
        komsuNode *_iter = komsusuSilinecekSehirDugum->komsuRoot; //komsulari dolasmak icin.
        if(_iter->plakaKodu == silinecekKomsuDugumu->plakaKodu)
        {
            komsuNode * temp = _iter;
            komsusuSilinecekSehirDugum->komsuRoot = _iter->next;
            komsusuSilinecekSehirDugum->komsuSayisi = komsusuSilinecekSehirDugum->komsuSayisi - 1;
            free(temp);
            printf("   Silmek istediginiz %s sehrinin komsu olan %s sehri silindi!\n",sehirIsmi,komsuIsmi);
            fprintf(cikti,"Silmek istediginiz %s sehrinin komsu olan %s sehri silindi!\n",sehirIsmi,komsuIsmi);
            fclose(cikti);
            return;
        }
        while(_iter->next->plakaKodu!=silinecekKomsuDugumu->plakaKodu && _iter->next!=NULL)
        {
            _iter=_iter->next;
        }
        if(_iter->next==NULL)
        {
            printf("   Silmek istediginiz %s sehri %s sehrinin komsusu degil, bu yuzden silinemedi!\n",silinecekKomsuDugumu->sehirIsmi,komsusuSilinecekSehirDugum->sehirIsmi);
            fprintf(cikti,"Silmek istediginiz %s sehri %s sehrinin komsusu degil, bu yuzden silinemedi!\n",silinecekKomsuDugumu->sehirIsmi,komsusuSilinecekSehirDugum->sehirIsmi);
            fclose(cikti);
            return;
        }

        komsuNode *temp = _iter->next;
        _iter->next= temp->next;
        komsusuSilinecekSehirDugum->komsuSayisi = komsusuSilinecekSehirDugum->komsuSayisi - 1;
        free(temp);
        printf("   %s sehrinin komsusu olan %s sehri silindi!\n",komsusuSilinecekSehirDugum->sehirIsmi,silinecekKomsuDugumu->sehirIsmi);
        fprintf(cikti,"%s sehrinin komsusu olan %s sehri silindi!\n",komsusuSilinecekSehirDugum->sehirIsmi,silinecekKomsuDugumu->sehirIsmi);

    }
    fclose(cikti);
}

sehirNode* sehirBul(char *sehirIsmi) // ismi girlen sehrin adresini dondurur eger sehir listede yoksa null dondurur.
{

    sehirNode *iterListe = sehirNodeRoot;
    while(iterListe!=NULL)
    {
        if (sehirIsmi[strlen(sehirIsmi)-1]=='\n') // eger satir sonundaysa \n icerdigi icin sorun olusturdugu icin boyle  bir sey yaptik.
        {
            char temp[strlen(sehirIsmi)-1];
            strncpy(temp,sehirIsmi,strlen(sehirIsmi)-1);
            temp[strlen(sehirIsmi)-1]='\0';

            strcpy(sehirIsmi,temp);
        }

        if(stricmp(sehirIsmi,iterListe->sehirIsmi)==0)
        {
            return iterListe;
        }
        iterListe=iterListe->sehirNext;
    }

    return NULL;
}


void bolgeyeGoreListele(char *output,char *bolge)
{
    FILE *cikti = fopen(output,"a");
    if(cikti==NULL)
    {
        printf("   dosya acilamadi!!!");
        exit(1);
    }
    fprintf(cikti,"\n---%s bölgesinde bulunan şehirlerin bilgileri----\n",bolge);



    sehirNode *iter = sehirNodeRoot;
    int sehirSayisi=0;

    while(iter!=NULL)
    {

        if( stricmp(iter->bolge,bolge) == 0 ) // eslesen bolge varsa sehir bilgileri dosyaya yazdiriliyor.
        {
            fprintf(cikti,"Sehir ismi : %s\n",iter->sehirIsmi);
            fprintf(cikti,"plaka kodu : %d\n",iter->plakaKodu);
            fprintf(cikti,"bolge : %s\n",iter->bolge);
            fprintf(cikti,"sehrin komsuluk sayisi = %d\n",iter->komsuSayisi);
            fprintf(cikti,"Komsular :\n");

            komsuNode *komsuIter = iter->komsuRoot;
            while(komsuIter!=NULL)
            {
                fprintf(cikti,"%d ",komsuIter->plakaKodu);
                komsuIter=komsuIter->next;
            }
            fprintf(cikti,"\n\n");
            sehirSayisi++;

        }

        iter =iter->sehirNext;
    }

    if(sehirSayisi==0)
    {
        fprintf(cikti,"Boyle bir bolge mevcut değil\n");
    }

    printf("   Islem sonucu cikti.txt ye yazdirildi.\n");
    fclose(cikti);
}
// verilen ust ve alt sinir araliginda komsu sayisina sahip sehirleri listeler
void araligaGoreListele(char *output,int ustSinir, int altSinir)
{

    iter = sehirNodeRoot;
    int sehirSayisi=0;
    FILE *cikti = fopen(output,"a");
    if(cikti==NULL)
    {
        printf("dosya acilamadi!!!");
        exit(1);
    }

    fprintf(cikti,"--- %d-%d arasinda komsu sayisina sahip olan sehirler ---\n",altSinir,ustSinir);

    while(iter!=NULL)
    {
        if(iter->komsuSayisi<=ustSinir && iter->komsuSayisi>=altSinir)
        {
            fprintf(cikti," %d-%s,%s,%d\n",iter->komsuSayisi,iter->sehirIsmi,iter->bolge,iter->plakaKodu);
            sehirSayisi++;
        }
        iter=iter->sehirNext;
    }
    if(!sehirSayisi)
    {
        fprintf(cikti," %d-%d arasinda komsu sayisina sahip olan sehirler bulunamadi",altSinir,ustSinir);
    }
    printf("   Islemin sonucu cikti.txt ye yazdirildi.\n");
    fclose(cikti); // dosya kapatiliyor.
}

void bellektekileriDosyayaYaz(char *output)
{
    FILE *cikti = fopen(output,"a");
    if(cikti==NULL)
    {
        printf("Dosya acilamadi\n");
        return; // fonksiyon bitiriliyor.
    }
    sehirNode *iter=sehirNodeRoot;
    fprintf(cikti,"\n--------Sehir Bilgileri --------\n");
    while(iter!=NULL)
    {
        fprintf(cikti,"Sehir ismi : %s\n",iter->sehirIsmi);
        fprintf(cikti,"plaka kodu : %d\n",iter->plakaKodu);
        fprintf(cikti,"bolge : %s\n",iter->bolge);
        fprintf(cikti,"sehrin komsuluk sayisi = %d\n",iter->komsuSayisi);
        fprintf(cikti,"Komsular :\n");

        komsuNode *komsuIter = iter->komsuRoot;
        while(komsuIter!=NULL)
        {
            fprintf(cikti,"%d ",komsuIter->plakaKodu);
            komsuIter=komsuIter->next;
        }
        fprintf(cikti,"\n\n");

        iter =iter->sehirNext;
    }

    fclose(cikti);
}
