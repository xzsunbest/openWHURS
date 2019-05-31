using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void Character_Click(object sender, EventArgs e)
        {
            MenuID = 9;
            int xs, ys;
            string output;
            MyForm2 myf = new MyForm2();
            if (myf.ShowDialog() == DialogResult.Cancel)
            {
                myf.Close();
                return;
            }
            xs = myf.X0;
            ys = myf.Y0;
            output = myf.Output;
            myf.Close();
            Graphics g = CreateGraphics();
            FontDialog fontDialog = new FontDialog();
            Font MyFont;
            if (fontDialog.ShowDialog() != DialogResult.Cancel)
            {
                MyFont = new Font(fontDialog.Font, FontStyle.Regular);
            }
            else
            {
                MyFont = new Font("Arial", 16);
            }
            ColorDialog colorDialog = new ColorDialog();
            SolidBrush MySolidBrush;
            if (colorDialog.ShowDialog() != DialogResult.Cancel)
            {
                MySolidBrush = new SolidBrush(colorDialog.Color);
            }
            else
            {
                MySolidBrush = new SolidBrush(Color.Black);
            }
            PointF p = new PointF(xs, ys);
            g.DrawString(output, MyFont, MySolidBrush, p);
        }
    }
}
