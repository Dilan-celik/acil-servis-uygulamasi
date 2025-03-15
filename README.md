# acil-servis-uygulamasi
Bu proje, acil servislerde hastaların durumlarını takip etmek, randevuları yönetmek ve HL7 formatında hasta bilgilerini oluşturmak için geliştirilmiş bir C programıdır. Program, hastaların bilgilerini saklamak ve işlemek için yapılandırılmış veri yapıları kullanmaktadır.

Kod Açıklaması
1. Tanımlamalar ve Değişkenler
Kodun başında gerekli kütüphaneler ve sabit değişkenler tanımlanmıştır.

MAX_ILAC, MAX_HASTA, MAX_ISLEM, MAX_RANDEVU: Maksimum hasta, ilaç, işlem ve randevu sayısını belirleyen sabitlerdir.
bolumler, erkek_doktorlar, kadin_doktorlar: Hastaların yönlendirileceği bölümler ve doktorların isimleri için diziler kullanılmıştır.
Veri yapıları:
Ilac: Hastalara yazılan ilaç bilgilerini içerir.
Hasta: Hastanın kişisel ve tıbbi bilgilerini içeren temel veri yapısıdır.
IslemKaydi: Yapılan işlemleri saklar (örn: hasta kaydı, muayene vb.).
Randevu: Hastaların randevu bilgilerini saklayan yapıdır.
2. Fonksiyonlar ve Görevleri
a) recete_no_uret()
Rastgele bir reçete numarası oluşturur.

b) hl7_mesaj_olustur()
HL7 (Health Level 7) formatında hasta bilgilerini içeren bir mesaj oluşturur.
Hastanın TC kimlik numarasını ve yaşını kullanarak HL7 uyumlu bir hasta ID’si üretir.
Mesaj, hastanın adı, acil durum telefon numarası, doktor ve bölüm bilgilerini içerir.


c) hl7_mesaj_kaydet()
Oluşturulan HL7 mesajını bir dosyaya kaydeder.
Dosya adı, hasta adı ve kayıt zamanına göre belirlenir.


d) log_kaydet()
Yapılan işlemleri bir diziye kaydeder.
Eğer işlem kaydı kapasitesi dolarsa, en eski kayıt silinir ve yeni kayıt eklenir.


e) bolum_belirle()
Hastanın semptomlarına göre uygun bir bölüm belirler.
Hastanın tercihine göre erkek veya kadın doktor atanır.


f) alan_belirle()
Hastanın durumuna göre hangi acil servis alanına yönlendirileceğini belirler:
Kırmızı alan: Kritik hastalar (örn: yaşlılar, kalp krizi hastaları).
Sarı alan: Orta düzeyde acil vakalar (örn: kırık, çıkık).
Yeşil alan: Daha az acil vakalar (örn: hafif baş ağrısı, ateş).
Hastanın bekleme süresini hesaplar.


g) hasta_ekle()
Kullanıcıdan hastaya ait bilgileri alır.
Hastanın semptomlarına göre önceliğini ve doktorunu belirler.
HL7 mesajı oluşturup kaydeder.
Hastayı bekleme listesine uygun bir sırada ekler (öncelikli hastalar üstte olur).
Hasta bilgilerini ekrana yazdırır.


H-) main() Fonksiyonu
Programın ana döngüsünü yönetir.
Kullanıcıya menü sunar:
Hasta ekleme seçeneği: Yeni bir hasta ekler.
Çıkış seçeneği: Programı sonlandırır.


Çalışma Prensibi
Kullanıcı programı başlattığında bir menü görüntülenir.
Hasta ekleme işlemi seçildiğinde, kullanıcıdan hasta bilgileri alınır.
Hasta bilgileri işlenerek uygun doktor ve bölüm belirlenir.
HL7 formatında bir mesaj oluşturularak dosyaya kaydedilir.
Hasta listesi, önceliğe göre sıralı şekilde güncellenir.
İşlem kaydı tutulur.
Program kullanıcıdan yeni bir işlem seçmesini ister veya çıkış yapmasını bekler.
Sonuç
Bu program, acil servislerde hasta yönetimini kolaylaştırmak için geliştirilmiştir. Hasta bilgilerini saklama, öncelik sıralaması yapma, doktor atama, HL7 mesajı oluşturma ve işlem kayıtlarını yönetme gibi önemli özelliklere sahiptir.
