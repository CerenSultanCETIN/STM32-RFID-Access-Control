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
   
    public partial class FrmGiris : Form

    {
        public static FrmGiris AcikGirisEkrani = null;
        public FrmGiris()

        {

            InitializeComponent();

        }

        private void FrmGiris_Load(object sender, EventArgs e)
        {
            // Form açıldığında köprüyü kur
            AcikGirisEkrani = this;

            // 2. ŞİFREYİ YILDIZLI YAP
            txtSifre.Multiline = false;
            txtSifre.PasswordChar = '*';
            // İstersen maksimum karakter sayısı koyabilirsin (Örn: 4 hane)
            txtSifre.MaxLength = 10;
        }

        private void FrmGiris_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Form kapanırken köprüyü yık
            AcikGirisEkrani = null;
        }

        // 3. KLAVYEDEN SADECE SAYI GİRİLMESİNİ SAĞLA
        private void txtSifre_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Eğer basılan tuş sayı değilse VE silme tuşu (Backspace) değilse engelle
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true; // "Bu tuşu görmezden gel" demektir.
            }
        }

        // 4. FORM1'DEN KEYPAD VERİSİNİ ALACAK FONKSİYON
        public void KeypadtenYaz(string tus)
        {
            // Gelen tuş sayısal mı? (0-9 arası)
            // A, B, C, D gibi harfleri şifreye eklemesin
            int sayi;
            bool sayiMi = int.TryParse(tus, out sayi);

            if (sayiMi)
            {
                txtSifre.Text += tus; // Metnin sonuna ekle

                // İmleci en sona getir (yazmaya devam etmek için)
                txtSifre.SelectionStart = txtSifre.Text.Length;
            }
            else
            {
                // İstersen '#' tuşunu "GİRİŞ YAP" butonu gibi kullanabilirsin
                if (tus == "#")
                {
                    btnGiris.PerformClick(); // Butona basılmış gibi davran
                }
                // '*' tuşunu "SİLME" (Backspace) gibi kullanabilirsin
                else if (tus == "*")
                {
                    if (txtSifre.Text.Length > 0)
                    {
                        txtSifre.Text = txtSifre.Text.Substring(0, txtSifre.Text.Length - 1);
                    }
                }
            }
        }

        private void btnGiris_Click(object sender, EventArgs e)

        {

            // VeriYoneticisi sınıfına sor: Bu kullanıcı adı ve şifre doğru mu?

            bool sonuc = VeriYoneticisi.YoneticiGirisKontrol(txtKullanici.Text, txtSifre.Text);



            if (sonuc)

            {

                // Giriş Başarılı ise Paneli Aç

                FrmPanel panel = new FrmPanel();

                this.Hide(); // Giriş ekranını gizle

                panel.ShowDialog(); // Paneli aç

                this.Close(); // Panel kapanınca bunu da kapat

            }

            else

            {

                MessageBox.Show("Hatalı Kullanıcı Adı veya Şifre!");

                txtSifre.Clear();

            }

        }

    }



}