using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void TransSymmetry_Click(object sender, EventArgs e)
        {
            MenuID = 14;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
            pointsgroup[0] = new Point(100, 100);
            pointsgroup[1] = new Point(200, 100);
            pointsgroup[2] = new Point(200, 200);
            pointsgroup[3] = new Point(100, 200);
            g.DrawPolygon(Pens.Red, pointsgroup);
        }

        private void TransSymmetry1(int x1, int y1, int x2, int y2)
        {
            if (x1 == x2 && y1 == y2) return;
            double angle;
            if (x1 == x2 && y1 < y2) angle = PI / 2.0;
            else if (x1 == x2 && y1 > y2) angle = 3.0 * PI / 2.0;
            else angle = Math.Atan((double)(y2 - y1) / (double)(x2 - x1));
            angle = angle * 180.0 / PI;
            Matrix myMatrix = new Matrix();
            myMatrix.Translate(-x1, -y1);
            myMatrix.Rotate(-(float)angle, MatrixOrder.Append);
            Matrix MyM1 = new Matrix(1, 0, 0, -1, 0, 0);
            myMatrix.Multiply(MyM1, MatrixOrder.Append);
            myMatrix.Rotate((float)angle, MatrixOrder.Append);
            myMatrix.Translate(x1, y1, MatrixOrder.Append);
            Graphics g = CreateGraphics();
            g.Transform = myMatrix;
            g.DrawPolygon(Pens.Blue, pointsgroup);
        }
    }
}
