using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace CLRDemo
{
    public partial class Form1 : Form
    {
        Axelynx.Engine eng;
        Axelynx.Window wnd;
        Axelynx.Canvas c;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            eng = Axelynx.Engine.Init();
            //wnd = eng.AddWindow(640, 480, 32, Axelynx.WindowMode.WINDOWED, 0);
            wnd = eng.ApplyWindow(renderpanel.Handle, 32, 0);


            c = eng.GetCanvas();
            //Axelynx.Font fnt = eng.LoadFreeTypeFont("arial", 16);
           // c.SetFont(fnt);
            c.ClearColor(0.12f, 0.05f, 0.03f, 1.0f);
            timer1.Start();

        }

        private void renderpanel_Paint(object sender, PaintEventArgs e)
        {

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            eng.Free();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            c.ClearColor((float)trackBar1.Value / 64.0f
                , (float)trackBar2.Value / 64.0f
                , (float)trackBar3.Value / 64.0f
                , 1.0f);

            c.Clear();
            wnd.Flip(true);
        }

    }
}
