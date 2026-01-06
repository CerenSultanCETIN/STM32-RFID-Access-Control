using System;

using System.Drawing;

using System.IO.Ports;

using System.Windows.Forms;



namespace manyetik_giris_karti

{

    public partial class Form1 : Form

    {

        // === ESKİ YEDEK TANIMLAR (Veritabanı dışı kartlar için) ===

        //string KART_1_ID = "82-B8-4A-4E-3E";

        //string KART_2_ID = "F3-24-BA-1B-76";

        //string KART_3_ID = "03-E0-76-FF-6A";



        public Form1()

        {

            InitializeComponent();

            PortlariGetir();

        }



        // Portları bulup kutuya ekleyen fonksiyon

        private void PortlariGetir()

        {

            cmbPorts.Items.Clear();

            string[] ports = SerialPort.GetPortNames();



            foreach (string port in ports)

            {

                cmbPorts.Items.Add(port);

            }



            // Otomatik seçim yapalım (Kolaylık olsun)

            if (ports.Length > 0)

                cmbPorts.SelectedIndex = 0;

            else

                MessageBox.Show("Bilgisayarda bağlı hiç COM Port bulunamadı!\nLütfen STM32'yi bağladığından emin ol.");

        }



        private void btnBaglan_Click(object sender, EventArgs e)

        {

            try

            {

                if (cmbPorts.SelectedItem == null)

                {

                    PortlariGetir();

                    if (cmbPorts.SelectedItem == null)

                    {

                        MessageBox.Show("Lütfen listeden bir Port seçiniz!");

                        return;

                    }

                }



                // Port zaten açıksa kapat

                if (serialPort1.IsOpen) serialPort1.Close();



                serialPort1.PortName = cmbPorts.SelectedItem.ToString();

                serialPort1.BaudRate = 9600;

                serialPort1.Open();



                // Olayı bağla (Çift tıklamayı önlemek için önce çıkarıp sonra ekliyoruz)

                serialPort1.DataReceived -= SerialPort1_DataReceived;

                serialPort1.DataReceived += SerialPort1_DataReceived;



                btnBaglan.Text = "BAĞLI";

                btnBaglan.BackColor = Color.LightGreen;

                MessageBox.Show("Bağlantı Başarılı!");

            }

            catch (Exception ex)

            {

                MessageBox.Show("Bağlantı Hatası: " + ex.Message);

                btnBaglan.BackColor = Color.Red;

            }

        }



        private void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)

        {

            try

            {

                string gelenVeri = serialPort1.ReadLine();

                this.Invoke(new MethodInvoker(delegate { VeriyiIsle(gelenVeri); }));

            }

            catch { }

        }



        // === TEK VE DÜZELTİLMİŞ VERİ İŞLEME FONKSİYONU ===


        // Form1.cs içindeki metodun
        private void VeriyiIsle(string veri)
        {
            // 1. KEYPAD KONTROLÜ
            if (veri.Contains("KEY:"))
            {
                string tus = veri.Replace("KEY:", "").Trim();

                // === YÖNLENDİRME BAŞLIYOR ===

                // Eğer Yönetici Giriş Ekranı Açıksa, tuşu oraya gönder!
                if (FrmGiris.AcikGirisEkrani != null)
                {
                    FrmGiris.AcikGirisEkrani.KeypadtenYaz(tus);
                    return; // Ana ekrana veya Windows'a gönderme, buradan çık.
                }

                // ============================

                // Eğer giriş ekranı kapalıysa, normal bilgisayar klavyesi gibi davran (Eski kodun)
                SendKeys.Send(tus);
                return;
            }

            // 2. KART OKUYUCU KONTROLÜ
            if (!veri.Contains("UID")) return;

            try
            {
                string[] parcalar = veri.Split(',');
                string safID = parcalar[0].Replace("UID:", "").Trim();
                string safTarih = parcalar[1].Replace("TARIH:", "").Trim();

                // === YÖNETİCİ PANELİ KONTROLÜ (Kayıt İçin) ===
                if (FrmPanel.AcikPanel != null)
                {
                    FrmPanel.AcikPanel.KartIDGetir(safID);
                    return; // Giriş yapma, sadece panele gönder ve çık.
                }

                // === NORMAL KAPI GİRİŞ SİSTEMİ ===
                lblTarih.Text = safTarih;
                lblKartID.Text = "KART ID: " + safID;

                // Veri Yöneticisine Sor: Bu kim?
                string sonuc = VeriYoneticisi.GirisCikisIsle(safID);

                // === RENK VE MESAJ AYARLAMASI (BURASI ÇOK ÖNEMLİ) ===
                if (sonuc == "TANIMSIZ KART")
                {
                    // Tanımsız Kart Durumu
                    lblAdSoyad.Text = "BU KART TANIMSIZ!";
                    lblAdSoyad.ForeColor = Color.Red;
                    this.BackColor = Color.Red;         // Arka plan KIPKIRMIZI

                    // Uyarı sesi (Bilgisayardan)
                    System.Media.SystemSounds.Hand.Play();
                }
                else
                {
                    // Tanımlı Kart (Giriş veya Çıkış)
                    lblAdSoyad.Text = sonuc;
                    lblAdSoyad.ForeColor = Color.Black;

                    if (sonuc.Contains("GİRİŞ"))
                    {
                        this.BackColor = Color.LightGreen; // Girişte Yeşil
                    }
                    else
                    {
                        this.BackColor = Color.Orange;     // Çıkışta Turuncu
                    }
                }
            }
            catch (Exception ex)
            {
                lblAdSoyad.Text = "Hata: " + ex.Message;
            }
        }



        private void Form1_FormClosing(object sender, FormClosingEventArgs e)

        {

            if (serialPort1.IsOpen) serialPort1.Close();

        }



        // BUTON YÖNLENDİRMELERİ

        private void btnYoneticiKayit_Click(object sender, EventArgs e)

        {

            FrmYoneticiKayit frm = new FrmYoneticiKayit();

            frm.ShowDialog();

        }



        private void btnYoneticiGiris_Click(object sender, EventArgs e)

        {

            FrmGiris frm = new FrmGiris();

            frm.ShowDialog();

        }

    }

}



