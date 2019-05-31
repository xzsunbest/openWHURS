using System;
using System.Drawing;
using System.Windows.Forms;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void Terrain1_Click(object sender, EventArgs e)
        {
            MenuID = 51;
            Terrain11();
        }

        private void Terrain11()
        {
            int[,] DEM = new int[200, 200];
            DEM = ReadDEM();
            int size = 3;
            double ky = 0.4, kz = 0.3;
            Graphics g = CreateGraphics();
            g.Clear(Color.LightGray);
            int dy = (int)(ky * size + 0.5);
            int dz = (int)(kz * size + 0.5);
            for (int i = 0; i < 200; i++)
            {
                for (int j = 0; j < 200; j++)
                {
                    int y = (int)(j * size - i * size * ky);
                    int z = (int)(-i * size * kz);
                    DrawPixel(g, dy, dz, size, y, z, DEM[i, j]);
                }
            }
        }
    }
}
