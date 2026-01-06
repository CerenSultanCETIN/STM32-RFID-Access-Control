using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace manyetik_giris_karti

{

    public partial class FrmPanel : Form

    {

        // KÖPRÜ DEĞİŞKENİ

        public static FrmPanel AcikPanel = null;



        public FrmPanel()

        {

            InitializeComponent();

        }



        // === TEK VE DOĞRU LOAD FONKSİYONU ===

        private void FrmPanel_Load(object sender, EventArgs e)

        {

            // Form açıldığında köprüyü kuruyoruz

            AcikPanel = this;

            LoglariGetir();

        }



        private void FrmPanel_FormClosing(object sender, FormClosingEventArgs e)

        {

            // Form kapanırken köprüyü yıkıyoruz

            AcikPanel = null;

        }



        // BU FONKSİYON Form1 TARAFINDAN ÇAĞIRILACAK

        public void KartIDGetir(string id)
        {
            // Gelen ID'yi her iki kutuya da yazalım, hangisi lazımsa onu kullanırsın.
            txtYeniKartID.Text = id;   // Ekleme kutusu
            txtSilinecekID.Text = id;  // Silme kutusu

            // Kullanıcıya görsel bir tepki verelim (isteğe bağlı)
            txtYeniKartID.BackColor = System.Drawing.Color.LightYellow;

            
        }


        private void btnEkle_Click(object sender, EventArgs e)

        {

            VeriYoneticisi.KullaniciEkle(txtYeniKartID.Text, txtYeniAdSoyad.Text);

            MessageBox.Show("Kullanıcı Eklendi!");

            txtYeniKartID.Clear();

            txtYeniAdSoyad.Clear();

        }



        private void btnSil_Click(object sender, EventArgs e)

        {

            // Silme fonksiyonu artık bir sayı döndürüyor
            int silinenAdet = VeriYoneticisi.KullaniciSil(txtSilinecekID.Text);

            if (silinenAdet > 0)
            {
                MessageBox.Show("Kullanıcı Başarıyla Silindi!");
                txtSilinecekID.Clear();
            }
            else
            {
                MessageBox.Show("HATA: Bu ID'ye sahip bir kullanıcı bulunamadı!\nID'yi kontrol edip tekrar deneyin.");
            }

        }



        // BURADAKİ FAZLALIK FrmPanel_Load SİLİNDİ!



        private void btnYenile_Click(object sender, EventArgs e)

        {

            LoglariGetir();

        }



        void LoglariGetir()

        {

            if (!System.IO.File.Exists("loglar.txt")) return;



            System.Data.DataTable dt = new System.Data.DataTable();

            dt.Columns.Add("Zaman");

            dt.Columns.Add("Kart ID");

            dt.Columns.Add("Ad Soyad");

            dt.Columns.Add("İşlem");



            string[] satirlar = System.IO.File.ReadAllLines("loglar.txt");

            foreach (string satir in satirlar)

            {

                if (string.IsNullOrWhiteSpace(satir)) continue;

                string[] parcalar = satir.Split(';');



                // Hata önleyici: Eğer satır bozuksa atla

                if (parcalar.Length >= 4)

                    dt.Rows.Add(parcalar);

            }



            dataGridView1.DataSource = dt;

        }

    }

}