## Proje Adı
ACİL SERVİS UYGULAMASI




## Amaç
Hasta kabul sistemi, hastaların bilgilerini kaydetmek, HL7 uyumlu mesajlar oluşturmak ve bu mesajları dosyaya kaydetmek için geliştirilmiştir. Sistem ayrıca işleme kayıtlarını ve hastaların önceliklerine göre yönlendirmelerini de yapmaktadır.

## Kapsam
Bu proje, hastaların kabul sürecini yönetmek, doktor ve bölüm atamalarını otomatikleştirmek ve hasta bilgilerini HL7 formatında saklamak için tasarlanmıştır.

## Gerekli Kaynaklar
- C Programlama Dili
- HL7 Standartları
- Dosya İşlemleri
- Rastgele Sayı Üretimi

## Fonksiyonlar ve Çalışma Mantığı

### 1. `recete_no_uret`
- Rastgele bir reçete numarası üretir.

### 2. `hl7_mesaj_olustur`
- Hasta bilgilerine göre HL7 formatında bir mesaj oluşturur.

### 3. `hl7_mesaj_kaydet`
- Oluşturulan HL7 mesajını bir dosyaya kaydeder.

### 4. `log_kaydet`
- İşlem kayıtlarını tutar. Eğer maksimum kayıt sayısına ulaşılmışsa en eski kaydı siler.

### 5. `bolum_belirle`
- Hastanın semptomlarına ve cinsiyet tercihlerine göre bir doktor ve bölüm atar.

### 6. `alan_belirle`
- Hastanın yaşına ve semptomlarına göre bir alan (kırmızı, sarı, yeşil) belirler.

### 7. `hasta_ekle`
- Yeni bir hasta ekler, tüm bilgilerini toplar ve HL7 formatında bir mesaj oluşturur ve kaydeder. Hastayı öncelik sırasına göre diziye ekler.

### 8. `main`
- Kullanıcıdan giriş alır ve hasta ekleme veya çıkış işlemlerini yönetir.

## Akış Şeması

### Hasta Ekleme Süreci
1. Kullanıcıdan hasta bilgileri alınır.
2. Hastanın önceliğine göre alan belirlenir.
3. Hastanın semptomlarına göre bölüm ve doktor atanır.
4. HL7 formatında mesaj oluşturulur ve dosyaya kaydedilir.
5. Hasta bilgileri öncelik sırasına göre diziye eklenir.
6. İşlem kayıtları güncellenir.

PROJE ANALİZ DÖKÜMANI


