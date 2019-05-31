using System;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void ParallelProjection_Click(object sender, EventArgs e)
        {
            MenuID = 41;
            Projection1();
        }

        private Point ParallelP(Point3D ViewP, Point3D ModeP)
        {
            Point p;
            int x, y;
            x = (int)(ModeP.X - ViewP.X / ViewP.Z * ModeP.Z + 0.5);
            y = (int)(ModeP.Y - ViewP.Y / ViewP.Z * ModeP.Z + 0.5);
            p = new Point(x, y);
            return p;
        }
    }
}
