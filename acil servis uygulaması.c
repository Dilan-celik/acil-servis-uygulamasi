#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ILAC 5
#define MAX_HASTA 100
#define MAX_ISLEM 50
#define MAX_RANDEVU 30
#define HL7_BUFFER_SIZE 1024

const char *bolumler[] = {"Dahiliye", "Cerrahi", "Kardiyoloji", "Ortopedi", "Noroloji", "Pediatri"};
const char *erkek_doktorlar[] = {"Erkek Doktor 1", "Erkek Doktor 2", "Erkek Doktor 3"};
const char *kadin_doktorlar[] = {"Kadin Doktor 1", "Kadin Doktor 2", "Kadin Doktor 3"};

typedef struct {
    char isim[50];
    char doz[20];
    char kullanim[100];
} Ilac;

typedef struct {
    char ad[50];
    int yas;
    char cinsiyet;
    char semptomlar[100];
    char alan[10];
    int recete_no;
    int oncelik;
    char bolum[20];
    char doktor[30];
    int bekleme_suresi;
    char tibbi_gecmis[200];
    char mevcut_ilaclar[200];
    char alerjiler[100];
    char onceki_ziyaretler[200];
    char acil_durum_kisi[50];
    char acil_durum_telefon[15];
    char test_sonuclari[200];
    char muayene_notu[300];
    char tetkikler[200];
    char teshis[100];
    Ilac yazilan_ilaclar[MAX_ILAC];
    int ilac_sayisi;
    char hasta_durumu[20]; // Bekleme, Muayene, Tetkik, Taburcu
    char taburcu_notu[200];
    time_t giris_zamani;
    char tc_kimlik[12];
    float ucret;
    int odeme_durumu; // 0: Odenmedi, 1: Odendi
    char hasta_id[20]; // HL7 uyumlu ID
} Hasta;

typedef struct {
    int id;
    char hasta_ad[50];
    char islem_turu[50];
    char detay[100];
    time_t zaman;
    char kullanici[30];
} IslemKaydi;

typedef struct {
    int id;
    char hasta_ad[50];
    char tc_kimlik[12];
    char doktor[30];
    char bolum[20];
    char tarih[20];
    char saat[10];
    int durum; // 0: Bekliyor, 1: Tamamlandi, 2: Iptal
} Randevu;

Hasta hastalar[MAX_HASTA];
int hasta_sayisi = 0;
IslemKaydi islem_kayitlari[MAX_ISLEM];
int islem_sayisi = 0;
Randevu randevular[MAX_RANDEVU];
int randevu_sayisi = 0;

int recete_no_uret() {
    return rand() % 9000 + 1000;
}

void hl7_mesaj_olustur(char *mesaj, Hasta *hasta) {
    time_t simdi = time(NULL);
    struct tm *tm_bilgi = localtime(&simdi);
    char tarih[15], saat[15], hasta_id[20];
    
    strftime(tarih, sizeof(tarih), "%Y%m%d", tm_bilgi);
    strftime(saat, sizeof(saat), "%H%M%S", tm_bilgi);
    
    sprintf(hasta_id, "%s-%d", hasta->tc_kimlik, hasta->yas);
    strcpy(hasta->hasta_id, hasta_id);
    
    sprintf(mesaj, 
        "MSH|^~\\&|ACILSERVIS|HASTANE|HIS|MERKEZ|%s%s||ADT^A01|%d|P|2.3\r"
        "EVN|A01|%s%s|||\r"
        "PID|1||%s||%s||%d|%c|||||||%s|||||||\r"
        "PV1|1|E|%s|%s||||%s||||||||||||%s||||||||||||||||||||%s%s||||||\r",
        tarih, saat, rand() % 1000000,
        tarih, saat,
        hasta_id, hasta->ad, hasta->yas, hasta->cinsiyet, hasta->acil_durum_telefon,
        hasta->alan, hasta->bolum, hasta->doktor, hasta->alan, tarih, saat
    );
}

