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

    public partial class FrmYoneticiKayit : Form

    {

        public FrmYoneticiKayit()

        {

            InitializeComponent();

        }



        private void btnKaydet_Click(object sender, EventArgs e)

        {

            if (txtYoneticiID.Text == "" || txtSifre.Text == "")

            {

                MessageBox.Show("Alanlar boş geçilemez!");

                return;

            }



            VeriYoneticisi.YoneticiKaydet(txtYoneticiID.Text, txtSifre.Text);

            MessageBox.Show("Yönetici Başarıyla Kaydedildi!");

            this.Close();

        }

    }

}