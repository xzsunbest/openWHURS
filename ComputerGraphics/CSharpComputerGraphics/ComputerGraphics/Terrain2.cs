using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void Terrain2_Click(object sender, EventArgs e)
        {
            MenuID = 52;
            Terrain21();
        }

        private void Dealwith(Graphics g, double x1, double y1, double x2, double y2, double[] A)
        {
            int flag = 0;
            int xsave1 = 0, xsave2, x;
            double ysave1 = 0, ysave2, y;
            if (x1 > x2)
            {
                Swap(ref x1, ref x2);
                Swap(ref y1, ref y2);
            }
            for (x = (int)x1 + 1; x < x2; x++)
            {
                y = (y2 - y1) / (x2 - x1) * ((double)x - x1) + y1;
                if (y > A[x])
                {
                    A[x] = y;
                    if (flag == 0)
                    {
                        xsave1 = x; ysave1 = y;
                        flag = 1;
                    }
                }
                else
                {
                    if (flag == 1)
                    {
                        xsave2 = x - 1;
                        ysave2 = (y2 - y1) / (x2 - x1) * ((double)(x - 1) - x1) + y1;
                        flag = 0;
                        ysave1 = 900 - ysave1;
                        ysave2 = 900 - ysave2;
                        g.DrawLine(Pens.Black, xsave1, (int)ysave1, xsave2, (int)ysave2);
                    }
                }
            }
            if (flag == 1)
            {
                y = (y2 - y1) / (x2 - x1) * (x - x1) + y1;
                ysave1 = 900 - ysave1; y = 900 - y;
                g.DrawLine(Pens.Black, xsave1, (int)ysave1, x, (int)y);
            }
        }

        private void Terrain21()
        {
            int[,] DEM = new int[200, 200];
            DEM = ReadDEM();
            int size = 3;
            double[] A = new double[1920];
            for (int i = 0; i < 1920; i++)
                A[i] = 0.0;
            double ky = 0.6, kz = 0.6;
            Graphics g = CreateGraphics();
            g.Clear(Color.LightGray);
            int dy = 400, dz = 500;
            for (int i = 199; i >= 0; i--)
            {
                for (int j = 0; j < 199; j++)
                {
                    double y1 = j * size - i * size * ky;
                    double z1 = DEM[i, j] - i * size * kz;
                    double y2 = (j + 1) * size - i * size * ky;
                    double z2 = DEM[i, j + 1] - i * size * kz;
                    Dealwith(g, y1 + dy, z1 + dz, y2 + dy, z2 + dz, A);
                }
                for (int j = 198; j >= 0; j--)
                {
                    double y1 = i * size - j * size * ky;
                    double z1 = DEM[j, i] - j * size * kz;
                    double y2 = i * size - (j + 1) * size * ky;
                    double z2 = DEM[j + 1, i] - (j + 1) * size * kz;
                    Dealwith(g, y1 + dy, z1 + dz, y2 + dy, z2 + dz, A);
                }
            }
            for (int i = 0; i < 200; i++)
            {
                double y1 = i * size - 199 * size * ky + dy;
                double z1 = DEM[199, i] - 199 * size * kz + dz;
                double z2 = -199 * size * kz + dz;
                z1 = 900 - z1; z2 = 900 - z2;
                g.DrawLine(Pens.Black, (int)y1, (int)z1, (int)y1, (int)z2);
                y1 = 199 * size - i * size * ky + dy;
                z1 = DEM[i, 199] - i * size * kz + dz;
                z2 = -i * size * kz + dz;
                z1 = 900 - z1; z2 = 900 - z2;
                g.DrawLine(Pens.Black, (int)y1, (int)z1, (int)y1, (int)z2);
            }
        }
    }
}
