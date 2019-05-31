using System;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace ComputerGraphics
{
    public partial class Form1 : Form
    {
        private void PerspectiveProjection_Click(object sender, EventArgs e)
        {
            MenuID = 42;
            Projection1();
        }

        private Point PerspectiveP(Point3D ViewP, Point3D ModeP)
        {
            Point p;
            int x, y;
            x = (int)(ViewP.X + (ModeP.X - ViewP.X) * ViewP.Z / (ViewP.Z - ModeP.Z) + 0.5);
            y = (int)(ViewP.Y + (ModeP.Y - ViewP.Y) * ViewP.Z / (ViewP.Z - ModeP.Z) + 0.5);
            p = new Point(x, y);
            return p;
        }
    }
}