void hl7_mesaj_kaydet(char *mesaj, char *hasta_ad) {
    char dosya_ismi[100];
    sprintf(dosya_ismi, "hl7_%s_%ld.txt", hasta_ad, time(NULL));
    
    FILE *fp = fopen(dosya_ismi, "w");
    if (fp != NULL) {
        fprintf(fp, "%s", mesaj);
        fclose(fp);
        printf("HL7 mesaji '%s' olarak kaydedildi.\n", dosya_ismi);
    } else {
        printf("HL7 mesaji kaydedilemedi!\n");
    }
}

void log_kaydet(char *hasta_ad, char *islem_turu, char *detay, char *kullanici) {
    int i;
    if (islem_sayisi >= MAX_ISLEM) {
        for (i = 0; i < MAX_ISLEM - 1; i++) {
            islem_kayitlari[i] = islem_kayitlari[i+1];
        }
        islem_sayisi = MAX_ISLEM - 1;
    }
    
    IslemKaydi yeni_islem;
    yeni_islem.id = islem_sayisi + 1;
    strcpy(yeni_islem.hasta_ad, hasta_ad);
    strcpy(yeni_islem.islem_turu, islem_turu);
    strcpy(yeni_islem.detay, detay);
    yeni_islem.zaman = time(NULL);
    strcpy(yeni_islem.kullanici, kullanici);
    
    islem_kayitlari[islem_sayisi] = yeni_islem;
    islem_sayisi++;
}

void bolum_belirle(Hasta *hasta) {
    char doktor_secimi;
    printf("Doktor cinsiyet tercihiniz var mi? (E: Erkek, K: Kadin, X: Farketmez): ");
    scanf(" %c", &doktor_secimi);

    if (strstr(hasta->semptomlar, "kalp")) {
        strcpy(hasta->bolum, bolumler[2]);
    } else if (strstr(hasta->semptomlar, "kirik") || strstr(hasta->semptomlar, "cikik")) {
        strcpy(hasta->bolum, bolumler[3]);
    } else if (strstr(hasta->semptomlar, "bas agrisi") || strstr(hasta->semptomlar, "felc")) {
        strcpy(hasta->bolum, bolumler[4]);
    } else if (strstr(hasta->semptomlar, "cocuk")) {
        strcpy(hasta->bolum, bolumler[5]);
    } else {
        strcpy(hasta->bolum, bolumler[0]);
    }

    if (doktor_secimi == 'E' || doktor_secimi == 'e') {
        strcpy(hasta->doktor, erkek_doktorlar[rand() % 3]);
    } else if (doktor_secimi == 'K' || doktor_secimi == 'k') {
        strcpy(hasta->doktor, kadin_doktorlar[rand() % 3]);
    } else {
        if (rand() % 2) {
            strcpy(hasta->doktor, erkek_doktorlar[rand() % 3]);
        } else {
            strcpy(hasta->doktor, kadin_doktorlar[rand() % 3]);
        }
    }
}

void alan_belirle(Hasta *hasta) {
    if (hasta->yas >= 65 || hasta->yas <= 7 || strstr(hasta->semptomlar, "hamile")) {
        hasta->oncelik = 1;
        strcpy(hasta->alan, "Kirmizi");
        hasta->bekleme_suresi = 10;
    } else if (strstr(hasta->semptomlar, "kalp krizi") || strstr(hasta->semptomlar, "nefes darligi")) {
        strcpy(hasta->alan, "Kirmizi");
        hasta->bekleme_suresi = 10;
        hasta->oncelik = 1;
    } else if (strstr(hasta->semptomlar, "kirik") || strstr(hasta->semptomlar, "cikik")) {
        strcpy(hasta->alan, "Sari");
        hasta->bekleme_suresi = 30;
        hasta->oncelik = 0;
    } else if (strstr(hasta->semptomlar, "ates") || strstr(hasta->semptomlar, "bas agrisi")) {
        strcpy(hasta->alan, "Yesil");
        hasta->bekleme_suresi = 60;
        hasta->oncelik = 0;
    } else {
        strcpy(hasta->alan, "Yesil");
        hasta->bekleme_suresi = 60;
        hasta->oncelik = 0;
    }

    bolum_belirle(hasta);
}

