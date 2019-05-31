using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void BresenhamCircle_Click(object sender, EventArgs e)
        {
            MenuID = 5;
            PressNum = 0;
            Graphics g = CreateGraphics();
            g.Clear(BackColor1);
        }

        private void BresenhamCircle1(int x0, int y0, int x1, int y1)
        {
            int r, d, x, y;
            Graphics g = CreateGraphics();
            r = (int)(Math.Sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0)) + 0.5);
            x = 0; y = r; d = 3 - 2 * r;
            while (x < y || x == y)
            {
                g.DrawRectangle(Pens.Red, x + x0, y + y0, 1, 1);
                g.DrawRectangle(Pens.Red, -x + x0, y + y0, 1, 1);
                g.DrawRectangle(Pens.Red, x + x0, -y + y0, 1, 1);
                g.DrawRectangle(Pens.Red, -x + x0, -y + y0, 1, 1);
                g.DrawRectangle(Pens.Red, y + x0, x + y0, 1, 1);
                g.DrawRectangle(Pens.Red, -y + x0, x + y0, 1, 1);
                g.DrawRectangle(Pens.Red, y + x0, -x + y0, 1, 1);
                g.DrawRectangle(Pens.Red, -y + x0, -x + y0, 1, 1);
                x++;
                if (d <= 0)
                {
                    d = d + 4 * x + 6;
                }
                else
                {
                    y--;
                    d = d + 4 * (x - y) + 10;
                }
            }
        }
    }
}
