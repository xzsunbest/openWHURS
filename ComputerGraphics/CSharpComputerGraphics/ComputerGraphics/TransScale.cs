using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void TransScale_Click(object sender, EventArgs e)
        {
            MenuID = 13;
            float xs, ys;
            MyForm myf = new MyForm();
            if (myf.ShowDialog() == DialogResult.Cancel)
            {
                myf.Close();
                return;
            }
            xs = myf.Xscale;
            ys = myf.Yscale;
            myf.Close();
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
            pointsgroup[0] = new Point(100, 100);
            pointsgroup[1] = new Point(200, 100);
            pointsgroup[2] = new Point(200, 200);
            pointsgroup[3] = new Point(100, 200);
            g.DrawPolygon(Pens.Red, pointsgroup);
            Matrix myMatrix = new Matrix();
            myMatrix.Translate(-100, -100);
            myMatrix.Scale(xs, ys, MatrixOrder.Append);
            myMatrix.Translate(100, 100, MatrixOrder.Append);
            g.Transform = myMatrix;
            g.DrawPolygon(Pens.Blue, pointsgroup);
        }
    }
}