void hasta_ekle() {
    if (hasta_sayisi >= MAX_HASTA) {
        printf("Hasta kapasitesi doldu!\n");
        return;
    }

    Hasta yeni_hasta;
    printf("Hasta adini girin: ");
    scanf(" %49[^\n]", yeni_hasta.ad);
    printf("TC Kimlik No girin: ");
    scanf(" %11[^\n]", yeni_hasta.tc_kimlik);
    printf("Yasini girin: ");
    scanf("%d", &yeni_hasta.yas);
    printf("Cinsiyet (E/K): ");
    scanf(" %c", &yeni_hasta.cinsiyet);
    printf("Semptomlarini girin: ");
    scanf(" %99[^\n]", yeni_hasta.semptomlar);

    printf("Tibbi gecmisi girin: ");
    scanf(" %199[^\n]", yeni_hasta.tibbi_gecmis);
    printf("Mevcut ilaclari girin: ");
    scanf(" %199[^\n]", yeni_hasta.mevcut_ilaclar);
    printf("Alerjilerini girin: ");
    scanf(" %99[^\n]", yeni_hasta.alerjiler);
    printf("Onceki hastane ziyaretlerini girin: ");
    scanf(" %199[^\n]", yeni_hasta.onceki_ziyaretler);
    printf("Acil durumda aranacak kisiyi girin: ");
    scanf(" %49[^\n]", yeni_hasta.acil_durum_kisi);
    printf("Acil durum telefonu: ");
    scanf(" %14[^\n]", yeni_hasta.acil_durum_telefon);
    printf("Test sonuclarini girin (varsa): ");
    scanf(" %199[^\n]", yeni_hasta.test_sonuclari);
    
    alan_belirle(&yeni_hasta);
    yeni_hasta.recete_no = recete_no_uret();
    yeni_hasta.bekleme_suresi = yeni_hasta.oncelik ? 20 : yeni_hasta.bekleme_suresi;
    yeni_hasta.ilac_sayisi = 0;
    strcpy(yeni_hasta.hasta_durumu, "Bekleme");
    strcpy(yeni_hasta.muayene_notu, "");
    strcpy(yeni_hasta.tetkikler, "");
    strcpy(yeni_hasta.teshis, "");
    strcpy(yeni_hasta.taburcu_notu, "");
    yeni_hasta.giris_zamani = time(NULL);
    yeni_hasta.ucret = 0.0;
    yeni_hasta.odeme_durumu = 0;

    char hl7_mesaj[HL7_BUFFER_SIZE];
    hl7_mesaj_olustur(hl7_mesaj, &yeni_hasta);
    hl7_mesaj_kaydet(hl7_mesaj, yeni_hasta.ad);

    int i;
    for (i = hasta_sayisi; i > 0 && hastalar[i - 1].oncelik < yeni_hasta.oncelik; i--) {
        hastalar[i] = hastalar[i - 1];
    }
    hastalar[i] = yeni_hasta;
    hasta_sayisi++;

    printf("\nHasta Bilgileri:\n");
    printf("Adi: %s\n", yeni_hasta.ad);
    printf("TC Kimlik: %s\n", yeni_hasta.tc_kimlik);
    printf("Yasi: %d\n", yeni_hasta.yas);
    printf("Cinsiyet: %c\n", yeni_hasta.cinsiyet);
    printf("Semptomlari: %s\n", yeni_hasta.semptomlar);
    printf("Yonlendirildigi Alan: %s\n", yeni_hasta.alan);
    printf("Bolum: %s\n", yeni_hasta.bolum);
    printf("Doktor: %s\n", yeni_hasta.doktor);
    printf("Recete No: %d\n", yeni_hasta.recete_no);
    printf("Oncelik Durumu: %s\n", yeni_hasta.oncelik ? "Oncelikli" : "Onceliksiz");
}

int main() {
    srand(time(NULL));
    int secim;

    while (1) {
        printf("\n1. Hasta Ekle\n2. Cikis\nSeciminiz: ");
        scanf("%d", &secim);
        if (secim == 1) {
            hasta_ekle();
        } else if (secim == 2) {
            break;
        } else {
            printf("Gecersiz secim!\n");
        }
    }

    return 0;
}
