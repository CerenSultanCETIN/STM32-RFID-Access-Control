using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace manyetik_giris_karti
{
    public static class VeriYoneticisi
    {
        static string yol = Application.StartupPath;
        static string dosyaKullanicilar = yol + "\\kullanicilar.txt";
        static string dosyaLoglar = yol + "\\loglar.txt";
        static string dosyaYonetici = yol + "\\yonetici.txt";

        // Bellekteki giriş/çıkış listesi
        public static Dictionary<string, bool> Iceridekiler = new Dictionary<string, bool>();

        static VeriYoneticisi()
        {
            DosyaKontrol(dosyaKullanicilar);
            DosyaKontrol(dosyaLoglar);
            DosyaKontrol(dosyaYonetici);
        }

        private static void DosyaKontrol(string dosyaYolu)
        {
            if (!File.Exists(dosyaYolu)) File.Create(dosyaYolu).Close();
        }

        // ID TEMİZLEYİCİ (Görünmez karakterleri yok eder)
        private static string IDTemizle(string hamID)
        {
            if (string.IsNullOrEmpty(hamID)) return "";
            return hamID.Trim().Replace("\r", "").Replace("\n", "").Replace("\t", "").Replace("\0", "").ToUpper();
        }

        public static int KullaniciSil(string kartID)
        {
            string silinecekID = IDTemizle(kartID);

            // 1. RAM'den (Hafızadan) Sil - BU ÇOK ÖNEMLİ
            if (Iceridekiler.ContainsKey(silinecekID))
            {
                Iceridekiler.Remove(silinecekID);
            }

            if (!File.Exists(dosyaKullanicilar)) return 0;

            var satirlar = File.ReadAllLines(dosyaKullanicilar).ToList();
            var yeniListe = new List<string>();
            int silinenSayisi = 0;

            // 2. Dosyadan Sil
            foreach (var satir in satirlar)
            {
                if (string.IsNullOrWhiteSpace(satir)) continue;
                var parcalar = satir.Split(';');

                if (parcalar.Length > 0)
                {
                    string dosyadakiID = IDTemizle(parcalar[0]);
                    if (dosyadakiID == silinecekID)
                    {
                        silinenSayisi++; // Listeye ekleme, yani sil
                    }
                    else
                    {
                        yeniListe.Add(satir); // Koru
                    }
                }
            }

            File.WriteAllLines(dosyaKullanicilar, yeniListe);
            return silinenSayisi;
        }

        public static void KullaniciEkle(string kartID, string adSoyad)
        {
            string temizID = IDTemizle(kartID);
            KullaniciSil(temizID); // Önce eskisi varsa sil
            string satir = temizID + ";" + adSoyad.Trim();
            File.AppendAllText(dosyaKullanicilar, satir + Environment.NewLine);
        }

        public static string KullaniciAdiGetir(string kartID)
        {
            string arananID = IDTemizle(kartID);
            if (!File.Exists(dosyaKullanicilar)) return "Tanımsız";

            foreach (var satir in File.ReadAllLines(dosyaKullanicilar))
            {
                if (string.IsNullOrWhiteSpace(satir)) continue;
                var parcalar = satir.Split(';');
                if (parcalar.Length > 1 && IDTemizle(parcalar[0]) == arananID)
                {
                    return parcalar[1].Trim();
                }
            }
            return "Tanımsız";
        }

        public static string GirisCikisIsle(string kartID)
        {
            string islenenID = IDTemizle(kartID);
            string ad = KullaniciAdiGetir(islenenID);

            // KART BULUNAMADIYSA BURADAN DÖNER
            if (ad == "Tanımsız") return "TANIMSIZ KART";

            // Listede yoksa ekle
            if (!Iceridekiler.ContainsKey(islenenID)) Iceridekiler[islenenID] = false;

            bool suAnIceride = Iceridekiler[islenenID];
            string islemTuru = suAnIceride ? "ÇIKIŞ" : "GİRİŞ";

            Iceridekiler[islenenID] = !suAnIceride;

            string logSatiri = $"{DateTime.Now};{islenenID};{ad};{islemTuru}";
            File.AppendAllText(dosyaLoglar, logSatiri + Environment.NewLine);

            return $"{islemTuru} YAPILDI: {ad}";
        }

        // ... Yönetici Metotları Aynen Kalabilir ...
        public static void YoneticiKaydet(string id, string sifre) => File.WriteAllText(dosyaYonetici, id.Trim() + ";" + sifre.Trim());
        public static bool YoneticiGirisKontrol(string id, string sifre)
        {
            if (!File.Exists(dosyaYonetici)) return false;
            string[] parcalar = File.ReadAllText(dosyaYonetici).Split(';');
            return (parcalar.Length > 1 && parcalar[0].Trim() == id.Trim() && parcalar[1].Trim() == sifre.Trim());
        }
        public static bool YoneticiVarMi() => File.Exists(dosyaYonetici);
    }
}