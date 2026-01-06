namespace manyetik_giris_karti
{
    partial class Form1
    {
        /// <summary>
        ///Gerekli tasarımcı değişkeni.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///Kullanılan tüm kaynakları temizleyin.
        /// </summary>
        ///<param name="disposing">yönetilen kaynaklar dispose edilmeliyse doğru; aksi halde yanlış.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer üretilen kod

        /// <summary>
        /// Tasarımcı desteği için gerekli metot - bu metodun 
        ///içeriğini kod düzenleyici ile değiştirmeyin.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.lblAdSoyad = new System.Windows.Forms.Label();
            this.lblTarih = new System.Windows.Forms.Label();
            this.lblKartID = new System.Windows.Forms.Label();
            this.cmbPorts = new System.Windows.Forms.ComboBox();
            this.btnBaglan = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnYoneticiGirisi = new System.Windows.Forms.Button();
            this.btnYoneticiKayit = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // serialPort1
            // 
            this.serialPort1.PortName = "COM3";
            // 
            // lblAdSoyad
            // 
            this.lblAdSoyad.AutoSize = true;
            this.lblAdSoyad.Location = new System.Drawing.Point(19, 107);
            this.lblAdSoyad.Name = "lblAdSoyad";
            this.lblAdSoyad.Size = new System.Drawing.Size(70, 16);
            this.lblAdSoyad.TabIndex = 0;
            this.lblAdSoyad.Text = "Ad Soyad:";
            // 
            // lblTarih
            // 
            this.lblTarih.AutoSize = true;
            this.lblTarih.Location = new System.Drawing.Point(19, 154);
            this.lblTarih.Name = "lblTarih";
            this.lblTarih.Size = new System.Drawing.Size(52, 16);
            this.lblTarih.TabIndex = 1;
            this.lblTarih.Text = "Zaman:";
            // 
            // lblKartID
            // 
            this.lblKartID.AutoSize = true;
            this.lblKartID.Location = new System.Drawing.Point(19, 57);
            this.lblKartID.Name = "lblKartID";
            this.lblKartID.Size = new System.Drawing.Size(75, 16);
            this.lblKartID.TabIndex = 2;
            this.lblKartID.Text = "Kullanıcı ID:";
            // 
            // cmbPorts
            // 
            this.cmbPorts.FormattingEnabled = true;
            this.cmbPorts.Location = new System.Drawing.Point(48, 55);
            this.cmbPorts.Name = "cmbPorts";
            this.cmbPorts.Size = new System.Drawing.Size(140, 24);
            this.cmbPorts.TabIndex = 3;
            // 
            // btnBaglan
            // 
            this.btnBaglan.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.btnBaglan.Location = new System.Drawing.Point(63, 85);
            this.btnBaglan.Name = "btnBaglan";
            this.btnBaglan.Size = new System.Drawing.Size(101, 23);
            this.btnBaglan.TabIndex = 4;
            this.btnBaglan.Text = "BAĞLAN";
            this.btnBaglan.UseVisualStyleBackColor = false;
            this.btnBaglan.Click += new System.EventHandler(this.btnBaglan_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.SystemColors.Info;
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.cmbPorts);
            this.groupBox1.Controls.Add(this.btnBaglan);
            this.groupBox1.Location = new System.Drawing.Point(36, 26);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(239, 144);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Port Ayarları";
            // 
            // groupBox2
            // 
            this.groupBox2.BackColor = System.Drawing.SystemColors.Info;
            this.groupBox2.Controls.Add(this.lblKartID);
            this.groupBox2.Controls.Add(this.lblAdSoyad);
            this.groupBox2.Controls.Add(this.lblTarih);
            this.groupBox2.Location = new System.Drawing.Point(335, 68);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(341, 264);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Kullanıcı Girişleri";
            // 
            // groupBox3
            // 
            this.groupBox3.BackColor = System.Drawing.SystemColors.Info;
            this.groupBox3.Controls.Add(this.btnYoneticiGirisi);
            this.groupBox3.Controls.Add(this.btnYoneticiKayit);
            this.groupBox3.Location = new System.Drawing.Point(36, 250);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(239, 148);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Yönetici İşlemleri";
            // 
            // btnYoneticiGirisi
            // 
            this.btnYoneticiGirisi.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.btnYoneticiGirisi.Location = new System.Drawing.Point(23, 43);
            this.btnYoneticiGirisi.Name = "btnYoneticiGirisi";
            this.btnYoneticiGirisi.Size = new System.Drawing.Size(201, 63);
            this.btnYoneticiGirisi.TabIndex = 1;
            this.btnYoneticiGirisi.Text = "Yönetici Giriş";
            this.btnYoneticiGirisi.UseVisualStyleBackColor = false;
            this.btnYoneticiGirisi.Click += new System.EventHandler(this.btnYoneticiGiris_Click);
            // 
            // btnYoneticiKayit
            // 
            this.btnYoneticiKayit.BackColor = System.Drawing.SystemColors.InactiveCaption;
            this.btnYoneticiKayit.Location = new System.Drawing.Point(23, 112);
            this.btnYoneticiKayit.Name = "btnYoneticiKayit";
            this.btnYoneticiKayit.Size = new System.Drawing.Size(201, 30);
            this.btnYoneticiKayit.TabIndex = 0;
            this.btnYoneticiKayit.Text = "Yönetici Kayıt";
            this.btnYoneticiKayit.UseVisualStyleBackColor = false;
            this.btnYoneticiKayit.Click += new System.EventHandler(this.btnYoneticiKayit_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(45, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 16);
            this.label1.TabIndex = 5;
            this.label1.Text = "Port Seçiniz";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.ClientSize = new System.Drawing.Size(699, 450);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Label lblAdSoyad;
        private System.Windows.Forms.Label lblTarih;
        private System.Windows.Forms.Label lblKartID;
        private System.Windows.Forms.ComboBox cmbPorts;
        private System.Windows.Forms.Button btnBaglan;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnYoneticiKayit;
        private System.Windows.Forms.Button btnYoneticiGirisi;
        private System.Windows.Forms.Label label1;
    }
}

