namespace ComputerGraphics
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.基本图形生成ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.DDALine = new System.Windows.Forms.ToolStripMenuItem();
            this.MidLine = new System.Windows.Forms.ToolStripMenuItem();
            this.BresenhamCircle = new System.Windows.Forms.ToolStripMenuItem();
            this.ZCircle = new System.Windows.Forms.ToolStripMenuItem();
            this.BezierCurve = new System.Windows.Forms.ToolStripMenuItem();
            this.BSampleCurve = new System.Windows.Forms.ToolStripMenuItem();
            this.HermiteCurve = new System.Windows.Forms.ToolStripMenuItem();
            this.Character = new System.Windows.Forms.ToolStripMenuItem();
            this.图形填充ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ScanLineFill = new System.Windows.Forms.ToolStripMenuItem();
            this.EdgeFill = new System.Windows.Forms.ToolStripMenuItem();
            this.FloodFill = new System.Windows.Forms.ToolStripMenuItem();
            this.二维裁剪图形ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.CohenCut = new System.Windows.Forms.ToolStripMenuItem();
            this.MidCut = new System.Windows.Forms.ToolStripMenuItem();
            this.LiangCut = new System.Windows.Forms.ToolStripMenuItem();
            this.WindowCut = new System.Windows.Forms.ToolStripMenuItem();
            this.二维图形变换ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.TransMove = new System.Windows.Forms.ToolStripMenuItem();
            this.TransRotate = new System.Windows.Forms.ToolStripMenuItem();
            this.TransScale = new System.Windows.Forms.ToolStripMenuItem();
            this.TransSymmetry = new System.Windows.Forms.ToolStripMenuItem();
            this.TransShear = new System.Windows.Forms.ToolStripMenuItem();
            this.投影ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ParallelProjection = new System.Windows.Forms.ToolStripMenuItem();
            this.PerspectiveProjection = new System.Windows.Forms.ToolStripMenuItem();
            this.SimpleProjection = new System.Windows.Forms.ToolStripMenuItem();
            this.消隐ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Terrain1 = new System.Windows.Forms.ToolStripMenuItem();
            this.Terrain2 = new System.Windows.Forms.ToolStripMenuItem();
            this.ZBuffer = new System.Windows.Forms.ToolStripMenuItem();
            this.光照ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Basic = new System.Windows.Forms.ToolStripMenuItem();
            this.UniformSampling = new System.Windows.Forms.ToolStripMenuItem();
            this.StratifiedSampling = new System.Windows.Forms.ToolStripMenuItem();
            this.JitteredSampling = new System.Windows.Forms.ToolStripMenuItem();
            this.ConstructiveSolidGeometry = new System.Windows.Forms.ToolStripMenuItem();
            this.Reflection = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.基本图形生成ToolStripMenuItem,
            this.图形填充ToolStripMenuItem,
            this.二维裁剪图形ToolStripMenuItem,
            this.二维图形变换ToolStripMenuItem,
            this.投影ToolStripMenuItem,
            this.消隐ToolStripMenuItem,
            this.光照ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1902, 28);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 基本图形生成ToolStripMenuItem
            // 
            this.基本图形生成ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.DDALine,
            this.MidLine,
            this.BresenhamCircle,
            this.ZCircle,
            this.BezierCurve,
            this.BSampleCurve,
            this.HermiteCurve,
            this.Character});
            this.基本图形生成ToolStripMenuItem.Name = "基本图形生成ToolStripMenuItem";
            this.基本图形生成ToolStripMenuItem.Size = new System.Drawing.Size(111, 24);
            this.基本图形生成ToolStripMenuItem.Text = "基本图形生成";
            // 
            // DDALine
            // 
            this.DDALine.Name = "DDALine";
            this.DDALine.Size = new System.Drawing.Size(179, 26);
            this.DDALine.Text = "DDA直线";
            this.DDALine.Click += new System.EventHandler(this.DDALine_Click);
            // 
            // MidLine
            // 
            this.MidLine.Name = "MidLine";
            this.MidLine.Size = new System.Drawing.Size(179, 26);
            this.MidLine.Text = "中点直线";
            this.MidLine.Click += new System.EventHandler(this.MidLine_Click);
            // 
            // BresenhamCircle
            // 
            this.BresenhamCircle.Name = "BresenhamCircle";
            this.BresenhamCircle.Size = new System.Drawing.Size(179, 26);
            this.BresenhamCircle.Text = "Bresenham圆";
            this.BresenhamCircle.Click += new System.EventHandler(this.BresenhamCircle_Click);
            // 
            // ZCircle
            // 
            this.ZCircle.Name = "ZCircle";
            this.ZCircle.Size = new System.Drawing.Size(179, 26);
            this.ZCircle.Text = "正负法圆";
            this.ZCircle.Click += new System.EventHandler(this.ZCircle_Click);
            // 
            // BezierCurve
            // 
            this.BezierCurve.Name = "BezierCurve";
            this.BezierCurve.Size = new System.Drawing.Size(179, 26);
            this.BezierCurve.Text = "Bezier曲线";
            this.BezierCurve.Click += new System.EventHandler(this.BezierCurve_Click);
            // 
            // BSampleCurve
            // 
            this.BSampleCurve.Name = "BSampleCurve";
            this.BSampleCurve.Size = new System.Drawing.Size(179, 26);
            this.BSampleCurve.Text = "B样条曲线";
            this.BSampleCurve.Click += new System.EventHandler(this.BSampleCurve_Click);
            // 
            // HermiteCurve
            // 
            this.HermiteCurve.Name = "HermiteCurve";
            this.HermiteCurve.Size = new System.Drawing.Size(179, 26);
            this.HermiteCurve.Text = "Hermite曲线";
            this.HermiteCurve.Click += new System.EventHandler(this.HermiteCurve_Click);
            // 
            // Character
            // 
            this.Character.Name = "Character";
            this.Character.Size = new System.Drawing.Size(179, 26);
            this.Character.Text = "字符";
            this.Character.Click += new System.EventHandler(this.Character_Click);
            // 
            // 图形填充ToolStripMenuItem
            // 
            this.图形填充ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ScanLineFill,
            this.EdgeFill,
            this.FloodFill});
            this.图形填充ToolStripMenuItem.Name = "图形填充ToolStripMenuItem";
            this.图形填充ToolStripMenuItem.Size = new System.Drawing.Size(81, 24);
            this.图形填充ToolStripMenuItem.Text = "图形填充";
            // 
            // ScanLineFill
            // 
            this.ScanLineFill.Name = "ScanLineFill";
            this.ScanLineFill.Size = new System.Drawing.Size(189, 26);
            this.ScanLineFill.Text = "扫描线填充算法";
            this.ScanLineFill.Click += new System.EventHandler(this.ScanLineFill_Click);
            // 
            // EdgeFill
            // 
            this.EdgeFill.Name = "EdgeFill";
            this.EdgeFill.Size = new System.Drawing.Size(189, 26);
            this.EdgeFill.Text = "边缘填充";
            this.EdgeFill.Click += new System.EventHandler(this.EdgeFill_Click);
            // 
            // FloodFill
            // 
            this.FloodFill.Name = "FloodFill";
            this.FloodFill.Size = new System.Drawing.Size(189, 26);
            this.FloodFill.Text = "种子填充";
            this.FloodFill.Click += new System.EventHandler(this.FloodFill_Click);
            // 
            // 二维裁剪图形ToolStripMenuItem
            // 
            this.二维裁剪图形ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.CohenCut,
            this.MidCut,
            this.LiangCut,
            this.WindowCut});
            this.二维裁剪图形ToolStripMenuItem.Name = "二维裁剪图形ToolStripMenuItem";
            this.二维裁剪图形ToolStripMenuItem.Size = new System.Drawing.Size(111, 24);
            this.二维裁剪图形ToolStripMenuItem.Text = "二维裁剪图形";
            // 
            // CohenCut
            // 
            this.CohenCut.Name = "CohenCut";
            this.CohenCut.Size = new System.Drawing.Size(174, 26);
            this.CohenCut.Text = "Cohen算法";
            this.CohenCut.Click += new System.EventHandler(this.CohenCut_Click);
            // 
            // MidCut
            // 
            this.MidCut.Name = "MidCut";
            this.MidCut.Size = new System.Drawing.Size(174, 26);
            this.MidCut.Text = "中点分割算法";
            this.MidCut.Click += new System.EventHandler(this.MidCut_Click);
            // 
            // LiangCut
            // 
            this.LiangCut.Name = "LiangCut";
            this.LiangCut.Size = new System.Drawing.Size(174, 26);
            this.LiangCut.Text = "梁友栋算法";
            this.LiangCut.Click += new System.EventHandler(this.LiangCut_Click);
            // 
            // WindowCut
            // 
            this.WindowCut.Name = "WindowCut";
            this.WindowCut.Size = new System.Drawing.Size(174, 26);
            this.WindowCut.Text = "多边形裁剪";
            this.WindowCut.Click += new System.EventHandler(this.WindowCut_Click);
            // 
            // 二维图形变换ToolStripMenuItem
            // 
            this.二维图形变换ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TransMove,
            this.TransRotate,
            this.TransScale,
            this.TransSymmetry,
            this.TransShear});
            this.二维图形变换ToolStripMenuItem.Name = "二维图形变换ToolStripMenuItem";
            this.二维图形变换ToolStripMenuItem.Size = new System.Drawing.Size(111, 24);
            this.二维图形变换ToolStripMenuItem.Text = "二维图形变换";
            // 
            // TransMove
            // 
            this.TransMove.Name = "TransMove";
            this.TransMove.Size = new System.Drawing.Size(144, 26);
            this.TransMove.Text = "图形平移";
            this.TransMove.Click += new System.EventHandler(this.TransMove_Click);
            // 
            // TransRotate
            // 
            this.TransRotate.Name = "TransRotate";
            this.TransRotate.Size = new System.Drawing.Size(144, 26);
            this.TransRotate.Text = "图形旋转";
            this.TransRotate.Click += new System.EventHandler(this.TransRotate_Click);
            // 
            // TransScale
            // 
            this.TransScale.Name = "TransScale";
            this.TransScale.Size = new System.Drawing.Size(144, 26);
            this.TransScale.Text = "图形缩放";
            this.TransScale.Click += new System.EventHandler(this.TransScale_Click);
            // 
            // TransSymmetry
            // 
            this.TransSymmetry.Name = "TransSymmetry";
            this.TransSymmetry.Size = new System.Drawing.Size(144, 26);
            this.TransSymmetry.Text = "对称变换";
            this.TransSymmetry.Click += new System.EventHandler(this.TransSymmetry_Click);
            // 
            // TransShear
            // 
            this.TransShear.Name = "TransShear";
            this.TransShear.Size = new System.Drawing.Size(144, 26);
            this.TransShear.Text = "错切变换";
            this.TransShear.Click += new System.EventHandler(this.TransShear_Click);
            // 
            // 投影ToolStripMenuItem
            // 
            this.投影ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ParallelProjection,
            this.PerspectiveProjection,
            this.SimpleProjection});
            this.投影ToolStripMenuItem.Name = "投影ToolStripMenuItem";
            this.投影ToolStripMenuItem.Size = new System.Drawing.Size(51, 24);
            this.投影ToolStripMenuItem.Text = "投影";
            // 
            // ParallelProjection
            // 
            this.ParallelProjection.Name = "ParallelProjection";
            this.ParallelProjection.Size = new System.Drawing.Size(144, 26);
            this.ParallelProjection.Text = "平行投影";
            this.ParallelProjection.Click += new System.EventHandler(this.ParallelProjection_Click);
            // 
            // PerspectiveProjection
            // 
            this.PerspectiveProjection.Name = "PerspectiveProjection";
            this.PerspectiveProjection.Size = new System.Drawing.Size(144, 26);
            this.PerspectiveProjection.Text = "透视投影";
            this.PerspectiveProjection.Click += new System.EventHandler(this.PerspectiveProjection_Click);
            // 
            // SimpleProjection
            // 
            this.SimpleProjection.Name = "SimpleProjection";
            this.SimpleProjection.Size = new System.Drawing.Size(144, 26);
            this.SimpleProjection.Text = "简单投影";
            this.SimpleProjection.Click += new System.EventHandler(this.SimpleProjection_Click);
            // 
            // 消隐ToolStripMenuItem
            // 
            this.消隐ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Terrain1,
            this.Terrain2,
            this.ZBuffer});
            this.消隐ToolStripMenuItem.Name = "消隐ToolStripMenuItem";
            this.消隐ToolStripMenuItem.Size = new System.Drawing.Size(51, 24);
            this.消隐ToolStripMenuItem.Text = "消隐";
            // 
            // Terrain1
            // 
            this.Terrain1.Name = "Terrain1";
            this.Terrain1.Size = new System.Drawing.Size(168, 26);
            this.Terrain1.Text = "地形显示1";
            this.Terrain1.Click += new System.EventHandler(this.Terrain1_Click);
            // 
            // Terrain2
            // 
            this.Terrain2.Name = "Terrain2";
            this.Terrain2.Size = new System.Drawing.Size(168, 26);
            this.Terrain2.Text = "地形显示2";
            this.Terrain2.Click += new System.EventHandler(this.Terrain2_Click);
            // 
            // ZBuffer
            // 
            this.ZBuffer.Name = "ZBuffer";
            this.ZBuffer.Size = new System.Drawing.Size(168, 26);
            this.ZBuffer.Text = "Z缓冲区算法";
            this.ZBuffer.Click += new System.EventHandler(this.ZBuffer_Click);
            // 
            // 光照ToolStripMenuItem
            // 
            this.光照ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Basic,
            this.ConstructiveSolidGeometry,
            this.Reflection});
            this.光照ToolStripMenuItem.Name = "光照ToolStripMenuItem";
            this.光照ToolStripMenuItem.Size = new System.Drawing.Size(51, 24);
            this.光照ToolStripMenuItem.Text = "光照";
            // 
            // Basic
            // 
            this.Basic.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.UniformSampling,
            this.StratifiedSampling,
            this.JitteredSampling});
            this.Basic.Name = "Basic";
            this.Basic.Size = new System.Drawing.Size(181, 26);
            this.Basic.Text = "基本";
            // 
            // UniformSampling
            // 
            this.UniformSampling.Name = "UniformSampling";
            this.UniformSampling.Size = new System.Drawing.Size(144, 26);
            this.UniformSampling.Text = "均匀采样";
            this.UniformSampling.Click += new System.EventHandler(this.UniformSampling_Click);
            // 
            // StratifiedSampling
            // 
            this.StratifiedSampling.Name = "StratifiedSampling";
            this.StratifiedSampling.Size = new System.Drawing.Size(144, 26);
            this.StratifiedSampling.Text = "分层采样";
            this.StratifiedSampling.Click += new System.EventHandler(this.StratifiedSampling_Click);
            // 
            // JitteredSampling
            // 
            this.JitteredSampling.Name = "JitteredSampling";
            this.JitteredSampling.Size = new System.Drawing.Size(144, 26);
            this.JitteredSampling.Text = "抖动采样";
            this.JitteredSampling.Click += new System.EventHandler(this.JitteredSampling_Click);
            // 
            // ConstructiveSolidGeometry
            // 
            this.ConstructiveSolidGeometry.Name = "ConstructiveSolidGeometry";
            this.ConstructiveSolidGeometry.Size = new System.Drawing.Size(181, 26);
            this.ConstructiveSolidGeometry.Text = "构造实体几何";
            this.ConstructiveSolidGeometry.Click += new System.EventHandler(this.ConstructiveSolidGeometry_Click);
            // 
            // Reflection
            // 
            this.Reflection.Name = "Reflection";
            this.Reflection.Size = new System.Drawing.Size(181, 26);
            this.Reflection.Text = "反射";
            this.Reflection.Click += new System.EventHandler(this.Reflection_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1902, 1033);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseClick);
            this.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDoubleClick);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 基本图形生成ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem DDALine;
        private System.Windows.Forms.ToolStripMenuItem MidLine;
        private System.Windows.Forms.ToolStripMenuItem BresenhamCircle;
        private System.Windows.Forms.ToolStripMenuItem 图形填充ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ScanLineFill;
        private System.Windows.Forms.ToolStripMenuItem 二维裁剪图形ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem CohenCut;
        private System.Windows.Forms.ToolStripMenuItem BezierCurve;
        private System.Windows.Forms.ToolStripMenuItem BSampleCurve;
        private System.Windows.Forms.ToolStripMenuItem MidCut;
        private System.Windows.Forms.ToolStripMenuItem LiangCut;
        private System.Windows.Forms.ToolStripMenuItem WindowCut;
        private System.Windows.Forms.ToolStripMenuItem 二维图形变换ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem TransMove;
        private System.Windows.Forms.ToolStripMenuItem TransRotate;
        private System.Windows.Forms.ToolStripMenuItem TransScale;
        private System.Windows.Forms.ToolStripMenuItem TransSymmetry;
        private System.Windows.Forms.ToolStripMenuItem TransShear;
        private System.Windows.Forms.ToolStripMenuItem EdgeFill;
        private System.Windows.Forms.ToolStripMenuItem FloodFill;
        private System.Windows.Forms.ToolStripMenuItem ZCircle;
        private System.Windows.Forms.ToolStripMenuItem Character;
        private System.Windows.Forms.ToolStripMenuItem 投影ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ParallelProjection;
        private System.Windows.Forms.ToolStripMenuItem PerspectiveProjection;
        private System.Windows.Forms.ToolStripMenuItem SimpleProjection;
        private System.Windows.Forms.ToolStripMenuItem 消隐ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Terrain1;
        private System.Windows.Forms.ToolStripMenuItem HermiteCurve;
        private System.Windows.Forms.ToolStripMenuItem Terrain2;
        private System.Windows.Forms.ToolStripMenuItem ZBuffer;
        private System.Windows.Forms.ToolStripMenuItem 光照ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Basic;
        private System.Windows.Forms.ToolStripMenuItem UniformSampling;
        private System.Windows.Forms.ToolStripMenuItem StratifiedSampling;
        private System.Windows.Forms.ToolStripMenuItem JitteredSampling;
        private System.Windows.Forms.ToolStripMenuItem ConstructiveSolidGeometry;
        private System.Windows.Forms.ToolStripMenuItem Reflection;
    }
}

