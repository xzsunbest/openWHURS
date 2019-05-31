using System;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void SimpleProjection_Click(object sender, EventArgs e)
        {
            MenuID = 43;
            Projection1();
        }

        private Point SimpleP(Point3D p3d)
        {
            Point p;
            float kx, ky;
            kx = (float)0.4;
            ky = (float)0.3;
            p = new Point((int)(p3d.X - kx * p3d.Z + 0.5), (int)(p3d.Y - ky * p3d.Z + 0.5));
            return p;
        }
    }
}
