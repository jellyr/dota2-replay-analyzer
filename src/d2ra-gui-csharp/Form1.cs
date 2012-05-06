using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Nabu.Forms.Html;
using Nabu.Collections;

namespace d2ra_gui_csharp
{
    public partial class Form1 : HtmlForm
    {
        public Form1()
        {
            InitializeComponent();
            this.Load += new EventHandler(OnLoad);
        }

        void OnLoad(object sender, EventArgs e)
        {
            string s = Path.GetDirectoryName(Application.ExecutablePath) + Path.DirectorySeparatorChar + "html" + Path.DirectorySeparatorChar;
            this.BaseUrl = new Uri(s);

            string html = BaseUrl + "index.html";

            LoadHtml(new Uri(html));
        }
    }
}
